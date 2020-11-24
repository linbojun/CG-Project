#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <math.h>
#include <iostream>
#include <thread>
#include <glm/gtx/string_cast.hpp>
#include <QCoreApplication>
#include <unistd.h>


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
    for(int i = 0; i < m_primitive_list.size(); i++)
    {
        m_shapes.push_back(m_primitive_list.at(i));
        m_transformation.push_back(m_transformation_list.at(i));
        char buff[FILENAME_MAX]; //create string buffer to hold path
        getcwd( buff, FILENAME_MAX );
        std::string current_working_dir(buff);
        QString file_name = QString::fromStdString(current_working_dir + m_primitive_list.at(i).material.textureMap.filename);
        //std::cout<<"textMap place 1, file_name: "<<m_primitive_list.at(i).material.textureMap.filename<<std::endl;
        QImage image(file_name);
        m_tetxures.push_back(image);
    }
}


RayScene::~RayScene()
{

}


glm::vec4 RayScene::rayTrace(glm::vec4 eye, glm::vec4 unit_d, int depth)
{
   // std::cout<<"ray tracing"<<std::endl;
   // std::cout<<"primitive_list size: "<<m_primitive_list.size()<<std::endl;
   glm::vec4 result(0,0,0,0);
    if(depth == 0)
        return result;
    std::tuple<float, glm::vec4, int> ans;
    if(settings.useKDTree == true)
        ans = tree_traverse(eye, unit_d);
    else
        ans = iterate_traverse(eye, unit_d);
   float t = std::get<0>(ans);
   glm::vec4 normal = std::get<1>(ans);
   int closest_index = std::get<2>(ans);

    if(closest_index != -1){
        //std::cout<<"closest_index: "<<closest_index<<std::endl;
        std::pair<float, glm::vec4> t_normal(t, normal);
        result += estimate_direct_light(closest_index, t_normal, unit_d, eye);
        if(settings.useReflection)
            result += estimate_indirect_light(closest_index, t_normal, unit_d, eye, depth);

    }

    return result;

}

std::tuple<float, glm::vec4, int> RayScene::iterate_traverse(glm::vec4 eye, glm::vec4 unit_d)
{
    float dist = FLT_MAX;
    int min_dist = -1;
    glm::vec4 normal;

    for(int i = 0; i < m_primitive_list.size(); i++)
   // for(int i = 1; i < 2; i++)
    {
        int index = i;
        if(m_primitive_list.at(index).type == PrimitiveType::PRIMITIVE_CUBE)
        {
            std::pair<float, glm::vec4> token = cube_intersect(m_transformation_list.at(index), eye, unit_d, dist);
            if(token.first < dist && token.first > 0)
            {
                dist = token.first;
                min_dist = index;
                normal = token.second;
            }
        }

        else if(m_primitive_list.at(index).type == PrimitiveType::PRIMITIVE_CONE)
        {
            std::pair<float, glm::vec4> token = cone_intersect(m_transformation_list.at(index), eye, unit_d, dist);
            if(token.first < dist && token.first > 0)
            {
                dist = token.first;
                min_dist = index;
                normal = token.second;
            }
        }

        else if(m_primitive_list.at(index).type == PrimitiveType::PRIMITIVE_CYLINDER)
        {
            std::pair<float, glm::vec4> token = cylinder_intersect(m_transformation_list.at(index), eye, unit_d, dist);
            if(token.first < dist && token.first > 0)
            {
                dist = token.first;
                min_dist = index;
                normal = token.second;
            }
        }
        else if(m_primitive_list.at(index).type == PrimitiveType::PRIMITIVE_SPHERE)
        {
            std::pair<float, glm::vec4> token = sphere_intersect(m_transformation_list.at(index), eye, unit_d, dist);
            if(token.first < dist && token.first > 0)
            {
                dist = token.first;
                min_dist = index;
                normal = token.second;
            }
        }
    }
    std::tuple<float, glm::vec4, int> ans(dist, normal, min_dist);
    return ans;
}


