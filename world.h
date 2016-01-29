#ifndef WORLD_H
#define WORLD_H

#include "phys.h"
#include "spring.h"
#include "wpos.h"

class World
{
public:
    World();

    std::vector<Phys*> objects;
    std::vector<Spring*> springs;
    std::vector<Renderable*> renderables;

    void applyGravity();
    void applySprings();

    Phys* objectAt(WPos);

    double timeSpeed;
    Phys* camFocus;
    double dt;

};

#endif // WORLD_H
