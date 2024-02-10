#include "canvas.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QPainter>
#include <QRandomGenerator>

Canvas::Canvas(QWidget *parent, int n)
    : QWidget{parent}
    , image{QImage(n, n, QImage::Format_ARGB32)}
    , n(n)
{
    connect(&timer, &QTimer::timeout, this, &Canvas::onTimerEvent);

    QRandomGenerator* rng = QRandomGenerator::system();

    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            world[y][x] = rng->bounded(2);
        }
    }
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
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

Canvas::Canvas(QWidget *parent, QJsonDocument fieldJson)
    : QWidget{parent}
{
    QJsonObject object = fieldJson.object();
    n = object["n"].toInt();
    image = QImage(n, n, QImage::Format_ARGB32);
    connect(&timer, &QTimer::timeout, this, &Canvas::onTimerEvent);
    QJsonArray worldArray = object["world"].toArray();
    for (int i = 0; i < n*n; i++){
        world[i % n][i / n] = worldArray[i].toInt();
    }
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
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

QJsonDocument Canvas::toJson()
{
    QJsonObject canvasCondition = QJsonObject();
    canvasCondition.insert("n", n);
    QJsonArray jsonWorld = QJsonArray();
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            jsonWorld.append(world[x][y]);
        }
    }
    canvasCondition.insert("world", jsonWorld);
    return QJsonDocument(canvasCondition);
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
    int oldWorld[n][n];
    std::copy(&world[0][0], &world[0][0]+n*n, &oldWorld[0][0]);
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            SCM neighbours = scm_list_n(scm_from_int64(oldWorld[(x - 1 + n) % n][(y - 1 + n) %  n]),
                                        scm_from_int64(oldWorld[(x + n) % n][(y - 1 + n) %  n]),
                                        scm_from_int64(oldWorld[(x + 1 + n) % n][(y - 1 + n) %  n]),
                                        scm_from_int64(oldWorld[(x - 1 + n) % n][(y + n) %  n]),
                                        scm_from_int64(oldWorld[(x + 1 + n) % n][(y + n) %  n]),
                                        scm_from_int64(oldWorld[(x - 1 + n) % n][(y + 1 + n) %  n]),
                                        scm_from_int64(oldWorld[(x + n) % n][(y + 1 + n) %  n]),
                                        scm_from_int64(oldWorld[(x + 1 + n) % n][(y + 1 + n) %  n]),
                                        SCM_UNDEFINED);
            world[x][y] = scm_to_int64(scm_call_2(cellUpdate, scm_from_int64(oldWorld[x][y]), neighbours));
        }
    }
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
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
