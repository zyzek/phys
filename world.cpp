/* World
 * World uses the Singleton creational pattern.
 * To obtain the current world, get_world() must be called.
 *
 * The world object holds the various physically-significant objects,
 * and the list of things to be rendered, which may or may not be physical.
 *
 * Additionally the world also handles applying gravitation and spring forces,
 * and determines which objects to check collisions between.
 */

#include "world.h"

#include <iostream>

#include "cosmogony.h"
#include "circle.h"
#include "polygon.h"
#include "line.h"
#include "collision.h"

#define G (6.67408e-11)

#define CONF_FILE "universe.txt"

World::~World() {
    for (Spring *s : springs) {
        delete s;
    }

    // For now, all objects are added to the render queue, so
    // deleting the latter deletes the former.
    for (Renderable *r : renderables) {
        delete r;
    }
}

World* World::the_world = 0;

World* World::get_world() {
    if (0 == the_world) the_world = new World();
    return the_world;
}

World::World()
{
    /*
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
    //Line* line = new Line(Vec(0,0), Vec(400,400));
    //line->strokeColor = QColor(Qt::white);
    */

    ifstream in_file(CONF_FILE);

    if (in_file.is_open())
    {
        vector<string> *tokens = tokenise(in_file);
        vector<Tag> *tags = parse_tags(*tokens);

        int i = 0;
        try {
            while (i < tags->size()) {
                Circle *c = parse_circle(*tags, i);
                std::cout << c->name << std::endl;
                objects.push_back(c);
                renderables.push_back(c);
            }
        }
        catch (string s) {
            std::cout << s << std::endl;
        }


        delete tokens;
        delete tags;
        in_file.close();
    }
    else
    {
        cout << "Error opening file: " << CONF_FILE << endl;
    }
}

void World::apply_gravity()
{
    for (auto c = objects.begin(); c != objects.end(); ++c) {
        for (auto d = c + 1; d != objects.end(); ++d) {
            Vec cd = (*d)->pos - (*c)->pos;
            Vec grav = cd.unit()*(G*((*c)->mass)*((*d)->mass)/pow(cd.length(), 2));
            (*c)->apply_force(grav, Vec(0,0));
            (*d)->apply_force(grav*-1, Vec(0, 0));
        }
    }
}

void World::apply_springs()
{
    for (Spring *s : springs)
    {
        s->apply();
    }
}

// Input vector is in world coordinates.
Phys* World::object_at(WPos coord) {
    for (Phys* o : objects)
    {
        if (o->is_internal( o->world_to_ego(coord).wpos() )) return o;
    }

    return nullptr;
}

