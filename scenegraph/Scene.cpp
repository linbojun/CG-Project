#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"


Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

    //deal with global data
    // virtual bool getGlobalData(CS123SceneGlobalData& data) const = 0;
    CS123SceneGlobalData token_global;
    parser->getGlobalData(token_global);
    sceneToFill->setGlobal(token_global);

    // virtual bool getLightData(const int i, CS123SceneLightData& data) const = 0;

    int numLight = parser->getNumLights();
    for(int i = 0; i < numLight; i++)
    {
        CS123SceneLightData token_light;
        parser->getLightData(i, token_light);
        sceneToFill->addLight(token_light);

    }

    CS123SceneNode* root = parser->getRootNode();

    glm::mat4x4 identity = glm::mat4x4(1,   0,  0,  0,
                                       0,   1,  0,  0,
                                       0,   0,  1,  0,
                                       0,   0,  0,  1);
    traverTree(root, identity, sceneToFill);


}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    CS123ScenePrimitive &prim = const_cast<CS123ScenePrimitive &>(scenePrimitive);
    glm::mat4x4 &mat = const_cast<glm::mat4x4 &>(matrix);
    m_primitive_list.append(prim);
    m_transformation_list.append(mat);

}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_light_list.append(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
}

void Scene::traverTree(CS123SceneNode *root, glm::mat4x4 T, Scene *sceneToFill)
{
    glm::mat4x4 complex = T;


    //iterate through all transformations
    for(int i = 0; i < root->transformations.size(); i++)
    {
        glm::mat4x4 token = customization(root->transformations.at(i));
        complex = complex * token;
    }

    //iterate through all primitives in node, and add to scene
    for(int i = 0; i < root->primitives.size(); i++)
    {
        sceneToFill->addPrimitive(*root->primitives.at(i),
                                  complex);
    }

    //iterate all children
    for(int i = 0; i < root->children.size(); i++)
    {
        traverTree(root->children.at(i),
                   complex,
                   sceneToFill);
    }
    return;


}



glm::mat4x4 Scene::customization(CS123SceneTransformation* T)
{
    glm::mat4x4 ans;
    //TRANSFORMATION_TRANSLATE, TRANSFORMATION_SCALE, TRANSFORMATION_ROTATE, TRANSFORMATION_MATRIX
    if(T->type == TRANSFORMATION_TRANSLATE)
    {
        glm::vec3 translate = T->translate;
        glm::mat4 token = glm::mat4(1, 0, 0, translate.x,
                                    0, 1, 0, translate.y,
                                    0, 0, 1, translate.z,
                                    0, 0, 0, 1);
       //ans->type = TRANSFORMATION_MATRIX;
       ans = glm::transpose(token);



    }
    else if(T->type == TRANSFORMATION_SCALE)
    {
        glm::vec3 scale = T->scale;
        glm::mat4x4 token = glm::mat4x4(scale.x,    0,      0,      0,
                                        0,       scale.y,   0,      0,
                                        0,          0,    scale.z,  0,
                                        0,          0,      0,      1);
        //ans->type = TRANSFORMATION_MATRIX;
        ans =glm::transpose(token);

    }
    else if(T->type == TRANSFORMATION_ROTATE)
    {
        float rad = T->angle;
        glm::vec3 rotate = T->rotate;
        glm::mat4x4 token;
        if(rotate.x == 1)
        {
          token = glm::mat4(1,    0,                0,              0,
                            0,    glm::cos(rad),    -glm::sin(rad), 0,
                            0,    glm::sin(rad),     glm::cos(rad), 0,
                            0,    0,                 0,             1);
        }
        else if(rotate.y == 1)
        {
            token = glm::mat4(glm::cos(rad),  0,   glm::sin(rad),   0,
                              0,              1,   0,               0,
                              -glm::sin(rad), 0,   glm::cos(rad),   0,
                              0,              0,   0,               1);
        }
        else
        {
            token = glm::mat4(glm::cos(rad),  -glm::sin(rad),   0,   0,
                              glm::sin(rad),  glm::cos(rad),    0,   0,
                              0,              0,                1,   0,
                              0,              0,                0,   1);
        }
        //ans->type = TRANSFORMATION_MATRIX;
        ans =glm::transpose(token);

    }
    else
    {
        //ans->type = T->type;
        ans = T->matrix;
    }
    return ans;

}
