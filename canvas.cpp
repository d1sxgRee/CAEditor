#include "canvas.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QMouseEvent>
#include <QPainter>

Canvas::Canvas(QWidget *parent, int n)
    : QWidget{parent}
    , image{QImage(n, n, QImage::Format_ARGB32)}
    , n(n)
    , activeColorIndex(0)
{
    connect(&timer, &QTimer::timeout, this, &Canvas::onTimerEvent);

    world.resize(n, std::vector<int>(n, 0));

    colors.push_back(0xffff8800);
    colors.push_back(0xffcc4400);

    drawImage();
    cellUpdate = SCM_UNDEFINED;
}

Canvas::Canvas(QWidget *parent, QJsonDocument fieldJson)
    : QWidget{parent}
    , activeColorIndex(0)
{
    QJsonObject object = fieldJson.object();
    n = object["n"].toInt();
    world.resize(n, std::vector<int>(n, 0));
    image = QImage(n, n, QImage::Format_ARGB32);
    connect(&timer, &QTimer::timeout, this, &Canvas::onTimerEvent);
    QJsonArray colorsArray = object["colors"].toArray();
    for (auto i : colorsArray){
        QString colorString = i.toString();
        colors.push_back(colorString.toUInt(nullptr, 16));
    }
    QJsonArray worldArray = object["world"].toArray();
    for (int i = 0; i < n*n; i++){
        world[i % n][i / n] = worldArray[i].toInt();
    }
    drawImage();
    cellUpdate = SCM_UNDEFINED;
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect rect(0, 0, 400, 400);

    painter.drawImage(rect, image);
}

int Canvas::colorNumber()
{
    return colors.size();
}

uint Canvas::getColor(int i)
{
    return colors[i];
}

QJsonDocument Canvas::toJson()
{
    QJsonObject canvasCondition = QJsonObject();
    canvasCondition.insert("n", n);
    QJsonArray jsonColors = QJsonArray();
    for(auto i : colors){
        QString colorString;
        colorString.setNum(i, 16);
        jsonColors.append(colorString);
    }
    canvasCondition.insert("colors", jsonColors);
    QJsonArray jsonWorld = QJsonArray();
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            jsonWorld.append(world[x][y]);
        }
    }
    canvasCondition.insert("world", jsonWorld);
    return QJsonDocument(canvasCondition);
}

void Canvas::setColor(int i)
{
    activeColorIndex = i;
}

void Canvas::setUpdateFunction(SCM f)
{
    cellUpdate = f;
}

void Canvas::resumeTimer()
{
    timer.start(300);
}

void Canvas::pauseTimer()
{
    timer.stop();
}

void Canvas::clearCanvas()
{
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            world[x][y] = 0;
        }
    }
    drawImage();
    repaint();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    QPoint point = logicalPoint(event->pos());
    world[point.x()][point.y()] = activeColorIndex;
    drawImage();
    repaint();
    return;
}

void Canvas::drawImage()
{
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            image.setPixel(x, y, colors[world[x][y]]);
        }
    }
}

QPoint Canvas::logicalPoint(QPoint gr)
{
    return QPoint(gr.x() / 4, gr.y() / 4);
}

void Canvas::onTimerEvent()
{
    int oldWorld[n][n];
    for(int y = 0; y < n; y++){
        for(int x = 0; x < n; x++){
            oldWorld[x][y] = world[x][y];
        }
    }
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
    drawImage();
    repaint();
}
