#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "Canvas2D.h"
#include "Camera.h"


#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);

    virtual ~RayScene();

    void render(Canvas2D *canvas, Camera* cam);

    void render_multithread(Canvas2D *canvas, Camera* cam);

private:
    RGBA rayTrace(glm::vec4 eye, glm::vec4 unit_d);

    void render_top();

    void render_bot();



    float cube_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist);

    float cone_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist);

    float cylinder_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist);

    float sphere_intersect(glm::mat4x4 transformation, glm::vec4 eye, glm::vec4 unit_d, float dist);

    glm::vec4 m_closest_normal;

    unsigned char REAL2byte(float f);

    Canvas2D* m_canvas;
    Camera* m_cam;
    RGBA*m_result;





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