std::tuple<float, glm::vec4, int> RayScene::tree_traverse(glm::vec4 eye, glm::vec4 unit_d)
{
    float dist = FLT_MAX;
    int min_dist = -1;
    glm::vec4 normal;

    node* node_token = m_tree->search(eye, unit_d);
    if(node_token != NULL)
    {
        std::cout<< "size of node_token.index: "<<node_token->index.size()<<std::endl<<std::endl;;
        for(int i = 0; i < node_token->index.size(); i++)
        {
            int index = node_token->index.at(i);
            if(m_primitive_list.at(index).type == PrimitiveType::PRIMITIVE_CUBE)
            {
                std::pair<float, glm::vec4> token = cube_intersect(m_transformation_list.at(index), eye, unit_d, dist);
                if(token.first < dist && token.first > 0)
                {
                    dist = token.first;
                    min_dist = index;
                    normal = token.second;
                }
            }

            else if(m_primitive_list.at(index).type == PrimitiveType::PRIMITIVE_CONE)
            {
                std::pair<float, glm::vec4> token = cone_intersect(m_transformation_list.at(index), eye, unit_d, dist);
                if(token.first < dist && token.first > 0)
                {
                    dist = token.first;
                    min_dist = index;
                    normal = token.second;
                }
            }

            else if(m_primitive_list.at(index).type == PrimitiveType::PRIMITIVE_CYLINDER)
            {
                std::pair<float, glm::vec4> token = cylinder_intersect(m_transformation_list.at(index), eye, unit_d, dist);
                if(token.first < dist && token.first > 0)
                {
                    dist = token.first;
                    min_dist = index;
                    normal = token.second;
                     std::cout<<"normal: "<<glm::to_string(normal)<<std::endl;
                }
            }
            else if(m_primitive_list.at(index).type == PrimitiveType::PRIMITIVE_SPHERE)
            {
                std::pair<float, glm::vec4> token = sphere_intersect(m_transformation_list.at(index), eye, unit_d, dist);
                if(token.first < dist && token.first > 0)
                {
                    dist = token.first;
                    min_dist = index;
                    normal = token.second;
                }
            }
        }

    }

     std::tuple<float, glm::vec4, int> ans(dist, normal, min_dist);
     return ans;

}

glm::vec4 RayScene::estimate_direct_light(int closest_index, std::pair<float, glm::vec4> closest_data,  glm::vec4 unit_d, glm::vec4 eye)
{
    float t = closest_data.first;
    glm::vec4 normal = closest_data.second;

    CS123ScenePrimitive closest_primitive = m_primitive_list.at(closest_index);
    glm::vec4 intersect_pos = eye + t * unit_d;
    glm::vec4 color(0,0,0,0);


    color += closest_primitive.material.cAmbient * m_global.ka;
  //  std::cout<<"material ambient: "<<glm::to_string(closest_primitive.material.cAmbient)<<std::endl;
  //  std::cout<<"global ka: "<<m_global.ka<<std::endl;
  //  std::cout<<"Ambient color: "<<glm::to_string(color)<<std::endl;
    glm::vec4 texture_color(0.0);
    if(settings.useTextureMapping && closest_primitive.material.textureMap.isUsed){
        glm::vec2 uv(0.0);
        if(closest_primitive.type == PrimitiveType::PRIMITIVE_CUBE){

            uv = map2cube(intersect_pos, m_transformation_list.at(closest_index));
            std::cout<<"finish running map2sphere(intersect_pos, m_transformation_list.at(closest_index))"<<std::endl;
            std::cout<<"uv: "<<glm::to_string(uv)<<std::endl;
            texture_color += textureMap(uv, closest_index);
        }

        else if(closest_primitive.type == PrimitiveType::PRIMITIVE_CONE){

            uv = map2cone(intersect_pos, m_transformation_list.at(closest_index));
            std::cout<<"finish running map2sphere(intersect_pos, m_transformation_list.at(closest_index))"<<std::endl;
            std::cout<<"uv: "<<glm::to_string(uv)<<std::endl;
            texture_color += textureMap(uv, closest_index);
        }
        else if(closest_primitive.type == PrimitiveType::PRIMITIVE_CYLINDER){

            uv = map2cylinder(intersect_pos, m_transformation_list.at(closest_index));
            std::cout<<"finish running map2sphere(intersect_pos, m_transformation_list.at(closest_index))"<<std::endl;
            std::cout<<"uv: "<<glm::to_string(uv)<<std::endl;
            texture_color += textureMap(uv, closest_index);
        }
        else if(closest_primitive.type == PrimitiveType::PRIMITIVE_SPHERE){

            uv = map2sphere(intersect_pos, m_transformation_list.at(closest_index));
            std::cout<<"finish running map2sphere(intersect_pos, m_transformation_list.at(closest_index))"<<std::endl;
            std::cout<<"uv: "<<glm::to_string(uv)<<std::endl;
            texture_color += textureMap(uv, closest_index);
        }
        std::cout<<"In :estimate_direct_light, text_color ouput: "<<glm::to_string(texture_color)<<std::endl;
    }

    for( int i = 0; i < m_light_list.size(); i++)
    {
        CS123SceneLightData cur_light = m_light_list.at(i);
        if(cur_light.type == LightType::LIGHT_POINT && settings.usePointLights)
            color += point_lighting(cur_light, intersect_pos, closest_primitive, normal, unit_d, texture_color);

        else if(cur_light.type == LightType::LIGHT_DIRECTIONAL && settings.useDirectionalLights)
            color += directional_lighting(cur_light, intersect_pos, closest_primitive, normal, unit_d, texture_color);

        else if(cur_light.type == LightType::LIGHT_SPOT && settings.useSpotLights){

            glm::vec4 light_vec =  intersect_pos-m_light_list.at(i).pos;
            light_vec = glm::normalize(light_vec);
            glm::vec4 R = glm::normalize(2.0f * normal * glm::dot(normal, light_vec) - light_vec);
            glm::vec4 V = -unit_d;


            glm::vec4 diffuse_term(0.0);
            float blend = closest_primitive.material.blend;

            if(settings.useTextureMapping && closest_primitive.material.textureMap.isUsed)
                diffuse_term = m_light_list.at(i).color
                        * (blend * texture_color + (1 - blend) * m_global.kd * closest_primitive.material.cDiffuse )
                        * std::max(0.0f, glm::dot(normal, light_vec));

            else
                diffuse_term = m_light_list.at(i).color * ( m_global.kd * closest_primitive.material.cDiffuse * std::max(0.0f, glm::dot(normal, light_vec)));
              glm::vec4 specular =  m_light_list.at(i).color * m_global.ks * closest_primitive.material.cSpecular * pow(glm::dot(R, V), closest_primitive.material.shininess);
             //n is the specular exponent
            color += diffuse_term + specular;


        }


    }


    return color;
}


