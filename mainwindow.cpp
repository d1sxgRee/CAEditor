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
    , saveCodeAction(QIcon::fromTheme("document-save"), "&Save")
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, &canvas);
    saveCodeAction.setShortcut(QKeySequence::Save);
    ui->menu_File->addAction(&saveCodeAction);
    scm_init_guile();
    connect(ui->schemeButton, &QPushButton::clicked, this, &MainWindow::evalScript);
    connect(ui->playButton, &QPushButton::clicked, &canvas, &Canvas::resumeTimer);
    connect(&saveCodeAction, &QAction::triggered, this, &MainWindow::saveCode);
    QString initialСode("(set! *random-state* (random-state-from-platform))   ; Random seed for PRNG\n"
                        "(define (cell-update itself neighbours)\n"
                        "  (random 4))\n");
    ui->schemeCode->setText(initialСode);
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
    canvas.setUpdateFunction(f);
}

void MainWindow::saveCode()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "save scheme code", QString(), "*.scm");
    QFile saveFile(saveFileName);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&saveFile);
    out << ui->schemeCode->toPlainText();
}
