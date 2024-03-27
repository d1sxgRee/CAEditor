#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <libguile.h>
#include <QAction>
#include <QSyntaxHighlighter>
#include "canvas.h"
#include "highlighter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern "C" {
    SCM eval_script(void*);
    SCM error_handler(void*, SCM key, SCM args);
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void evalScript();
    void saveCode();
    void loadCode();
    void saveField();
    void loadField();
    void parenHighlight();
    void updateColor();
    void updateInterval();

private:
    Ui::MainWindow *ui;
    Canvas *canvas;
    Highlighter *hghlghtr;
    QAction saveCodeAction;
    QAction loadCodeAction;
    QAction saveFieldAction;
    QAction loadFieldAction;
    QFont monospaseFont;
    int highlightedSymbol;
    int getInterval();
};

#endif // MAINWINDOW_H