glm::vec4 RayScene::estimate_indirect_light(int closest_index, std::pair<float, glm::vec4> closest_data,  glm::vec4 unit_d, glm::vec4 eye, int depth)
{
    float t = closest_data.first;
    glm::vec4 normal = closest_data.second;
    glm::vec4 result(0,0,0,0);
    if(depth == 0)
        return result;
    CS123ScenePrimitive closest_primitive = m_primitive_list.at(closest_index);
    glm::vec4 intersect_pos = eye + t * unit_d;
    glm::vec4 epsilon = 0.001f * normal;

    glm::vec4 reflect_unit = glm::normalize(unit_d - 2.0f * normal * glm::dot(normal, unit_d));
    result += rayTrace(intersect_pos + epsilon, reflect_unit, depth-1) * m_global.ks * closest_primitive.material.cReflective;

   return result;

}



glm::vec4 RayScene::directional_lighting(CS123SceneLightData dir_light,
                                         glm::vec4 intersect_surf,
                                         CS123ScenePrimitive intersect_shape,
                                         glm::vec4 normal,
                                         glm::vec4 sight_vect,
                                         glm::vec4 texture_color)
{
    std::cout<<"In directional_lighting, input texture_color: "<<glm::to_string(texture_color)<<std::endl;
    //not attenuation
    glm::vec4 color(0,0,0,0);
    glm::vec4 oppo_light_dir = glm::normalize(-dir_light.dir);
    std::tuple<float, glm::vec4, int> token;
    glm::vec4 epsilon = 0.001f * normal;
    intersect_surf += epsilon;
    //token = iterate_traverse(intersect_surf, oppo_light_dir);
    if(settings.useKDTree)
        token = tree_traverse(intersect_surf, oppo_light_dir);
    else
        token = iterate_traverse(intersect_surf, oppo_light_dir);
    int closest_index = std::get<2>(token);


    //light hit something, not estimate the light(shadow)
    if(settings.useShadows && closest_index != -1)
        return color;
    glm::vec4 light_vec = oppo_light_dir;
    //std::cout<<"normal: "<<glm::to_string(normal)<<std::endl;
    //std::cout<<"light_vec: "<<glm::to_string(light_vec)<<std::endl;
    //std::cout<<"n dot light: "<<glm::dot(normal, light_vec)<<std::endl;
    glm::vec4 R = glm::normalize(2.0f * normal * glm::dot(normal, light_vec) - light_vec);
    glm::vec4 V = -sight_vect;
    //not hit anything, estimate the light
    glm::vec4 diffuse_term(0.0);
    float blend = intersect_shape.material.blend;
    if(settings.useTextureMapping && intersect_shape.material.textureMap.isUsed){
        diffuse_term = dir_light.color *
                (blend * texture_color +(1 - blend) * m_global.kd *intersect_shape.material.cDiffuse)
               * std::max(0.0f, glm::dot(normal, light_vec));
    }
    else{
        diffuse_term = dir_light.color * ( m_global.kd *intersect_shape.material.cDiffuse * std::max(0.0f, glm::dot(normal, light_vec)));
    }
    //std::cout<<"blend: "<<blend<<std::endl;
    //std::cout<<"In directional_lighting, the diffuse_term : "<<glm::to_string(diffuse_term)<<std::endl;
    glm::vec4 specular_term = dir_light.color *  m_global.ks * intersect_shape.material.cSpecular * pow(glm::dot(R, V), intersect_shape.material.shininess);
    //std::cout<<"In directional_lighting, the specular_term: "<<glm::to_string(specular_term)<<std::endl;
    color += diffuse_term + specular_term;
    //std::cout<<"In directional_lighting, the final color: "<<glm::to_string(color)<<std::endl;

    return color;

}


