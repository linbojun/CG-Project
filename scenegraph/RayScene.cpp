#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <iostream>
#include <thread>
#include <glm/gtx/string_cast.hpp>

std::mutex mtx;

RayScene::RayScene(Scene &scene) :
    //copy construction
    Scene(scene)
{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.



}


RayScene::~RayScene()
{
    std::cout<<"call ray scene destructor"<<std::endl;
    //delete m_result;
}

RGBA RayScene::rayTrace(glm::vec4 eye, glm::vec4 unit_d)
{
   // std::cout<<"primitive_list size: "<<m_primitive_list.size()<<std::endl;
    RGBA result(0, 0, 0);
    float dist = FLT_MAX;
    int min_dist = -1;
    mtx.lock();
    for(int i = 0; i < m_primitive_list.size(); i++)
    {

        if(m_primitive_list.at(i).type == PrimitiveType::PRIMITIVE_CUBE)
        {
            float t = cube_intersect(m_transformation_list.at(i), eye, unit_d, dist);
            if(t < dist)
            {
                dist = t;
                min_dist = i;
            }
        }

        else if(m_primitive_list.at(i).type == PrimitiveType::PRIMITIVE_CONE)
        {
            float t = cone_intersect(m_transformation_list.at(i), eye, unit_d, dist);
            if(t < dist)
            {
                dist = t;
                min_dist = i;
            }
        }

        else if(m_primitive_list.at(i).type == PrimitiveType::PRIMITIVE_CYLINDER)
        {
            float t = cylinder_intersect(m_transformation_list.at(i), eye, unit_d, dist);
            if(t < dist)
            {
                dist = t;
                min_dist = i;
            }
        }
        else if(m_primitive_list.at(i).type == PrimitiveType::PRIMITIVE_SPHERE)
        {
            float t = sphere_intersect(m_transformation_list.at(i), eye, unit_d, dist);
            if(t < dist)
            {
                dist = t;
                min_dist = i;
            }
        }


    }

    if(min_dist != -1){
        glm::vec4 token(0,0,0,0);
        CS123ScenePrimitive closest = m_primitive_list.at(min_dist);
        glm::vec4 intersect_pos = eye + dist * unit_d;
    //std::cout<<glm::to_string(closest.material.cDiffuse)<<std::endl;
       token += closest.material.cAmbient;
       for( int i = 0; i < m_light_list.size(); i++)
       {
           glm::vec4 light_vec = m_light_list.at(i).pos - intersect_pos ;
           //light_vec[3] = 0;
           light_vec = glm::normalize(light_vec);

            token += m_light_list.at(i).color* (closest.material.cDiffuse *  glm::dot(m_closest_normal, light_vec));
       }


      // std::cout<<"In ray trace: "<<std::endl;
      // std::cout<<"dist: "<<dist<<", min_dist: "<<min_dist<<std::endl<<std::endl;
      std::cout<<"normal: "<<glm::to_string(m_closest_normal)<<std::endl;
      // std::cout<<"light token: "<<glm::to_string(token)<<std::endl;

        result.r = REAL2byte(token[0]);
        result.g = REAL2byte(token[1]);
        result.b = REAL2byte(token[2]);

       //result.r = 255;
       //result.g = 255;
       //result.b = 255;


     //   result.
      //  std::cout<<"light result: ("<<result.r<<"," <<result.g<<","<<result.b<<")"<<std::endl;

    }
    mtx.unlock();

    return result;




}

