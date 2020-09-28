#include "Cylinder.h"
#include "ui/Settings.h"
#include <math.h>
#include <iostream>

Cylinder::Cylinder(){

}

Cylinder::Cylinder(int p1, int p2):
    mesh_size(p1),
    side_size(p2)
{
    shape_type = SHAPE_CYLINDER;
    if(p2 < 3)
        side_size = 3;
    create_vertex_data();
    buildVAO();

}

Cylinder::~Cylinder(){
}

void Cylinder::create_vertex_data(){
    std::vector<GLfloat> token;

    //top
    //side_size = 4;
    float side_space = (2*M_PI)/(float)side_size;
    std::cout<< "side_space: "<<side_space<< std::endl;
    float x = 0.5, y = 0, z = 0.5;
    std::vector<float> normal = {0, 1, 0};
    for(int i = 1; i <= side_size; i++)
    {
        float old_x = x;
        float old_y = y;
        float old_x_spc = old_x/(float)mesh_size;
        float old_y_spc = old_y/(float)mesh_size;

        token.push_back(0);
        token.push_back(0);
        token.push_back(z);

        token.insert(token.end(), normal.begin(), normal.end());

        token.push_back(old_x_spc);
        token.push_back(old_y_spc);
        token.push_back(z);
         token.insert(token.end(), normal.begin(), normal.end());

      //  std::cout<<"x's val:"<<x<<"y.val:"<<y<<"z.val:"<<z<<std::endl;
        x = cos(i * side_space) * 0.5;
        y = sin(i * side_space) * 0.5;
        float new_x_spc = x/(float)mesh_size;
        float new_y_spc = y/(float)mesh_size;
        token.push_back(new_x_spc);
        token.push_back(new_y_spc);
        token.push_back(z);
        token.insert(token.end(), normal.begin(), normal.end());

        for(int j = 1; j < mesh_size; j++)
        {

            //first triangle
            token.push_back(j * old_x_spc);
            token.push_back(j * old_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back((j+1) * old_x_spc);
            token.push_back((j+1) * old_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back((j+1) * new_x_spc);
            token.push_back((j+1) * new_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            //second triangle
            token.push_back(j * old_x_spc);
            token.push_back(j * old_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back((j+1) * new_x_spc);
            token.push_back((j+1) * new_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(j * new_x_spc);
            token.push_back(j * new_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

        }

    }



    x = 0.5, y = 0, z = -0.5;
    normal = {0, -1, 0};
    for(int i = 1; i <= side_size; i++)
    {
        float old_x = x;
        float old_y = y;
        float old_x_spc = old_x/(float)mesh_size;
        float old_y_spc = old_y/(float)mesh_size;

        token.push_back(old_x_spc);
        token.push_back(old_y_spc);
        token.push_back(z);
         token.insert(token.end(), normal.begin(), normal.end());

        token.push_back(0);
        token.push_back(0);
        token.push_back(z);
        token.insert(token.end(), normal.begin(), normal.end());

      //  std::cout<<"x's val:"<<x<<"y.val:"<<y<<"z.val:"<<z<<std::endl;
        x = cos(i * side_space) * 0.5;
        y = sin(i * side_space) * 0.5;
        float new_x_spc = x/(float)mesh_size;
        float new_y_spc = y/(float)mesh_size;
        token.push_back(new_x_spc);
        token.push_back(new_y_spc);
        token.push_back(z);
        token.insert(token.end(), normal.begin(), normal.end());


        for(int j = 1; j < mesh_size; j++)
        {

            //first triangle
            token.push_back(j * old_x_spc);
            token.push_back(j * old_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back((j+1) * new_x_spc);
            token.push_back((j+1) * new_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back((j+1) * old_x_spc);
            token.push_back((j+1) * old_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            //second triangle
            token.push_back(j * old_x_spc);
            token.push_back(j * old_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back(j * new_x_spc);
            token.push_back(j * new_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

            token.push_back((j+1) * new_x_spc);
            token.push_back((j+1) * new_y_spc);
            token.push_back(z);
            token.insert(token.end(), normal.begin(), normal.end());

        }

    }
     x = 0.5, y = 0;
    float barrel_mesh_size = 1/(float)mesh_size;
    for(int i = 1; i <= side_size; i++)
    {
        float old_x = x;
        float old_y = y;
        x = cos(i * side_space) * 0.5;
        y = sin(i * side_space) * 0.5;
        z = 0.5;
        glm:: vec3 normal_ = {(old_x+x)/2, (old_y+y)/2, 0};
        normal_ = glm::normalize(normal_);
        for(int j = 0; j < mesh_size; j++)
        {
            token.push_back(old_x);
            token.push_back(old_y);
            token.push_back(z - (j * barrel_mesh_size));
            token.push_back(normal_[0]);
            token.push_back(normal_[1]);
            token.push_back(normal_[2]);


            token.push_back(old_x);
            token.push_back(old_y);
            token.push_back(z - ((j+1) * barrel_mesh_size));
            token.push_back(normal_[0]);
            token.push_back(normal_[1]);
            token.push_back(normal_[2]);


            token.push_back(x);
            token.push_back(y);
            token.push_back(z - (j+1) * barrel_mesh_size);
            token.push_back(normal_[0]);
            token.push_back(normal_[1]);
            token.push_back(normal_[2]);

            //second triangle
            token.push_back(old_x);
            token.push_back(old_y);
            token.push_back(z - (j * barrel_mesh_size));
            token.push_back(normal_[0]);
            token.push_back(normal_[1]);
            token.push_back(normal_[2]);

            token.push_back(x);
            token.push_back(y);
            token.push_back(z - (j+1) * barrel_mesh_size);
            token.push_back(normal_[0]);
            token.push_back(normal_[1]);
            token.push_back(normal_[2]);

            token.push_back(x);
            token.push_back(y);
            token.push_back(z - (j * barrel_mesh_size));
            token.push_back(normal_[0]);
            token.push_back(normal_[1]);
            token.push_back(normal_[2]);


        }

    }

    m_vertexData = token;

}