glm::vec4 RayScene::point_lighting(CS123SceneLightData point_light,
                                   glm::vec4 intersect_surf,
                                   CS123ScenePrimitive intersect_shape,
                                   glm::vec4 normal,
                                   glm::vec4 sight_vect,
                                   glm::vec4 texture_color)
{
    //with attenuation
    glm::vec4 color(0,0,0,0);
    float atten = 1;
    intersect_surf[3] = 1;
    glm::vec4 oppo_light_dir = glm::normalize(point_light.pos - intersect_surf);

    glm::vec4 epsilon = 0.001f * normal;
    intersect_surf += epsilon;
    std::tuple<float, glm::vec4, int> token;

    if(settings.useKDTree)
        token = tree_traverse(intersect_surf, oppo_light_dir);
    else
        token = iterate_traverse(intersect_surf, oppo_light_dir);
    int closest_index = std::get<2>(token);
    float dist = std::get<0>(token);

    //light hit something, not estimate the light(shadow)
    if(settings.useShadows && closest_index != -1 && glm::length(point_light.pos - intersect_surf) > dist)
        return color;

    float light_d = glm::length(point_light.pos - intersect_surf);
    float  atten_token = 1.0f/(point_light.function.x
                           + point_light.function.y * light_d
                           + point_light.function.z * light_d * light_d);
    atten = std::min(1.0f, atten_token);

    glm::vec4 light_vec = oppo_light_dir;

    glm::vec4 R = glm::normalize(2.0f * normal * glm::dot(normal, light_vec) - light_vec);
    glm::vec4 V = -sight_vect;
    //not hit anything, estimate the light
    glm::vec4 diffuse_term(0.0);
   // std::cout<<"normal:"<<glm::to_string(normal)<<std::endl;
   // std::cout<<"light_vec"<<glm::to_string(light_vec)<<std::endl;
   // std::cout<<"dot(normal, light_vdec): "<< glm::dot(normal, light_vec)<<std::endl;
   //std::cout<<"light_pos: "<< glm::to_string(point_light.pos)<<std::endl;
    if(settings.useTextureMapping && intersect_shape.material.textureMap.isUsed)
        diffuse_term = diffuse_term = point_light.color *
                (intersect_shape.material.blend * texture_color +(1 - intersect_shape.material.blend) * m_global.kd *intersect_shape.material.cDiffuse)
                * std::max(0.0f, glm::dot(normal, light_vec));
    else
        diffuse_term = point_light.color * ( m_global.kd *intersect_shape.material.cDiffuse * std::max(0.0f, glm::dot(normal, light_vec)));

    glm::vec4 specular_term = point_light.color * m_global.ks * intersect_shape.material.cSpecular * pow(glm::dot(R, V), intersect_shape.material.shininess);
    color += atten * (diffuse_term +specular_term );
    return color;
}

void RayScene::render(Canvas2D *canvas, Camera* cam)
{

    if(settings.useKDTree)
         m_tree = std::make_shared<OctTree>(m_shapes, m_transformation);
    if(settings.useMultiThreading)
        render_multithread(canvas, cam);
    else
        render_singlethread(canvas, cam);
}


