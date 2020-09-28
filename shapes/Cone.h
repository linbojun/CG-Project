#ifndef CONE_H
#define CONE_H

#include "Shape.h"
class Cone: public Shape
{
public :
    Cone();
    Cone(int p1, int p2);
    ~Cone();
private:
    void create_vertex_data();
    int side_size;
    int mesh_size;
};








#endif
