#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <libguile.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalLayout->insertWidget(0, &canvas);
    scm_init_guile();
    scm_c_eval_string("(display \"Hello, Scheme world!\")");
    connect(ui->schemeButton, &QPushButton::clicked, this, &MainWindow::evalScript);
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
    rvalue = scm_to_utf8_stringn(res, NULL);
    qDebug(rvalue);
}