void RayScene::render_singlethread(Canvas2D *canvas, Camera* cam)
{

    std::cout<<"RayScene::render_singlethread(Canvas2D *canvas, Camera* cam), start render"<<std::endl;
    int width = canvas->width();
    int height = canvas->height();
    float width_f = width;
    float height_f = height;
    m_canvas = canvas;
    m_cam = cam;
    RGBA* data = m_canvas->data();
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
            //std::cout<<"p_film: "<<glm::to_string(p_film)<<std::endl;
            //std::cout<<"p_world: "<<glm::to_string(p_world)<<std::endl;
            //std::cout<<"eye_world: "<<glm::to_string(eye_world)<<std::endl;
            //std::cout<<"unit_d: "<<glm::to_string(d)<<std::endl;
            d[3] = 0;
            glm::vec4 token = rayTrace(eye_world, d, reflect_times);
            int index = y * width + x;
            data[index] = RGBA(REAL2byte(token[0]), REAL2byte(token[1]), REAL2byte(token[2]));
            if((int)x % 5 == 0)
            {
                 QCoreApplication::processEvents();
                 m_canvas->update();
            }


        }
    }
    QCoreApplication::processEvents();
    m_canvas->update();
    std::cout<<"# of lights: "<<m_light_list.size()<<std::endl;
    for(auto cur_light:m_light_list){
        std::cout<<"light type: ";
        if(cur_light.type == LightType::LIGHT_AREA)
            std::cout<< "light area" <<std::endl;
        else if(cur_light.type == LightType::LIGHT_POINT)
             std::cout<< "light point" <<std::endl;
        else if(cur_light.type == LightType::LIGHT_SPOT)
            std::cout<< "light spot" <<std::endl;
        else if(cur_light.type == LightType::LIGHT_DIRECTIONAL)
            std::cout<< "light directional" <<std::endl;
    }
    std::cout<<"finish all film piexel"<<std::endl;
    // memcpy(canvas->data(), result, width * height * sizeof(RGBA));
}

void RayScene::render_multithread(Canvas2D *canvas, Camera* cam)
{

    //int width = canvas->width();
    //int height = canvas->height();
    m_cam = cam;
    m_canvas = canvas;
    std::thread top(&RayScene::render_top, this);
    std::thread bot(&RayScene::render_bot, this);
    top.join();
    bot.join();
    std::cout<<"# of lights: "<<m_light_list.size()<<std::endl;
    for(auto cur_light:m_light_list){
        std::cout<<"light type: ";
        if(cur_light.type == LightType::LIGHT_AREA)
            std::cout<< "light area" <<std::endl;
        else if(cur_light.type == LightType::LIGHT_POINT)
             std::cout<< "light point" <<std::endl;
        else if(cur_light.type == LightType::LIGHT_SPOT)
            std::cout<< "light spot" <<std::endl;
        else if(cur_light.type == LightType::LIGHT_DIRECTIONAL)
            std::cout<< "light directional" <<std::endl;
    }
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
    RGBA * data = m_canvas->data();


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
            glm::vec4 token = rayTrace(eye_world, d, reflect_times);
            int index = y * width + x;
            data[index] = RGBA(REAL2byte(token[0]), REAL2byte(token[1]), REAL2byte(token[2]));
            if((int)x % 5 == 0)
            {
                 QCoreApplication::processEvents();
                 m_canvas->update();
            }
        }
    }
    QCoreApplication::processEvents();
    m_canvas->update();
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
     RGBA * data = m_canvas->data();

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
            glm::vec4 token = rayTrace(eye_world, d, reflect_times);
            int index = y * width + x;
            data[index] = RGBA(REAL2byte(token[0]), REAL2byte(token[1]), REAL2byte(token[2]));
            if((int)x % 5 == 0)
            {
                 QCoreApplication::processEvents();
                 m_canvas->update();
            }

        }
    }
    QCoreApplication::processEvents();
    m_canvas->update();
   pthread_exit(NULL);
}


std::pair<float, glm::vec4> RayScene::cube_intersect(glm::mat4x4 obj2world, glm::vec4 eye, glm::vec4 unit_d, float dist)
{
   //  std::cout<<"run cube_intersect"<<std::endl;
    glm::mat4 world2obj = glm::inverse(obj2world);
    glm::vec4 eye_obj = world2obj * eye;
    glm::vec4 d_obj = world2obj * unit_d;
    glm::mat3 transform_token(obj2world);
  //  std::cout<<"transformation: "<<glm::to_string(transformation)<<std::endl;
  //  std::cout<<"transform_token: "<<glm::to_string(transform_token)<<std::endl;
    //std::cout<<"eye_obj: "<<glm::to_string(eye_obj)<<std::endl;
    //std::cout<<"d_obj: "<<glm::to_string(d_obj)<<std::endl;

    //top side y = (0, 1, 0, 0) * transformation
    float t = (0.5 - eye_obj.y)/d_obj.y;
    glm::vec4 intersect_obj = eye_obj + t * d_obj;
    glm::vec4 closest_normal;
    //std::cout<<"top intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
    if(t >= 0 && t < dist && intersect_obj.x <= 0.5  && intersect_obj.x >= -0.5  && intersect_obj.z <= 0.5  &&intersect_obj.z >= -0.5 )
    {
        dist = std::min(dist, t);
        glm::vec3 normal(0,1,0);
        glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
        closest_normal = glm::vec4(token, 0);

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
         closest_normal = glm::vec4(token, 0);
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
         closest_normal = glm::vec4(token, 0);
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
         closest_normal = glm::vec4(token, 0);
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
        closest_normal = glm::vec4(token, 0);

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
         closest_normal = glm::vec4(token, 0);
    }
    closest_normal = glm::normalize(closest_normal);
    std::pair<float, glm::vec4> ans(dist, closest_normal);

    return ans;
}