void RayScene::render(Canvas2D *canvas, Camera* cam)
{

    int width = canvas->width();
    int height = canvas->height();
    float width_f = width;
    float height_f = height;
    RGBA result[width * height];
    glm::vec4 eye = glm::vec4(0, 0, 0, 1);
    glm::vec4 eye_world = glm::inverse(cam->getViewMatrix()) * eye;

    for(float x = 0; x < width; x++)
    {
        for(float y = 0; y < height; y++)
        {

            float x_dir = 2*x/width_f-1;
            float y_dir = 1-(2*y/height_f);
            glm::vec4 p_film = glm::vec4(x_dir, y_dir, -1, 1);
            glm::mat4x4 film_to_world = glm::inverse(cam->getScaleMatrix() * cam->getViewMatrix());
            glm::vec4 p_world = film_to_world * p_film;
            glm::vec4 d = (p_world-eye_world)/glm::length(p_world-eye_world);
            std::cout<<"p_film: "<<glm::to_string(p_film)<<std::endl;
            std::cout<<"p_world: "<<glm::to_string(p_world)<<std::endl;
            std::cout<<"eye_world: "<<glm::to_string(eye_world)<<std::endl;
            std::cout<<"unit_d: "<<glm::to_string(d)<<std::endl;
            d[3] = 0;
            RGBA token = rayTrace(eye_world, d);
            result[(int)(y * width + x)] = token;


        }
    }
    std::cout<<"finish all film piexel"<<std::endl;
     memcpy(canvas->data(), result, width * height * sizeof(RGBA));
}

void RayScene::render_multithread(Canvas2D *canvas, Camera* cam)
{
    int width = canvas->width();
    int height = canvas->height();
    m_result = new RGBA[canvas->width() * canvas->height()];
    m_cam = cam;
    m_canvas = canvas;
    std::thread top(&RayScene::render_top, this);
    std::thread bot(&RayScene::render_bot, this);
    top.join();
    bot.join();
     memcpy(canvas->data(), m_result, width * height * sizeof(RGBA));
     delete m_result;
     std::cout<<"finish all film piexel"<<std::endl;
}

void RayScene::render_top()

{
    int width = m_canvas->width();
    int height = m_canvas->height();
    float width_f = width;
    float height_f = height;
    glm::vec4 eye = glm::vec4(0, 0, 0, 1);
    glm::vec4 eye_world = glm::inverse(m_cam->getViewMatrix()) * eye;

    for(float x = 0; x < width; x++)
    {
        for(float y = 0; y < height/2; y++)
        {

            float x_dir = 2*x/width_f-1;
            float y_dir = 1-(2*y/height_f);
            glm::vec4 p_film = glm::vec4(x_dir, y_dir, -1, 1);
            glm::mat4x4 film_to_world = glm::inverse(m_cam->getScaleMatrix()* m_cam->getViewMatrix() );
            glm::vec4 p_world = film_to_world * p_film;
            glm::vec4 d = glm::normalize(p_world-eye_world);
          //  std::cout<<"p_film: "<<glm::to_string(p_film)<<std::endl;
          //  std::cout<<"p_world: "<<glm::to_string(p_world)<<std::endl;
          //  std::cout<<"eye_world: "<<glm::to_string(eye_world)<<std::endl;
          //  std::cout<<"unit_d: "<<glm::to_string(d)<<std::endl;
            d[3] = 0;
            RGBA token = rayTrace(eye_world, d);
            m_result[(int)(y * width + x)] = token;


        }
    }
    pthread_exit(NULL);
}

void RayScene::render_bot()
{
    int width = m_canvas->width();
    int height = m_canvas->height();
    float width_f = width;
    float height_f = height;
    glm::vec4 eye = glm::vec4(0, 0, 0, 1);
    glm::vec4 eye_world = glm::inverse(m_cam->getViewMatrix()) * eye;

    for(float x =  0; x < width; x++)
    {
        for(float y = height/2; y < height; y++)
        {

            float x_dir = 2*x/width_f-1;
            float y_dir = 1-(2*y/height_f);
            glm::vec4 p_film = glm::vec4(x_dir, y_dir, -1, 1);
            glm::mat4x4 film_to_world = glm::inverse(m_cam->getScaleMatrix() * m_cam->getViewMatrix());
            glm::vec4 p_world = film_to_world * p_film;
            glm::vec4 d = (p_world-eye_world)/glm::length(p_world-eye_world);
            //std::cout<<"p_film: "<<glm::to_string(p_film)<<std::endl;
            //std::cout<<"p_world: "<<glm::to_string(p_world)<<std::endl;
            //std::cout<<"eye_world: "<<glm::to_string(eye_world)<<std::endl;
            //std::cout<<"unit_d: "<<glm::to_string(d)<<std::endl;
            d[3] = 0;
            RGBA token = rayTrace(eye_world, d);
            m_result[(int)(y * width + x)] = token;

        }
    }
   pthread_exit(NULL);
}


