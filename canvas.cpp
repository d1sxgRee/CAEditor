#include "canvas.h"
#include <QPainter>
#include <QRandomGenerator>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}, image{QImage(100, 100, QImage::Format_ARGB32)}
{
    connect(&timer, &QTimer::timeout, this, &Canvas::onTimerEvent);

    QRandomGenerator* rng = QRandomGenerator::system();
    for(int y = 0; y < 100; y++){
        for(int x = 0; x < 100; x++){
            world[y][x] = rng->bounded(4);
        }
    }
    for(int y = 0; y < 100; y++){
        for(int x = 0; x < 100; x++){
            switch (world[y][x]) {
            case 0:
                image.setPixel(x, y, 0xffff8800);
                break;
            case 1:
                image.setPixel(x, y, 0xffcc4400);
                break;
            case 2:
                image.setPixel(x, y, 0xff880000);
                break;
            default:
                image.setPixel(x, y, 0xff660033);
                break;
            }
        }
    }
    cellUpdate = SCM_UNDEFINED;
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect rect(0, 0, 400, 400);

    painter.drawImage(rect, image);
}

void Canvas::setUpdateFunction(SCM f)
{
    cellUpdate = f;
}

void Canvas::resumeTimer()
{
    timer.start(1000);
}

void Canvas::onTimerEvent()
{
    int oldWorld[100][100];
    std::copy(&world[0][0], &world[0][0]+10000, &oldWorld[0][0]);
    for(int y = 0; y < 100; y++){
        for(int x = 0; x < 100; x++){
            SCM neighbours = scm_list_n(scm_from_int64(oldWorld[(x - 1 + 100) % 100][(y - 1 + 100) %  100]),
                                        scm_from_int64(oldWorld[(x + 100) % 100][(y - 1 + 100) %  100]),
                                        scm_from_int64(oldWorld[(x + 1 + 100) % 100][(y - 1 + 100) %  100]),
                                        scm_from_int64(oldWorld[(x - 1 + 100) % 100][(y + 100) %  100]),
                                        scm_from_int64(oldWorld[(x + 1 + 100) % 100][(y + 100) %  100]),
                                        scm_from_int64(oldWorld[(x - 1 + 100) % 100][(y + 1 + 100) %  100]),
                                        scm_from_int64(oldWorld[(x + 100) % 100][(y + 1 + 100) %  100]),
                                        scm_from_int64(oldWorld[(x + 1 + 100) % 100][(y + 1 + 100) %  100]),
                                        SCM_UNDEFINED);
            world[x][y] = scm_to_int64(scm_call_2(cellUpdate, scm_from_int64(oldWorld[x][y]), neighbours));
        }
    }
    for(int y = 0; y < 100; y++){
        for(int x = 0; x < 100; x++){
            switch (world[y][x]) {
            case 0:
                image.setPixel(x, y, 0xffff8800);
                break;
            case 1:
                image.setPixel(x, y, 0xffcc4400);
                break;
            case 2:
                image.setPixel(x, y, 0xff880000);
                break;
            default:
                image.setPixel(x, y, 0xff660033);
                break;
            }
        }
    }
    repaint();
}