std::pair<float, glm::vec4> RayScene::cone_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d,  float dist_)
{
  //   std::cout<<"run cone_intersect"<<std::endl;
    float dist = dist_;
    glm::vec4 eye_obj = glm::inverse(transformation) * eye;
    glm::vec4 d_obj = glm::inverse(transformation) * unit_d;
    glm::vec4 P = eye_obj;
    glm::vec4 d = d_obj;
    glm::vec4 intersect_obj;
    glm::mat3 transform_token(transformation);
    glm::vec4 closest_normal(0.0f);
   // std::cout<<"transformation: "<<glm::to_string(transformation)<<std::endl;
  //  std::cout<<"transform_token: "<<glm::to_string(transform_token)<<std::endl;


    //cone cap intersect check
    float t = (-0.5 - P.y)/d.y;
    intersect_obj = P + t * d;
    float x = intersect_obj.x;
    float z = intersect_obj.z;
    if(t >= 0 && t < dist && pow(x, 2) + pow(z, 2) <= pow(0.5, 2))
    {

         dist = std::min(dist, t);
         std::cout<<"dist "<<dist<<std::endl;
         glm::vec3 normal(0,-1,0);
         glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
         closest_normal = glm::vec4(token, 0);

    }
    //cone body intersect check
    float A = pow(d.x, 2) + pow(d.z, 2) - 0.25*pow(d.y, 2);
    float B = 2*P.x*d.x + 2*P.z*d.z - 0.5*P.y*d.y +0.25*d.y;
    float C = pow(P.x, 2) + pow(P.z, 2) - 0.25*pow(P.y, 2) + 0.25*P.y - 1.f/16.f;
    if(pow(B,2) - 4 * A * C >= 0)
    {
       float t1 = (-B + sqrt(pow(B,2) - 4*A*C))/(2*A);
       float t2 = (-B - sqrt(pow(B,2) - 4*A*C))/(2*A);
       //float t_comp = std::min(t1, t2);
       if(t1 >= 0 && t1 < dist ){
            intersect_obj = P + t1 * d;
      //       std::cout<<"Cone body: intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;

            if(intersect_obj.y < 0.5 && intersect_obj.y > -0.5){
                 dist = std::min(dist, t1);
                 std::cout<<"dist "<<dist<<std::endl;
                 glm::vec3 normal(2*intersect_obj.x, 0.5*intersect_obj.y - 0.25, 2*intersect_obj.z);

                 glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
                 //std::cout<<"t1 Cone body: intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
                 //std::cout<<"t1 Cone body: normal"<<glm::to_string(normal)<<std::endl;
                 //std::cout<<"t1 Cone body: normal 's obj2wrold"<<glm::to_string(glm::inverse(glm::transpose(transform_token)))<<std::endl;
                 closest_normal = glm::vec4(token, 0);

            }
       }

       if(t2 >= 0 && t2 < dist)
       {
           intersect_obj = P + t2 * d;
       //    std::cout<<"Cone body: intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
           if(intersect_obj.y < 0.5 && intersect_obj.y > -0.5){

               dist = std::min(dist, t2);
               glm::vec3 normal(2*intersect_obj.x, 0.5*intersect_obj.y - 0.25, 2*intersect_obj.z);
               //std::cout<<"t2 Cone body: intersect_obj: "<<glm::to_string(intersect_obj)<<std::endl;
               //std::cout<<"t2 Cone body: normal"<<glm::to_string(normal)<<std::endl;
               //std::cout<<"t2 Cone body: normal 's obj2wrold"<<glm::to_string(glm::inverse(glm::transpose(transform_token)))<<std::endl;

               glm::vec3 token = glm::inverse(glm::transpose(transform_token)) * normal;
               closest_normal = glm::vec4(token, 0);

           }
       }

    }

    closest_normal = glm::normalize(closest_normal);
    std::pair<float, glm::vec4> ans(dist, closest_normal);

    return ans;

}

