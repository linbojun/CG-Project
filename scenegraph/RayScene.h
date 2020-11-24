#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "Canvas2D.h"
#include "Camera.h"
#include "OctTree.h"


#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */

/*
struct t_normal{
    float t;
    glm::vec4 normal;

};
*/

class RayScene : public Scene {
public:
    RayScene(Scene &scene);

    virtual ~RayScene();

    void render(Canvas2D *canvas, Camera* cam);

    void render_singlethread(Canvas2D *canvas, Camera* cam);

    void render_multithread(Canvas2D *canvas, Camera* cam);

    static std::pair<float, glm::vec4> cube_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist);

    static std::pair<float, glm::vec4> cone_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist);

    static std::pair<float, glm::vec4> cylinder_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist);

    static std::pair<float, glm::vec4> sphere_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist);

private:
    glm::vec4 rayTrace(glm::vec4 eye, glm::vec4 unit_d, int depth);

    void render_top();

    void render_bot();

    glm::vec4 estimate_direct_light(int closest_index, std::pair<float, glm::vec4> closest_data,  glm::vec4 unit_d, glm::vec4 eye);

    glm::vec4 estimate_indirect_light(int closest_index, std::pair<float, glm::vec4> closest_data,  glm::vec4 unit_d, glm::vec4 eye, int depth);

    std::tuple<float, glm::vec4, int> tree_traverse(glm::vec4 eye, glm::vec4 unit_d);

    std::tuple<float, glm::vec4, int> iterate_traverse(glm::vec4 eye, glm::vec4 unit_d);

    glm::vec4 directional_lighting(CS123SceneLightData dir_light,
                                             glm::vec4 intersect_surf,
                                             CS123ScenePrimitive intersect_shape,
                                             glm::vec4 normal,
                                             glm::vec4 sight_vect,
                                             glm::vec4 texture_color);

    glm::vec4 point_lighting(CS123SceneLightData point_light,
                                       glm::vec4 intersect_surf,
                                       CS123ScenePrimitive intersect_shape,
                                       glm::vec4 normal,
                                       glm::vec4 sight_vec,
                                       glm::vec4 texture_color);

    glm::vec2 map2cube(glm::vec4 intersect_surf, glm::mat4x4 obj2world);

    glm::vec2 map2cylinder(glm::vec4 intersect_surf_world, glm::mat4x4 obj2world);

    glm::vec2 map2cone(glm::vec4 intersect_surf_world, glm::mat4x4 obj2world);

    glm::vec2 map2sphere(glm::vec4 intersect_surf_world, glm::mat4x4 obj2world);

    glm::vec4 textureMap(glm::vec2 uv, int shape_index);



    //glm::vec4 m_closest_normal;



    unsigned char REAL2byte(float f);

    Canvas2D* m_canvas;
    Camera* m_cam;
    std::shared_ptr<OctTree> m_tree;
   // RGBA*m_result;

    const int reflect_times = 2;

    std::vector<CS123ScenePrimitive> m_shapes;
    std::vector<glm::mat4x4> m_transformation;
    std::vector<QImage> m_tetxures;

    /*
     *
     *  CS123SceneGlobalData m_global;
     *  QList<CS123SceneLightData> m_light_list;
    *   QList<glm::mat4x4> m_transformation_list;
    *   QList<CS123ScenePrimitive> m_primitive_list;
    *
    *  PRIMITIVE_CUBE,
        PRIMITIVE_CONE,
        PRIMITIVE_CYLINDER,
         PRIMITIVE_SPHERE,
    */

};

#endif // RAYSCENE_H
