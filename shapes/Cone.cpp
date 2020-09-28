#include "Cone.h"
#include "ui/Settings.h"

Cone::Cone(){

}

Cone::Cone(int p1, int p2):
    mesh_size(p1),
    side_size(p2)
{
    shape_type = SHAPE_CONE;
}
