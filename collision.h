#ifndef COLLISION_H
#define COLLISION_H

#include "constants.h"

class Phys;
class Circle;
class Polygon;

namespace collision {
    bool intersecting (Phys&, Phys&);
    bool colliding (Phys&, Phys&);
    void collide(Phys&, Phys&, double);

    bool cc_intersecting (Circle&, Circle&);
    bool cp_intersecting (Circle&, Polygon&);
    bool pp_intersecting (Polygon&, Polygon&);

    bool cc_colliding (Circle&, Circle&);
    bool cp_colliding (Circle&, Polygon&);
    bool pp_colliding (Polygon&, Polygon&);

    void cc_collide(Circle&, Circle&, double);
    void cp_collide(Circle&, Polygon&, double);
    void pp_collide(Polygon&, Polygon&, double);
}

#endif // COLLISION_H
