#include "Cone.h"
#include "ui/Settings.h"

Cone::Cone()
{
}

Cone::Cone(int p1, int p2):
    mesh_size(p1),
    side_size(p2)
{
    if(side_size < 2)
        side_size = 2;
    create_vertex_data();
    buildVAO();
}

Cone::~Cone()
{

}

void Cone::create_vertex_data()
{

}
