#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <libguile.h>
#include <QFile>
#include <QFileDialog>
#include <QIcon>
#include <QKeySequence>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canvas(new Canvas(this, 100))
    , saveCodeAction(QIcon::fromTheme("document-save"), "&Save")
    , loadCodeAction(QIcon::fromTheme("document-open"), "&Open")
    , saveFieldAction("Save field state")
    , loadFieldAction("Open field file")
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, canvas);
    saveCodeAction.setShortcut(QKeySequence::Save);
    ui->menu_File->addAction(&saveCodeAction);
    loadCodeAction.setShortcut(QKeySequence::Open);
    ui->menu_File->addAction(&loadCodeAction);
    ui->menu_File->addAction(&saveFieldAction);
    ui->menu_File->addAction(&loadFieldAction);
    scm_init_guile();
    connect(ui->schemeButton, &QPushButton::clicked, this, &MainWindow::evalScript);
    connect(ui->playButton, &QPushButton::clicked, canvas, &Canvas::resumeTimer);
    connect(&saveCodeAction, &QAction::triggered, this, &MainWindow::saveCode);
    connect(&loadCodeAction, &QAction::triggered, this, &MainWindow::loadCode);
    connect(&saveFieldAction, &QAction::triggered, this, &MainWindow::saveField);
    connect(&loadFieldAction, &QAction::triggered, this, &MainWindow::loadField);
    QString initialСode("(set! *random-state* (random-state-from-platform))   ; Random seed for PRNG\n"
                        "(define (cell-update itself neighbours)\n"
                        "  (random 4))\n");
    ui->schemeCode->setText(initialСode);
    hghlghtr = new Highlighter(ui->schemeCode->document());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::evalScript()
{
    char* rvalue;
    SCM res;
    res = scm_c_eval_string(ui->schemeCode->toPlainText().toStdString().c_str());
    SCM f;
    f = scm_variable_ref(scm_c_lookup("cell-update"));
    canvas->setUpdateFunction(f);
}

void MainWindow::saveCode()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "save scheme code file", QString(), "*.scm");
    QFile saveFile(saveFileName);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&saveFile);
    out << ui->schemeCode->toPlainText();
}

void MainWindow::loadCode()
{
    ui->schemeCode->clear();
    QString loadFileName = QFileDialog::getOpenFileName(this, "open scheme code file", QString(), "*.scm");
    QFile loadFile(loadFileName);
    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&loadFile);
    while(!in.atEnd()){
        QString line = in.readLine();
        ui->schemeCode->append(line);
    }
}

void MainWindow::saveField()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "save field state", QString(), "*.json");
    QFile saveFile(saveFileName);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    saveFile.write(canvas->toJson().toJson());
}

void MainWindow::loadField()
{
    QString loadFileName = QFileDialog::getOpenFileName(this, "open field file", QString(), "*.json");
    QFile loadFile(loadFileName);
    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    ui->verticalLayout->removeWidget(canvas);
    Canvas *canvas1 = canvas;
    disconnect(ui->playButton, &QPushButton::clicked, canvas, &Canvas::resumeTimer);
    QJsonDocument canvasJson = QJsonDocument::fromJson(loadFile.readAll());
    canvas = new Canvas(this, canvasJson);
    delete canvas1;
    ui->verticalLayout->insertWidget(0, canvas);
    connect(ui->playButton, &QPushButton::clicked, canvas, &Canvas::resumeTimer);
}