float RayScene::cube_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist)
{
   //  std::cout<<"run cube_intersect"<<std::endl;
    glm::vec4 eye_obj = glm::inverse(transformation) * eye;
    glm::vec4 d_obj = glm::inverse(transformation) * unit_d;
    glm::mat3 transform_token(transformation);
  //  std::cout<<"transformation: "<<glm::to_string(transformation)<<std::endl;
  //  std::cout<<"transform_token: "<<glm::to_string(transform_token)<<std::endl;
    //std::cout<<"eye_obj: "<<glm::to_string(eye_obj)<<std::endl;
    //std::cout<<"d_obj: "<<glm::to_string(d_obj)<<std::endl;

    //top side y = (0, 1, 0, 0) * transformation
    float t = (0.5 - eye_obj.y)/d_obj.y;
    glm::vec4 intersect_obj = eye_obj + t * d_obj;
    //std::cout<<"top intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
    if(t >= 0 && t < dist && intersect_obj.x <= 0.5  && intersect_obj.x >= -0.5  && intersect_obj.z <= 0.5  &&intersect_obj.z >= -0.5 )
    {
        dist = std::min(dist, t);
        glm::vec3 normal(0,1,0);
        glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
        m_closest_normal = glm::vec4(token, 0);

    }

    //bot side y = (0, -1, 0, 0)
    t = (-0.5 -eye_obj.y)/d_obj.y;
    intersect_obj = eye_obj + t * d_obj;
    //std::cout<<"bot intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
    if(t >= 0 && t < dist && intersect_obj.x <= 0.5  && intersect_obj.x >= -0.5  && intersect_obj.z <= 0.5  &&intersect_obj.z >= -0.5 )
    {
         dist = std::min(dist, t);
         glm::vec3 normal(0,-1,0);

         glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
         m_closest_normal = glm::vec4(token, 0);
    }

    //right side x = 1
    t = (0.5  - eye_obj.x)/d_obj.x;
    intersect_obj = eye_obj + t * d_obj;
    //std::cout<<"right intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
    if(t >= 0 && t < dist && intersect_obj.y <= 0.5  && intersect_obj.y >= -0.5  && intersect_obj.z <= 0.5  &&intersect_obj.z >= -0.5 )
    {
         dist = std::min(dist, t);
         glm::vec3 normal(1,0,0);

         glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
         m_closest_normal = glm::vec4(token, 0);
    }

    //left side x = -1
    t = (-0.5  - eye_obj.x)/d_obj.x;
    intersect_obj = eye_obj + t * d_obj;
     //std::cout<<"left intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
    if(t >= 0 && t < dist && intersect_obj.y <= 0.5  && intersect_obj.y >= -0.5  && intersect_obj.z <= 0.5  &&intersect_obj.z >= -0.5 )
    {
         dist = std::min(dist, t);
         glm::vec3 normal(-1,0,0);

         glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
         m_closest_normal = glm::vec4(token, 0);
    }

    //front side z= 1
    t = (0.5  - eye_obj.z)/d_obj.z;
    intersect_obj = eye_obj + t * d_obj;
    //std::cout<<"front intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
    if(t >= 0 && t < dist && intersect_obj.y <= 0.5  && intersect_obj.y >= -0.5  && intersect_obj.x <= 0.5  &&intersect_obj.x >= -0.5 )
    {
        dist = std::min(dist, t);
        glm::vec3 normal(0,0,1);

        glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
        m_closest_normal = glm::vec4(token, 0);

    }

    //back side z=-1
    t = (-0.5  - eye_obj.z)/d_obj.z;
    intersect_obj = eye_obj + t * d_obj;
    //std::cout<<"back intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
    if(t >= 0 && t < dist && intersect_obj.y <= 0.5 && intersect_obj.y >= -0.5  && intersect_obj.x <= 0.5  &&intersect_obj.x >= -0.5 )
    {
         dist = std::min(dist, t);
         glm::vec3 normal(0,0,-1);

         glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
         m_closest_normal = glm::vec4(token, 0);
    }
    return dist;
}


