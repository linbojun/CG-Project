#include "Cube.h"
#include "ui/Settings.h"

Cube::Cube()
{
}

Cube::Cube(int param1_):
    size(param1_)
{

    shape_type = SHAPE_CONE;
    create_vertex_data();
    buildVAO();
}

Cube::~Cube()
{

}

void Cube::create_vertex_data()
{
    float space = 1/(float)size;
    //add front face which is parallel to xy plane, z is constant = -0.5
    float start_x = 0.5f, start_y = 0.5f, start_z = -0.5f;
    std::vector<float> normal = {0, 0, -1};
    for(int i = 0; i < size; i++)
    {
        float y = start_y - i * space;
        float z = start_z;
        for(int j = 0; j < size; j++)
        {

            float x = start_x - j * space;
            //first triangle
            update_m_vtx(x, y, z, normal);

            update_m_vtx(x, y-space, z, normal);

           update_m_vtx(x-space, y, z, normal);


            //second triangle
           update_m_vtx(x, y-space, z, normal);

            update_m_vtx(x-space, y-space, z, normal);

            update_m_vtx(x-space, y, z, normal);


        }

    }

    // add the face paralleled to xy plane, z is constant =0.5
    start_x = -0.5f, start_y =0.5f, start_z = 0.5f;
    normal = {0, 0, 1};
    for(int i = 0; i < size; i++)
    {
        float y = start_y - i * space;
        float z = start_z;
        for(int j = 0; j < size; j++)
        {
             float x = start_x+ j * space;
            //first triangle
             update_m_vtx(x, y, z, normal);

              update_m_vtx(x, y-space, z, normal);

              update_m_vtx(x+space, y, z, normal);

              //second triangle
             update_m_vtx(x, y-space, z, normal);

             update_m_vtx(x+space, y - space, z, normal);

               update_m_vtx(x+space, y, z, normal);
        }
    }

    // add the face paralleled to xz plane, y is constant =0.5
    start_x = -0.5f, start_y = 0.5f, start_z = -0.5f;
    normal = {0, 1, 0};
    for(int i = 0; i < size; i++)
    {
        float y = start_y;
        float z = start_z + i* space;
        for(int j = 0; j < size; j++)
        {

            float x = start_x+ j * space;
            //first triangle
            update_m_vtx(x, y, z, normal);

            update_m_vtx(x, y, z+space, normal);

            update_m_vtx(x+space, y, z, normal);

            //second triangle
             update_m_vtx(x, y, z+space, normal);

             update_m_vtx(x+space, y, z+space, normal);

             update_m_vtx(x+space, y, z, normal);

        }

    }

    //above good
    // add the face paralleled to xz plane, y is constant =-0.5
    start_x = 0.5f, start_y = -0.5f, start_z = 0.5f;
    normal = {0, -1, 0};
    for(int i = 0; i < size; i++)
    {
        float y = start_y;
        float x = start_x - i * space;

        for(int j = 0; j < size; j++)
        {

            float z = start_z - j* space;
            //first triangle
             update_m_vtx(x, y, z, normal);

            update_m_vtx(x-space, y, z, normal);

            update_m_vtx(x, y, z-space, normal);

            //second triangle
            update_m_vtx(x-space, y, z, normal);
            update_m_vtx(x-space, y, z-space, normal);
            update_m_vtx(x, y, z-space, normal);

        }

    }

    // add the face paralleled to yz plane, x is constant =0.5
    start_x = 0.5f, start_y = 0.5f, start_z = 0.5f;
    normal = {1, 0, 0};
    for(int i = 0; i < size; i++)
    {
        float x = start_x;
        float y = start_y - i* space;
        for(int j = 0; j < size; j++)
        {

            float z = start_z - j * space;
            //first triangle
            update_m_vtx(x, y, z, normal);

            update_m_vtx(x, y-space, z, normal);

            update_m_vtx(x, y, z-space, normal);

            //second triangle
            update_m_vtx(x, y-space, z, normal);

            update_m_vtx(x, y-space, z-space, normal);

             update_m_vtx(x, y, z-space, normal);

        }

    }

    // add the face paralleled to yz plane, x is constant =-0.5
    start_x = -0.5f, start_y = 0.5f, start_z = -0.5f;
    normal = {-1, 0, 0};
    for(int i = 0; i < size; i++)
    {
        float x = start_x;
        float y = start_y - i* space;
        for(int j = 0; j < size; j++)
        {

            float z = start_z+ j * space;
            //first triangle
            update_m_vtx(x, y, z, normal);

             update_m_vtx(x, y-space, z, normal);

            update_m_vtx(x, y, z+space, normal);

            //second triangle
            update_m_vtx(x, y-space, z, normal);
            update_m_vtx(x, y-space, z+space, normal);

           update_m_vtx(x, y, z+space, normal);


        }

    }

}
