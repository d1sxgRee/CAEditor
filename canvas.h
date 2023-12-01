#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <libguile.h>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*) override;

public slots:
    void setUpdateFunction(SCM f);
    void resumeTimer();

private:
    QTimer timer;
    QImage image;
    int world[100][100];
    SCM cellUpdate;

signals:

private slots:
    void onTimerEvent();
};

#endif // CANVAS_H
