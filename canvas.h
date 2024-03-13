#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QJsonDocument>
#include <vector>
#include <libguile.h>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr, int n = 100);
    explicit Canvas(QWidget *parent, QJsonDocument fieldJson);
    void paintEvent(QPaintEvent*) override;
    QJsonDocument toJson();

public slots:
    void setUpdateFunction(SCM f);
    void resumeTimer();
    void pauseTimer();
    void clearCanvas();

private:
    int n;
    QTimer timer;
    QImage image;
    std::vector<std::vector<int>> world;
    std::vector<uint> colors;
    SCM cellUpdate;
    void drawImage();

signals:

private slots:
    void onTimerEvent();
};

#endif // CANVAS_H
