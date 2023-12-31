#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <canvas.h>

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

private:
    Ui::MainWindow *ui;
    Canvas *canvas;
    QAction saveCodeAction;
    QAction loadCodeAction;
    QAction saveFieldAction;
};
#endif // MAINWINDOW_H
