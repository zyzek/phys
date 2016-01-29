#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

#include <vector>


#include "world.h"
#include "camera.h"
#include "clock.h"

#include "polygon.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();


public slots:
    void nextFrame();
    void physFrame();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent *);

    Ui::Dialog *ui;

private:
    Camera cam;
    World world;
    Clock clock;

    Vec lastMousePos = Vec(0, 0);
};

#endif // DIALOG_H