float RayScene::cone_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d,  float dist)
{
  //   std::cout<<"run cone_intersect"<<std::endl;

    glm::vec4 eye_obj = glm::inverse(transformation) * eye;
    glm::vec4 d_obj = glm::inverse(transformation) * unit_d;
    glm::vec4 P = eye_obj;
    glm::vec4 d = d_obj;
    glm::vec4 intersect_obj;
    glm::mat3 transform_token(transformation);
   // std::cout<<"transformation: "<<glm::to_string(transformation)<<std::endl;
  //  std::cout<<"transform_token: "<<glm::to_string(transform_token)<<std::endl;

    //cone body intersect check
    float A = pow(d.x, 2) + pow(d.z, 2) - 0.25*pow(d.y, 2);
    float B = 2*P.x*d.x + 2*P.z*d.z - 0.5*P.y*d.y +0.25*d.y;
    float C = pow(P.x, 2) + pow(P.z, 2) - 0.25*pow(P.y, 2) + 0.25*P.y - 1.f/16.f;
    if(pow(B,2) - 4 * A * C >= 0)
    {
       float t1 = (-B + sqrt(pow(B,2) - 4*A*C))/(2*A);
       float t2 = (-B - sqrt(pow(B,2) - 4*A*C))/(2*A);
       if(t1 >= 0 && t1 < dist ){
            glm::vec4 intersect_obj = P + t1 * d;
            if(intersect_obj.y <= 0.5 && intersect_obj.y >= -0.5){
                 dist = std::min(dist, t1);
                 glm::vec3 normal(2*intersect_obj.x, 0.5*intersect_obj.y - 0.25, 2*intersect_obj.z);
                 normal = glm::normalize(normal);
                 glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
                 m_closest_normal = glm::vec4(token, 0);

            }
       }
       if(t2 >= 0 && t2 < dist)
       {
           glm::vec4 intersect_obj = P + t2 * d;
           if(intersect_obj.y <= 0.5 && intersect_obj.y >= -0.5){

               dist = std::min(dist, t2);
               glm::vec3 normal(2*intersect_obj.x, 0.5*intersect_obj.y - 0.25, 2*intersect_obj.z);
               normal = glm::normalize(normal);
               glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
               m_closest_normal = glm::vec4(token, 0);

           }
       }
    }

    //cone cap intersect check
    float t = (-0.5 - P.y)/d.y;
    intersect_obj = P + t * d;
    float x = intersect_obj.x;
    float z = intersect_obj.z;
    if(t >= 0 && t < dist && pow(x, 2) + pow(z, 2) <= pow(0.5, 2))
    {

         dist = std::min(dist, t);
         glm::vec3 normal(0,-1,0);
         glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
         m_closest_normal = glm::vec4(token, 0);

    }
    return dist;

}

