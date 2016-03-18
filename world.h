#ifndef WORLD_H
#define WORLD_H

#include "circle.h"
#include "spring.h"
#include "wpos.h"

class World
{
public:
    virtual ~World();

    static World* get_world();

    std::vector<Phys*> objects;
    std::vector<Spring*> springs;
    std::vector<Renderable*> renderables;

    void apply_gravity();
    void apply_springs();

    Phys* object_at(WPos);

    double time_speed = 10.0;
    Phys* cam_focus;
    double dt;

protected:
    World();

private:
    static World* the_world;

};

#endif // WORLD_H
