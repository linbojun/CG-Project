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
    std::vector<GLfloat> token;
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
            token.push_back(x);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y-space);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x-space);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());


            //second triangle
            token.push_back(x);
            token.push_back(y-space);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x-space);
            token.push_back(y-space);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x-space);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

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
            token.push_back(x);
             token.push_back(y);
             token.push_back(z);
             token.insert(token.end(), normal.begin(), normal.end());

             token.push_back(x);
             token.push_back(y-space);
             token.push_back(z);
             token.insert(token.end(), normal.begin(), normal.end());

             token.push_back(x+space);
             token.push_back(y);
             token.push_back(z);
             token.insert(token.end(), normal.begin(), normal.end());

              //second triangle
             token.push_back(x);
             token.push_back(y-space);
             token.push_back(z);
             token.insert(token.end(), normal.begin(), normal.end());

             token.push_back(x+space);
             token.push_back(y-space);
             token.push_back(z);
             token.insert(token.end(), normal.begin(), normal.end());


              token.push_back(x+space);
              token.push_back(y);
              token.push_back(z);
              token.insert(token.end(), normal.begin(), normal.end());

        }
    }
    //above good!!!!


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
            token.push_back(x);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y);
            token.push_back(z+space);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x+space);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            //second triangle
            token.push_back(x);
            token.push_back(y);
            token.push_back(z+space);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x+space);
            token.push_back(y);
            token.push_back(z+space);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x+space);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());


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
            token.push_back(x);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x - space);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y);
            token.push_back(z-space);
            token.insert(token.end(), normal.begin(), normal.end());

            //second triangle
            token.push_back(x - space);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x-space);
            token.push_back(y);
            token.push_back(z-space);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y);
            token.push_back(z-space);
            token.insert(token.end(), normal.begin(), normal.end());
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
            token.push_back(x);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y-space);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y);
            token.push_back(z - space);
            token.insert(token.end(), normal.begin(), normal.end());

            //second triangle
            token.push_back(x);
            token.push_back(y-space);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y-space);
            token.push_back(z-space);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y);
            token.push_back(z - space);
            token.insert(token.end(), normal.begin(), normal.end());

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
            token.push_back(x);
            token.push_back(y);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y-space);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y);
            token.push_back(z+space);
            token.insert(token.end(), normal.begin(), normal.end());

            //second triangle
            token.push_back(x);
            token.push_back(y-space);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());


            token.push_back(x);
            token.push_back(y-space);
            token.push_back(z+space);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(x);
            token.push_back(y);
            token.push_back(z+space);
            token.insert(token.end(), normal.begin(), normal.end());


        }

    }

    //std::vector<GLfloat> m_vertexData
    m_vertexData = token;
}