float RayScene::cylinder_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d,  float dist)
{
 //   std::cout<<"run cylinder_intersect"<<std::endl;
    //float dist = FLT_MAX;
    glm::vec4 P= glm::inverse(transformation) * eye;
    glm::vec4 d = glm::inverse(transformation) * unit_d;
    glm::vec4 intersect_obj;
    glm::mat3 transform_token(transformation);
   // std::cout<<"transformation: "<<glm::to_string(transformation)<<std::endl;
  //  std::cout<<"transform_token: "<<glm::to_string(transform_token)<<std::endl;

    //top cap
    float t = (0.5 - P.y)/d.y;
    intersect_obj = P + t * d;
    float x = intersect_obj.x;
    float z = intersect_obj.z;
    if(t >= 0 && t < dist && pow(x, 2) + pow(z, 2) <= pow(0.5, 2))
    {

         dist = std::min(dist, t);
         glm::vec3 normal(0,1,0);
         glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
         m_closest_normal = glm::vec4(token, 0);
    }

    //bot cap
    t = (-0.5 - P.y)/d.y;
    intersect_obj = P + t * d;
    x = intersect_obj.x;
    z = intersect_obj.z;
    if(t >= 0 && t < dist && pow(x, 2) + pow(z, 2) <= pow(0.5, 2))
    {
        dist = std::min(dist, t);
        glm::vec3 normal(0,-1,0);
        glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
        m_closest_normal = glm::vec4(token, 0);
    }

    //cylinder body
    float A = pow(d.x, 2) + pow(d.z, 2);
    float B = 2 * P.x * d.x + 2 * P.z *d.z;
    float C = pow(P.x, 2) + pow(P.z, 2)-0.25;
    if(pow(B,2) - 4 * A* C >= 0)
    {
        float t1 = (-B + sqrt(pow(B,2) - 4*A*C))/(2*A);
        float t2 = (-B - sqrt(pow(B,2) - 4*A*C))/(2*A);
        if(t1 >= 0 && t1 < dist){
             glm::vec4 intersect_obj = P + t1 * d;
             if(intersect_obj.y <= 0.5 && intersect_obj.y >= -0.5){

                 dist = std::min(dist, t1);
                 glm::vec3 normal(2*intersect_obj.x, 0, 2*intersect_obj.z);
                 normal = glm::normalize(normal);
                 glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
                 m_closest_normal = glm::vec4(token, 0);

             }
        }
        if(t2 >= 0 && t2 < dist)
        {
            glm::vec4 intersect_obj = P + t2 * d;
            if(intersect_obj.y <= 0.5 && intersect_obj.y >= -0.5){
                dist = std::min(dist, t2);
                glm::vec3 normal(2*intersect_obj.x, 0, 2*intersect_obj.z);
                normal = glm::normalize(normal);
                glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
                m_closest_normal = glm::vec4(token, 0);
            }
        }
    }
    return dist;

}

float RayScene::sphere_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d,  float dist)
{
  //   std::cout<<"run sphere_intersect"<<std::endl;
   // float dist = FLT_MAX;
    glm::vec4 P= glm::inverse(transformation) * eye;
    glm::vec4 d = glm::inverse(transformation) * unit_d;
    glm::vec4 intersect_obj;
    glm::mat3 transform_token(transformation);
   // std::cout<<"transformation: "<<glm::to_string(transformation)<<std::endl;
   // std::cout<<"transform_token: "<<glm::to_string(transform_token)<<std::endl;

    float A = pow(d.x, 2) + pow(d.y, 2)+ pow(d.z, 2);
    float B = 2 * P.x * d.x + 2 * P.y * d.y +2 * P.z *d.z;
    float C = pow(P.x, 2) + pow(P.y, 2) + pow(P.z, 2)-0.25;
    if(pow(B,2) - 4 * A* C >= 0)
    {
        float t1 = (-B + sqrt(pow(B,2) - 4*A*C))/(2*A);
        float t2 = (-B - sqrt(pow(B,2) - 4*A*C))/(2*A);
        if(t1 >= 0 && t1 < dist){
             glm::vec4 intersect_obj = P + t1 * d;
             if(intersect_obj.y <= 0.5 && intersect_obj.y >= -0.5){
                 dist = std::min(dist, t1);
                 glm::vec3 normal(2*intersect_obj.x, 2*intersect_obj.y, 2*intersect_obj.z);
                 normal = glm::normalize(normal);
                 glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
                 m_closest_normal = glm::vec4(token, 0);
             }
        }
        if(t2 >= 0 && t2 < dist)
        {
            glm::vec4 intersect_obj = P + t2 * d;
            if(intersect_obj.y <= 0.5 && intersect_obj.y >= -0.5){
                dist = std::min(dist, t2);
                glm::vec3 normal(2*intersect_obj.x, 2*intersect_obj.y, 2*intersect_obj.z);
                normal = glm::normalize(normal);
                glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
                m_closest_normal = glm::vec4(token, 0);
            }
        }
    }
    return dist;



}


unsigned char RayScene::REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}


