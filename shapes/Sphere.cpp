#include "Sphere.h"
#include "ui/Settings.h"
#include "math.h"

Sphere::Sphere()
{

}

Sphere::Sphere(int p1, int p2):
    mesh_size(p1),
    side_size(p2){

    if(mesh_size < 2)
        mesh_size = 2;
    if(side_size < 3)
        side_size = 3;
    shape_type = SHAPE_SPHERE;
    create_vertex_data();
    buildVAO();
}

Sphere::~Sphere()
{

}

void Sphere::create_vertex_data()
{
    float side_space = (2 * M_PI) / (float)side_size;
    float mesh_space = M_PI / (float)mesh_size;
    for(int i = 0; i < side_size; i++)
    {
         //draw the upper part
        float top_x = 0, top_y = 0.5, top_z = 0;
        float left_x = 0.5*sin(mesh_space) *cos((i+1) * side_space);
        float left_y = 0.5*cos(mesh_space);
        float left_z = 0.5*sin(mesh_space)*sin((i+1) * side_space);

        float right_x = 0.5*sin(mesh_space)*cos(i * side_space);
        float right_y = 0.5*cos(mesh_space);
        float right_z = 0.5*sin(mesh_space)*sin(i * side_space);

        glm::vec3 v1 = {right_x - left_x, right_y-left_y, right_z - left_z};
        glm::vec3 v2 = {right_x - top_x, right_y - top_y, right_z - top_z};
        glm::vec3 normal = crossProduct(v1, v2);
        normal = -glm::normalize(normal);
        update_m_vtx(top_x, top_y, top_z, normal);
        update_m_vtx(left_x, left_y, left_z, normal);
        update_m_vtx(right_x, right_y, right_z, normal);

        //draw the bot part
         float bot_x = 0, bot_y = -0.5, bot_z = 0;
         left_x = 0.5*sin((mesh_size - 1) * mesh_space) * cos((i+1) * side_space);
         left_y = 0.5*cos((mesh_size - 1) * mesh_space);
         left_z = 0.5*sin((mesh_size - 1) * mesh_space) * sin((i+1) * side_space);

         right_x = 0.5*sin((mesh_size - 1) * mesh_space) * cos(i * side_space);
         right_y = 0.5*cos((mesh_size - 1) * mesh_space);
         right_z = 0.5*sin((mesh_size - 1) * mesh_space) * sin(i * side_space);

        v1 = {right_x - left_x, right_y-left_y, right_z - left_z};
        v2 = {right_x - bot_x, right_y - bot_y, right_z - bot_z};
        normal = crossProduct(v1, v2);
        normal = glm::normalize(normal);
        update_m_vtx(right_x, right_y, right_z, normal);
        update_m_vtx(left_x, left_y, left_z, normal);
        update_m_vtx(bot_x, bot_y, bot_z, normal);

        for(int j = 1; j < mesh_size-1; j++)
        {
             //define four corner vertices
            float top_right_x =  0.5*sin(j * mesh_space) * cos(i * side_space);
            float top_right_y =  0.5*cos(j * mesh_space);
            float top_right_z =  0.5*sin(j * mesh_space)*sin(i * side_space);

            float top_left_x = 0.5*sin(j * mesh_space) * cos((i+1) * side_space);
            float top_left_y = 0.5*cos(j * mesh_space);
            float top_left_z = 0.5*sin(j * mesh_space)*sin((i+1) * side_space);

            float bot_right_x = 0.5*sin((j+1) * mesh_space) * cos(i * side_space);
            float bot_right_y = 0.5*cos((j+1) * mesh_space);
            float bot_right_z = 0.5*sin((j+1) * mesh_space)*sin(i * side_space);

            float bot_left_x = 0.5*sin((j+1) * mesh_space) * cos((i+1) * side_space);
            float bot_left_y = 0.5*cos((j+1) * mesh_space);
            float bot_left_z = 0.5*sin((j+1) * mesh_space)*sin((i+1) * side_space);

            glm::vec3 v1 = {top_right_x - bot_left_x, top_right_y - bot_left_y, top_right_z - bot_left_z};
            glm::vec3 v2 = {bot_right_x - bot_left_x, bot_right_y - bot_left_y, bot_right_z - bot_left_z};
            glm::vec3 normal = crossProduct(v1, v2);
            normal = -glm::normalize(normal);

            //add first vertex
            update_m_vtx(top_right_x, top_right_y, top_right_z, normal);
            update_m_vtx(top_left_x, top_left_y, top_left_z, normal);
            update_m_vtx(bot_right_x, bot_right_y, bot_right_z, normal);

            //add second vertex
            update_m_vtx(bot_right_x, bot_right_y, bot_right_z, normal);
            update_m_vtx(top_left_x, top_left_y, top_left_z, normal);
            update_m_vtx(bot_left_x, bot_left_y, bot_left_z, normal);
        }


    }
}
