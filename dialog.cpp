#include "dialog.h"
#include "ui_dialog.h"

#define WIDTH 1300
#define HEIGHT 1000

Dialog::Dialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog),
    cam(Camera(WIDTH, HEIGHT)),
    world()
{
    this->setMouseTracking(true);

    ui->setupUi(this);
    this->resize(WIDTH, HEIGHT);
    this->setStyleSheet("background-color: #111111;");

    cam.focus = nullptr;
    cam.zoom = 0.75;

    QTimer *frameTimer = new QTimer(this);
    connect(frameTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    frameTimer->start(17);

    QTimer *physTimer = new QTimer(this);
    connect(physTimer, SIGNAL(timeout()), this, SLOT(physFrame()));
    physTimer->start(10);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::nextFrame()
{
    // Runs paintEvent
    update();
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    cam.renderScene(painter, world.renderables);

    std::string camPos("Camera Position: " + std::string(cam.pos));
    painter.drawText(20, 20, camPos.c_str());

    Phys* mObject = world.objectAt(cam.mouseWorldCoords().wpos());
    std::string objName("Mouse on object: ");
    if (mObject) {
        objName += mObject->name;
    } else {
        objName += "-";
    }
    painter.drawText(20, 40, objName.c_str());

    std::string ts("Time Speed: " + std::to_string(world.timeSpeed));
    painter.drawText(20, 60, ts.c_str());
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    cam.mousePos = Vec(event->x(), event->y());

    if (cam.grabbed) {
        if (cam.held_object) {
            cam.held_object->pos = cam.pos_when_grabbed + (cam.mouseWorldCoords() - cam.grab_coords);
        }
        else {
            cam.pos = cam.pos_when_grabbed + (cam.convertToWorldCoords(cam.grab_coords) - cam.mouseWorldCoords());
        }
    }
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    cam.mousePos = Vec(event->x(), event->y());

    if (event->button() == Qt::LeftButton)
    {
        cam.grabbed = true;

        Phys* mObject = world.objectAt(cam.mouseWorldCoords().wpos());

        if (mObject) {
            cam.pos_when_grabbed = mObject->pos;
            cam.grab_coords = cam.mouseWorldCoords();
            cam.held_object = mObject;
        }
        else {
            cam.pos_when_grabbed = cam.pos;
            cam.grab_coords = Vec(event->x(), event->y());
        }
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    cam.mousePos = Vec(event->x(), event->y());

    if (event->button() == Qt::LeftButton)
    {
        cam.grabbed = false;

        if (cam.held_object) {
            cam.held_object = nullptr;
        }

    }
}

void Dialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    cam.mousePos = Vec(event->x(), event->y());

    if (event->button() == Qt::LeftButton)
    {
        Vec clickCoord = cam.mouseWorldCoords();
        cam.focus = world.objectAt(clickCoord.wpos());
    }
}

void Dialog::wheelEvent(QWheelEvent *event)
{
    cam.mousePos = Vec(event->x(), event->y());

    int delta = event->delta();

    if (delta > 0) cam.zoom += delta*cam.zoom/1440;
    else cam.zoom += delta*cam.zoom/2880;

    // Magic numbers here because the delta is supplied in 1/8ths of a degree of scroll
    // and we want to go that fraction of the distance to doubling or halving the zoom level.
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Equal:
        case Qt::Key_Plus:
            world.timeSpeed += 0.1;
            break;
        case Qt::Key_Minus:
            world.timeSpeed -= 0.1;
            break;
    }

    if (world.timeSpeed < 0) world.timeSpeed = 0;
    else if (world.timeSpeed > 5) world.timeSpeed = 5;
}



void Dialog::physFrame()
{
    world.dt = clock.delta()*world.timeSpeed;

    world.applySprings();
    world.applyGravity();


    for (auto c = world.objects.begin();
         c != world.objects.end();
         ++c)
    {
        (*c)->integrate(world.dt);
    }

    cam.mouseVel = cam.mouseWorldCoords() - lastMousePos;

    if (cam.held_object)
    {
        cam.held_object->vel = cam.mouseVel*(1/world.dt);
        cam.held_object->pos = cam.pos_when_grabbed + (cam.mouseWorldCoords() - cam.grab_coords);
    }

    lastMousePos = cam.mouseWorldCoords();

    // Run collisions
    for (size_t i = 0; i < world.objects.size(); ++i) {
        for (size_t j = i+1; j < world.objects.size(); ++j) {
            world.objects[i]->collide(*world.objects[j], world.dt);

            //if (world.objects[i]->colliding(*world.objects[j])) {
                //WPos p = world.objects[i]->collisionPoint(*world.objects[j]);
                //Circle* c = new Circle(p, 5);
                //world.renderables.push_back(c);
            //}
        }
    }

}

