#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QIcon>
#include <QKeySequence>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canvas(new Canvas(this, 100, 100))
    , saveCodeAction(QIcon::fromTheme("document-save"), "&Save")
    , loadCodeAction(QIcon::fromTheme("document-open"), "&Open")
    , saveFieldAction("Save field state")
    , loadFieldAction("Open field file")
    , monospaseFont("monospase")
    , highlightedSymbol(0)
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, canvas);
    saveCodeAction.setShortcut(QKeySequence::Save);
    ui->menu_File->addAction(&saveCodeAction);
    loadCodeAction.setShortcut(QKeySequence::Open);
    ui->menu_File->addAction(&loadCodeAction);
    ui->menu_File->addAction(&saveFieldAction);
    ui->menu_File->addAction(&loadFieldAction);
    hghlghtr = new Highlighter(ui->schemeCode->document());
    scm_init_guile();
    connect(ui->schemeButton, &QPushButton::clicked, this, &MainWindow::evalScript);
    connect(ui->playButton, &QPushButton::clicked, canvas, &Canvas::resumeTimer);
    connect(ui->pauseButton, &QPushButton::clicked, canvas, &Canvas::pauseTimer);
    connect(&saveCodeAction, &QAction::triggered, this, &MainWindow::saveCode);
    connect(&loadCodeAction, &QAction::triggered, this, &MainWindow::loadCode);
    connect(&saveFieldAction, &QAction::triggered, this, &MainWindow::saveField);
    connect(&loadFieldAction, &QAction::triggered, this, &MainWindow::loadField);
    connect(ui->schemeCode, &QTextEdit::cursorPositionChanged, this, &MainWindow::parenHighlight);
    connect(ui->colorSelect, &QComboBox::currentTextChanged, this, &MainWindow::updateColor);
    connect(ui->clearButton, &QPushButton::clicked, canvas, &Canvas::clearCanvas);
    connect(ui->nextstepButton, &QPushButton::clicked, canvas, &Canvas::onTimerEvent);
    connect(ui->speedSlider, &QSlider::sliderReleased, this, &MainWindow::updateInterval);
    QString initialСode("(set! *random-state* (random-state-from-platform))   ; Random seed for PRNG\n"
                        "(define (cell-update itself neighbours)\n"
                        "  (random 2))\n");
    ui->schemeCode->setText(initialСode);
    monospaseFont.setStyleHint(QFont::Monospace);
    ui->schemeCode->setFont(monospaseFont);
    QImage iconBase = QImage(8, 8, QImage::Format_ARGB32);
    int cn = canvas->colorNumber();
    for(int i = 0; i < cn; i++){
        for(int x = 0; x < 8; x++){
            for(int y = 0; y < 8; y++){
                iconBase.setPixel(x, y, uint(canvas->getColor(i)));
            }
        }
        QPixmap pixmap = QPixmap::fromImage(iconBase);
        ui->colorSelect->addItem(QIcon(pixmap), QString::number(i), QVariant(i));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

SCM error_handler(void *, SCM key, SCM args)
{
    qDebug("Error in scheme code");
    return SCM_BOOL_F;
}


void MainWindow::evalScript()
{
    SCM res;
//    res = eval_script((void*)ui->schemeCode->toPlainText().toStdString().c_str());
    res = scm_c_catch(SCM_BOOL_T, eval_script, (void*)ui->schemeCode->toPlainText().toStdString().c_str(), error_handler, NULL, NULL, NULL);
    SCM f;
    f = scm_variable_ref(scm_c_lookup("cell-update"));
    canvas->setUpdateFunction(f);
}

SCM eval_script(void *script)
{
    char* script_str = (char*)script;
    return scm_c_eval_string(script_str);
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
    QString loadFileName = QFileDialog::getOpenFileName(this, "open scheme code file", QString(), "*.scm");
    QFile loadFile(loadFileName);
    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    ui->schemeCode->clear();
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
    disconnect(ui->pauseButton, &QPushButton::clicked, canvas, &Canvas::pauseTimer);
    disconnect(ui->clearButton, &QPushButton::clicked, canvas, &Canvas::clearCanvas);
    disconnect(ui->nextstepButton, &QPushButton::clicked, canvas, &Canvas::onTimerEvent);
    QJsonDocument canvasJson = QJsonDocument::fromJson(loadFile.readAll());
    canvas = new Canvas(this, canvasJson, getInterval());
    delete canvas1;
    ui->verticalLayout->insertWidget(0, canvas);
    connect(ui->playButton, &QPushButton::clicked, canvas, &Canvas::resumeTimer);
    connect(ui->pauseButton, &QPushButton::clicked, canvas, &Canvas::pauseTimer);
    connect(ui->clearButton, &QPushButton::clicked, canvas, &Canvas::clearCanvas);
    connect(ui->nextstepButton, &QPushButton::clicked, canvas, &Canvas::onTimerEvent);
    ui->colorSelect->clear();
    QImage iconBase = QImage(8, 8, QImage::Format_ARGB32);
    int cn = canvas->colorNumber();
    for(int i = 0; i < cn; i++){
        for(int x = 0; x < 8; x++){
            for(int y = 0; y < 8; y++){
                iconBase.setPixel(x, y, uint(canvas->getColor(i)));
            }
        }
        QPixmap pixmap = QPixmap::fromImage(iconBase);
        ui->colorSelect->addItem(QIcon(pixmap), QString::number(i), QVariant(i));
    }
}

void MainWindow::parenHighlight()
{
    hghlghtr->unhighlightParen(highlightedSymbol);
    QString code = ui->schemeCode->toPlainText();
    int curPos = ui->schemeCode->textCursor().position();
    QChar curChar = code[curPos];
    int endPos = code.length() - 1;
    int parenCounter = 1;
    if (curChar == '('){
        curPos++;
        while (curPos < endPos){
            if (code[curPos] == '(')
                parenCounter++;
            else if (code[curPos] == ')')
                parenCounter--;
            if (parenCounter == 0){
                highlightedSymbol = curPos;
                hghlghtr->highlightParen(highlightedSymbol);
                return;
            }
            curPos++;
        }
    } else if (curChar == ')'){
        curPos--;
        while (curPos >= 0){
            if (code[curPos] == ')')
                parenCounter++;
            else if (code[curPos] == '(')
                parenCounter--;
            if (parenCounter == 0){
                highlightedSymbol = curPos;
                hghlghtr->highlightParen(highlightedSymbol);
                return;
            }
            curPos--;
        }
    }
}

void MainWindow::updateColor()
{
    canvas->setColor(ui->colorSelect->currentData().toInt());
}

void MainWindow::updateInterval()
{
    canvas->setInterval(getInterval());
    ui->speedInfo->setText(QString().number(getInterval()));
}

int MainWindow::getInterval()
{
    if(ui->speedSlider->value() == 0){
        return 1000;
    } else if(ui->speedSlider->value() == 1){
        return 500;
    } else if(ui->speedSlider->value() == 2){
        return 333;
    } else if(ui->speedSlider->value() == 3){
        return 200;
    } else {
        return 100;
    }
}
