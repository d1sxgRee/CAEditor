#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QTimer>
#include <QImage>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*) override;

private:
    QTimer timer;
    QImage image;
    int world[100][100];

signals:

private slots:
    void onTimerEvent();
};

#endif // CANVAS_H
