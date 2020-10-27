#ifndef SCENE_H
#define SCENE_H

#include "shapes/Shape.h"
#include "CS123SceneData.h"
#include <QList>

class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    static glm::mat4x4 customization(CS123SceneTransformation* T);


    static void traverTree(CS123SceneNode *root, glm::mat4x4 T, Scene *sceneToFill);


protected:




    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);



    CS123SceneGlobalData m_global;
    QList<CS123SceneLightData> m_light_list;
    QList<glm::mat4x4> m_transformation_list;
    QList<CS123ScenePrimitive> m_primitive_list;



};

#endif // SCENE_H
