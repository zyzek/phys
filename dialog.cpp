#include "dialog.h"
#include "ui_dialog.h"

#define WIDTH 1300
#define HEIGHT 1000

#define SECONDS_PER_DAY (86400)

Dialog::Dialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog),
    cam(Camera(WIDTH, HEIGHT)),
    world(World::get_world())
{
    this->setMouseTracking(true);

    ui->setupUi(this);
    this->resize(WIDTH, HEIGHT);
    this->setStyleSheet("background-color: #111111;");

    std::vector<Vec> planet_positions;

    // Set up camera focus and zoom parameters.
    cam.focus = nullptr;
    cam.zoom = 0.75;
    if (world->objects.size()) {
        double smallest_planet = world->objects[0]->radius;
        for (size_t i = 0; i < world->objects.size(); ++i) {
            if (world->objects[i]->radius < smallest_planet) {
                smallest_planet = world->objects[i]->radius;
                planet_positions.push_back(world->objects[i]->pos);
            }
        }
        cam.min_scale_log = log(smallest_planet);
        cam.zoom_rescale = 1.0;
    }
    cam.encompass_points(planet_positions);

    // Set up distinct timers for simulation and drawing.
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

    cam.move_to_focus();
    cam.render_scene(painter, world->renderables);

    painter.setPen(QColor("#ffffff"));

    std::string camPos("Camera Position: " + std::string(cam.pos));
    painter.drawText(20, 20, camPos.c_str());

    Phys* mObject = world->object_at(cam.mouse_world_coords().wpos());
    std::string objName("Mouse on object: ");
    if (mObject) {
        objName += mObject->name;
    } else {
        objName += "-";
    }
    painter.drawText(20, 40, objName.c_str());

    std::string ts("Days per second: " + std::to_string(world->time_speed));
    painter.drawText(20, 60, ts.c_str());
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    cam.mouse_pos = Vec(event->x(), event->y());

    if (cam.grabbed) {
        if (cam.held_object) {
            cam.held_object->pos = cam.pos_when_grabbed + (cam.mouse_world_coords() - cam.grab_coords);
        }
        else {
            cam.pos = cam.pos_when_grabbed + (cam.to_world_space(cam.grab_coords) - cam.mouse_world_coords());
        }
    }
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    cam.mouse_pos = Vec(event->x(), event->y());

    if (event->button() == Qt::LeftButton)
    {
        cam.grabbed = true;

        Phys* mObject = world->object_at(cam.mouse_world_coords().wpos());

        if (mObject) {
            cam.pos_when_grabbed = mObject->pos;
            cam.grab_coords = cam.mouse_world_coords();
            cam.held_object = mObject;
        }
        else {
            cam.pos_when_grabbed = cam.pos;
            cam.grab_coords = Vec(event->x(), event->y());
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        Vec clickCoord = cam.mouse_world_coords();
        cam.focus = world->object_at(clickCoord.wpos());
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    cam.mouse_pos = Vec(event->x(), event->y());

    if (event->button() == Qt::LeftButton)
    {
        cam.grabbed = false;

        if (cam.held_object) {
            cam.held_object = nullptr;
        }

    }
}

void Dialog::wheelEvent(QWheelEvent *event)
{
    cam.mouse_pos = Vec(event->x(), event->y());

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
            world->time_speed += 1.0;
            break;
        case Qt::Key_Minus:
            world->time_speed -= 1.0;
            break;
        case Qt::Key_Plus:
            world->time_speed += 10.0;
            break;
        case Qt::Key_Underscore:
            world->time_speed -= 10.0;
            break;
    }

    if (world->time_speed < 0) world->time_speed = 0;
    else if (world->time_speed > 1000) world->time_speed = 1000;
}



void Dialog::physFrame()
{
    world->dt = clock.delta()*world->time_speed*SECONDS_PER_DAY;

    world->apply_springs();
    world->apply_gravity();


    for (auto c = world->objects.begin();
         c != world->objects.end();
         ++c)
    {
        (*c)->integrate(world->dt);
    }

    cam.mouse_vel = cam.mouse_world_coords() - last_mouse_pos;

    if (cam.held_object)
    {
        if (world->dt) {
            cam.held_object->vel = cam.mouse_vel*(1/world->dt);
        }
        cam.held_object->pos = cam.pos_when_grabbed + (cam.mouse_world_coords() - cam.grab_coords);
    }

    last_mouse_pos = cam.mouse_world_coords();

    // Run collisions
    for (size_t i = 0; i < world->objects.size(); ++i) {
        for (size_t j = i+1; j < world->objects.size(); ++j) {
            collision::collide(*world->objects[i], *world->objects[j], world->dt);
        }
    }

}

