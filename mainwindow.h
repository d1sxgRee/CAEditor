#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSyntaxHighlighter>
#include "canvas.h"
#include "highlighter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
};

#endif // MAINWINDOW_H
