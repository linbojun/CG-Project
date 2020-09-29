#ifndef SPHERE_H
#define SPHERE_H
#include "ui/Settings.h"
#include "Shape.h"


class Sphere : public Shape{
public:
    Sphere();
    Sphere(int p1, int p2);
    ~Sphere();
private:
    void create_vertex_data();
    int side_size;
    int mesh_size;

};

#endif