std::pair<float, glm::vec4> RayScene::cylinder_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d,  float dist)
{
 //   std::cout<<"run cylinder_intersect"<<std::endl;
    //float dist = FLT_MAX;
    glm::vec4 P= glm::inverse(transformation) * eye;
    glm::vec4 d = glm::inverse(transformation) * unit_d;
    glm::vec4 intersect_obj;
    glm::mat3 transform_token(transformation);
     glm::vec4 closest_normal;
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
         closest_normal = glm::vec4(token, 0);
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
        closest_normal = glm::vec4(token, 0);
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
                 closest_normal = glm::vec4(token, 0);

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
                closest_normal = glm::vec4(token, 0);
            }
        }
    }
     closest_normal = glm::normalize(closest_normal);
   std::pair<float, glm::vec4> ans(dist, closest_normal);

    return ans;

}

std::pair<float, glm::vec4> RayScene::sphere_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d,  float dist)
{
  //   std::cout<<"run sphere_intersect"<<std::endl;
   // float dist = FLT_MAX;
    glm::vec4 P= glm::inverse(transformation) * eye;
    glm::vec4 d = glm::inverse(transformation) * unit_d;
    glm::vec4 intersect_obj;
    glm::mat3 transform_token(transformation);
     glm::vec4 closest_normal;
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
                 closest_normal = glm::vec4(token, 0);
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
                closest_normal = glm::vec4(token, 0);
            }
        }
    }
     closest_normal = glm::normalize(closest_normal);
    std::pair<float, glm::vec4> ans(dist, closest_normal);

    return ans;
}
glm::vec2 RayScene::map2cube(glm::vec4 intersect_surf_world, glm::mat4x4 obj2world)
{
    //intersect_surf
    glm::vec4 intersect_surf_obj = glm::inverse(obj2world) * intersect_surf_world;
    std::cout<<"intersect_surf_obj: "<<glm::to_string(intersect_surf_obj)<<std::endl;
    glm::vec2 uv(0.0f);
    //std::cout<<"intersect_surf_obj: "<<glm::to_string(intersect_surf_obj)<<std::endl;
    //std::cout<<"intersect_surf_obj.yz()"<<glm::to_string(intersect_surf_obj.yz())<<std::endl;
    //top
    if(abs(intersect_surf_obj.x - 0.5) <= 0.0001)
    {  // (0,1) - (0.5,0.5) = (-0.5,0.5) ->(1,0)
       // (1,0) - (0.5,0.5) = (0.5,-0.5) ->(0,1)
        uv[0] = intersect_surf_obj.z * -1 + 0.5;
        uv[1] = intersect_surf_obj.y * -1 + 0.5;
       //uv = intersect_surf_obj.yz() + glm::vec2(0.5, 0.5);
       // uv = glm::vec2(0,0);
    }
    //bot
    else if(abs(intersect_surf_obj.x + 0.5) <= 0.0001)
    {
        uv[0] = 1-(intersect_surf_obj.z * -1 + 0.5);
        uv[1] =  intersect_surf_obj.y * -1 + 0.5;
     //    uv = glm::vec2(0,0);
    }

    else if(abs(intersect_surf_obj.y - 0.5) <= 0.0001)
    {
        //left top (0.5,0.5) ->(1, 1)
       uv = intersect_surf_obj.xz() + glm::vec2(0.5, 0.5);
        // uv = glm::vec2(0,0);
    }
    //bot
    else if(abs(intersect_surf_obj.y + 0.5) <= 0.0001)
    {
         //(0,1) - (0.5,0.5) = (-0.5, 0.5) ->(1, 1)
        //(1,1) - (0.5,0.5) = (0.5, 0.5) ->(1, 0)
      // uv = glm::vec2(0.5, 0.5) + intersect_surf_obj.xz() ;
        uv[0] = intersect_surf_obj.x + 0.5;
        uv[1] = 1 - (intersect_surf_obj.z + 0.5);
       //uv = intersect_surf_obj.xz() + glm::vec2(0.5, 0.5);
    }

    else if(abs(intersect_surf_obj.z - 0.5) <= 0.0001)
    {
       // (0.5,0.5) ->(1, 0)
        uv[0] = intersect_surf_obj.x + 0.5;
        uv[1] = 1 - (intersect_surf_obj.y + 0.5);

    }
    else if(abs(intersect_surf_obj.z + 0.5) <= 0.0001)
    {
       //uv = glm::vec2(0.5, 0.5) + intersect_surf_obj.xy() ;
        uv[0] = 1-(intersect_surf_obj.x + 0.5);
        uv[1] = 1-(intersect_surf_obj.y + 0.5);

    }
    return uv;

}

