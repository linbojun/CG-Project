#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"


class Cube : public Shape
{
public:
    Cube();
    Cube(int param1);
    ~Cube();

private:
    void create_vertex_data();
    void create_facet(float center_x, float center_y, float center_z);
    int size;

};

#endif
