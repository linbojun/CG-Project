#include "Cone.h"
#include "ui/Settings.h"
#include "math.h"

Cone::Cone(){

}

Cone::Cone(int p1, int p2):
    mesh_size(p1),
    side_size(p2)
{
    if(side_size < 3)
        side_size = 3;
    shape_type = SHAPE_CONE;
    create_vertex_data();
    buildVAO();
}

Cone::~Cone(){
}



void Cone::create_vertex_data()
{
    float side_space = (2 *M_PI) / (float)side_size;
    float x = 0.5, y = -0.5, z = 0;
    create_cycle(-0.5, false);

    float tip_x = 0, tip_y = 0.5, tip_z = 0;
    x = 0.5, y = -0.5, z = 0;
    for(int i = 1;  i <= side_size; i++)
    {
        float old_x = x;
        float old_z = z;
        float old_z_spc = old_z/(float)mesh_size;
        float old_x_spc = old_x/(float)mesh_size;
        float y_spc = (y - 0.5)/(float)mesh_size;
        x = cos(i * side_space) * 0.5;
        z = sin(i * side_space) * 0.5;
        float new_x_spc = x/(float)mesh_size;
        float new_z_spc = z/(float)mesh_size;
        glm::vec3 v1 = {old_x - tip_x, y - tip_y, old_z - tip_z};
        glm::vec3 v2 = {x - tip_x, y - tip_y, z - tip_z};
        glm::vec3 _normal = crossProduct(v1, v2);
        _normal = -glm::normalize(_normal);


        update_m_vtx(tip_x, tip_y, tip_z, _normal);

       // update_m_vtx(tip_x+new_x_spc, tip_y-y_spc, tip_z+new_z_spc, _normal);
          update_m_vtx(tip_x + new_x_spc, tip_y+ y_spc, tip_z+new_z_spc, _normal);
       // update_m_vtx(tip_x-old_x_spc, tip_y-y_spc, tip_z + new_z_spc, _normal);
        update_m_vtx(tip_x + old_x_spc, tip_y + y_spc, tip_z+old_z_spc, _normal);

        for(int j = 1; j < mesh_size; j++)
        {
            //first triangle
            float top_old_x = tip_x + j * old_x_spc;
            float top_old_y = tip_y + j * y_spc;
            float top_old_z = tip_z + j * old_z_spc;
            update_m_vtx(tip_x + j * old_x_spc, tip_y + j * y_spc, tip_z + j * old_z_spc, _normal);

            update_m_vtx(tip_x + (j+1) * new_x_spc, tip_y + (j+1)*y_spc, tip_z + (j+1)*new_z_spc, _normal);
             update_m_vtx(top_old_x + old_x_spc, top_old_y+y_spc, top_old_z+old_z_spc, _normal);

            //second triangle
            update_m_vtx(tip_x + j * old_x_spc, tip_y + j * y_spc, tip_z + j * old_z_spc, _normal);

            update_m_vtx(tip_x + j*new_x_spc, tip_y +j*y_spc, tip_z+j*new_z_spc, _normal);
            update_m_vtx(tip_x + (j+1) * new_x_spc, tip_y + (j+1)*y_spc, tip_z + (j+1)*new_z_spc, _normal);
        }
    }


}





void Cone::create_cycle(float y_axis, bool face_top)
{
    float side_space = (2*M_PI)/(float)side_size;
    float x = 0.5, y = y_axis, z = 0;
    std::vector<float> normal;
    if(face_top)
        normal = {0, 1, 0};
    else
        normal = {0, -1, 0};
    for(int i = 1; i <= side_size; i++)
    {
        float old_x = x;
        float old_z = z;
        float old_x_spc = old_x/(float)mesh_size;
        float old_z_spc = old_z/(float)mesh_size;

        x = cos(i * side_space) * 0.5;
        z = sin(i * side_space) * 0.5;
        float new_x_spc = x/(float)mesh_size;
        float new_z_spc = z/(float)mesh_size;

        if(face_top){
            update_m_vtx(old_x_spc, y, old_z_spc, normal);
            update_m_vtx(0, y, 0, normal);
            update_m_vtx(new_x_spc, y, new_z_spc, normal);
        }
        else
        {
            update_m_vtx(0, y, 0, normal);
            update_m_vtx(old_x_spc, y, old_z_spc, normal);
            update_m_vtx(new_x_spc, y, new_z_spc, normal);
        }


        for(int j = 1; j < mesh_size; j++)
        {

            //first triangle
            if(face_top){
            update_m_vtx(j*old_x_spc, y, j*old_x_spc, normal);

            update_m_vtx((j+1)*new_x_spc, y, (j+1)*new_z_spc, normal);

            update_m_vtx((j+1)*old_x_spc, y, (j+1)*old_z_spc, normal);

            //second triangle
            update_m_vtx(j*old_x_spc, y, j*old_z_spc, normal);

            update_m_vtx(j*new_x_spc, y, j*new_z_spc, normal);

            update_m_vtx((j+1)*new_x_spc, y, (j+1)*new_z_spc, normal);
            }
            else{
                update_m_vtx(j * old_x_spc, y, j * old_z_spc, normal);

                update_m_vtx((j+1) * old_x_spc, y, (j+1) * old_z_spc, normal);

                update_m_vtx((j+1) * new_x_spc, y, (j+1) * new_z_spc, normal);

                //second triangle
                update_m_vtx(j * old_x_spc, y, j * old_z_spc, normal);

                update_m_vtx((j+1) * new_x_spc, y, (j+1) * new_z_spc, normal);

                 update_m_vtx(j * new_x_spc, y, j * new_z_spc, normal);
            }


        }

    }

}


