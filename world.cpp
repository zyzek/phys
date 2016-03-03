#include "world.h"

#include "circle.h"
#include "polygon.h"
#include "staticpoly.h"
#include "line.h"

World::World():
    timeSpeed(1.0)
{
    Circle* sun = new Circle(Vec(0,0), 40.0, 4.0);
    sun->name = "Sol";

    Circle* mercury = new Circle(Vec(-100, 0), 5.0);
    mercury->vel = Vec(0, 150);
    mercury->fillColor = QColor("#e60000");
    mercury->name = "Mercury";

    Circle* earth = new Circle(Vec(200, 0), 10.0, 1.5);
    earth->vel = Vec(0, -150);
    earth->fillColor = QColor("#1ac6ff");
    earth->name = "Earth";

    Circle* moon = new Circle(Vec(225, 0), 4, 0.4);
    moon->vel = Vec(0, -115);
    moon->fillColor = QColor("#bfbfbf");
    moon->name = "The Moon";

    Spring* em = new Spring(earth, Vec(0,0), moon, Vec(0,0), 15, 50);
    em->strokeColor = Qt::white;
    springs.push_back(em);
    renderables.push_back(em);

    std::vector<WPos> points({WPos(0,0), WPos(60,-10), WPos(85, 15), WPos(85, 115), WPos(80, 125), WPos(60, 135), WPos(10, 120), WPos(0, 110)});
    for (WPos &p : points)
    {
        p.x /= 6.0;
        p.y /= 6.0;
    }

    Polygon* tardis = new Polygon(Vec(-300, 0), points);
    tardis->name = "TARDIS";
    tardis->vel = Vec(0, 65);
    tardis->strokeColor = QColor("#777777");
    tardis->fillColor = QColor("#111188");

    Circle* jupiter = new Circle(Vec(0, 400), 15);
    jupiter->vel = Vec(-170, 0);
    jupiter->fillColor = QColor("#ff8c1a");
    jupiter->name = "Jupiter";

    Circle* europa = new Circle(Vec(0, 425), 3, 0.3);
    europa->vel = Vec(-140, 0);
    europa->fillColor = QColor("#ccccff");
    europa->name = "Europa";

    Circle* callisto = new Circle(Vec(0, 360), 3, 0.3);
    callisto->vel = Vec(-150, 0);
    callisto->fillColor = QColor("#996633");
    callisto->name = "Callisto";

    Circle* pluto = new Circle(Vec(0.0, -600.0), 3);
    pluto->vel = Vec(120, 0);
    pluto->fillColor = QColor("#e5e5ff");
    pluto->name = "Pluto";

    //Line* line = new Line(Vec(0,0), Vec(400,400));
    //line->strokeColor = QColor(Qt::white);

    objects.push_back(sun);
    objects.push_back(mercury);
    objects.push_back(earth);
    objects.push_back(moon);
    //objects.push_back(tardis);
    objects.push_back(jupiter);
    objects.push_back(europa);
    objects.push_back(callisto);
    objects.push_back(pluto);

    renderables.push_back(sun);
    renderables.push_back(mercury);
    renderables.push_back(earth);
    renderables.push_back(moon);
    renderables.push_back(tardis);
    renderables.push_back(jupiter);
    renderables.push_back(europa);
    renderables.push_back(callisto);
    renderables.push_back(pluto);
    //renderables.push_back(line);
}

void World::applyGravity()
{
    for (auto c = objects.begin();
         c != objects.end();
         ++c)
    {
        for (auto d = c + 1;
             d != objects.end();
             ++d)
        {
            Vec cd = (*d)->pos - (*c)->pos;
            Vec grav = cd.unit()*(1.0*((*c)->mass)*((*d)->mass)/cd.length());
            (*c)->applyForce(grav, Vec(0,0));
            (*d)->applyForce(grav*-1, Vec(0, 0));
        }
    }
}

void World::applySprings()
{
    for (Spring *s : springs)
    {
        s->apply();
    }
}

// Input vector is in world coordinates.
Phys* World::objectAt(WPos coord) {
    for (Phys* o : objects)
    {
        if (o->isInternal( o->worldToEgo(coord).wpos() )) return o;
    }

    return nullptr;
}