glm::vec2 RayScene::map2cylinder(glm::vec4 intersect_surf_world, glm::mat4x4 obj2world)
{
    //intersect_surf
    glm::vec4 intersect_surf_obj = glm::inverse(obj2world) * intersect_surf_world;
    glm::vec2 uv(0.0f);
    //cap
    if(abs(intersect_surf_obj.y - 0.5) <= 0.0001)
    {
       uv = intersect_surf_obj.xz() + glm::vec2(0.5, 0.5);
        // uv = glm::vec2(0,0);
    }
    //bot
    else if(abs(intersect_surf_obj.y + 0.5) <= 0.0001)
    {
        uv[0] = intersect_surf_obj.x + 0.5;
        uv[1] = 1 - (intersect_surf_obj.z + 0.5);
    }

     // on body
    else
    {
         uv[1] =  0.5 - intersect_surf_obj.y;
        float theta = atan2(intersect_surf_obj.z, intersect_surf_obj.x);
        if(theta < 0)
            uv[0] = -theta/(2.0f * M_PI);
        else
            uv[0] = 1-(theta/(2.0f * M_PI));
    }

    return uv;
}

glm::vec2 RayScene::map2cone(glm::vec4 intersect_surf_world, glm::mat4x4 obj2world)
{
    //intersect_surf
    glm::vec4 intersect_surf_obj = glm::inverse(obj2world) * intersect_surf_world;
    glm::vec2 uv(0.0f);
    //cap
    if(abs(abs(intersect_surf_obj.y) - 0.5) <= 0.0001)
    {
        uv[0] = intersect_surf_obj.x + 0.5;
        uv[1] = 1 - (intersect_surf_obj.z + 0.5);
    }
     // on body
    else
    {
        uv[1] =  0.5 - intersect_surf_obj.y;
        float theta = atan2(intersect_surf_obj.z, intersect_surf_obj.x);
        if(theta < 0)
            uv[0] = -theta/(2.0f * M_PI);
        else
            uv[0] = 1-(theta/(2.0f * M_PI));
    }

    return uv;
}


glm::vec2 RayScene::map2sphere(glm::vec4 intersect_surf_world, glm::mat4x4 obj2world)
{
    std::cout<<"start running map2sphere"<<std::endl;
    //intersect_surf
    glm::vec4 intersect_surf_obj = glm::inverse(obj2world) * intersect_surf_world;
    glm::vec2 uv(0.0f);


        float phi = asin(intersect_surf_obj.y/0.5);
        float v = phi/M_PI + 0.5;
        uv[1] = v;
        if(v == 0 || v == 1)
            uv[0] = 0.5;
        else{
            uv[1] =  0.5 - intersect_surf_obj.y;
            float theta = atan2(intersect_surf_obj.z, intersect_surf_obj.x);
            if(theta < 0)
                uv[0] = -theta/(2.0f * M_PI);
            else
                uv[0] = 1-(theta/(2.0f * M_PI));
        }

    return uv;
}

glm::vec4 RayScene::textureMap(glm::vec2 uv, int shape_index)
{
    std::cout<<"start running textureMap(glm::vec2 uv, CS123SceneMaterial mater)"<<std::endl;
    glm::vec4 color(0.0f);
    CS123SceneMaterial mater = m_shapes.at(shape_index).material;
    QImage image = m_tetxures.at(shape_index);
    QSize image_size = image.size();
    std::cout<<"textMap place 2, is null: "<<image.isNull()<<std::endl;
    int height = image_size.height();
    int width = image_size.width();

    std::cout<<"textMap place 3, with width: "<<width<<"height: "<<height<<std::endl;
    float u = uv[0], v = uv[1];
    std::cout<<"textMap place 4: u: "<<u<<"v"<<v<<std::endl;
    int s = ((int) (u * mater.textureMap.repeatU * width)) % width;


     std::cout<<"textMap place 5"<<std::endl;
    int t = ((int) (v * mater.textureMap.repeatV * height)) % height;

    std::cout<<"textMap place 6, s: "<<s<<" t: "<<t<<std::endl;
    //image.pixelColor(s,t);
    QColor cur_color = image.pixelColor(s,t);

    float red = cur_color.red()/255.0f;
    float green = cur_color.green()/255.0f;
    float blue = cur_color.blue()/255.0f;
    glm::vec4 text_color(red,green, blue,0);
     std::cout<<"textMap place 7, output color:"<<glm::to_string(text_color)<<std::endl;
    return text_color;
   // return glm::vec4(0.0);
}


unsigned char RayScene::REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}


