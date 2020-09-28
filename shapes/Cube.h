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
    int size;

};

#endif
