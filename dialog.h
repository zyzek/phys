#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QTimer>
#include <vector>

#include "phys.h"
#include "spring.h"
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

private:
    void applyGravity();
    void applySprings();

    Ui::Dialog *ui;
    std::vector<Phys*> objects;
    std::vector<Spring*> springs;
    std::vector<Renderable*> renderables;
    Camera cam;
    Clock clock;
    double dt;
    double timeSpeed;
    Phys* camFocus;
    Vec mousePos;

    Polygon tardis;

};

#endif // DIALOG_H
