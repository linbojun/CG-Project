#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "CS123SceneData.h"
#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"

#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"
#include "shapes/Sphere.h"
#include "shapes/Shape.h"
using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();

   // m_scene = std::make_shared<Scene>();

    //----------------shapeScene-----------
   // initializeSceneLight();
  // initializeSceneMaterial();
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::render(SupportCanvas3D *context) {
    /*
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms(context);
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();
    */
    //----------------------
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_phongShader->bind();
    setSceneUniforms(context);
    //setLights(context->getCamera()->getViewMatrix());
    setLights();
    setMatrixUniforms(m_phongShader.get(), context);
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}


void SceneviewScene::setLights()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //
    clearLights();
   // m_phongShader->setLight(m_light);
    int num_lights = m_light_list.size();
    for(int i = 0; i < num_lights; i++)
    {
        m_phongShader->setLight(m_light_list.at(i));
    }

}



void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //

    std::cout<<"running SceneviewScene::renderGeometry() "<<std::endl;



    int num_shape = m_shape_list.size();
    if(num_shape == 0)
       settingsChanged();
        //return;

    for(int i = 0; i < num_shape; i++)
    {
        glm::mat4x4 token_trans =  m_transformation_list.at(i);
        m_phongShader->setUniform("m", m_transformation_list.at(i));
        std::cout << glm::to_string(token_trans) << std::endl<<std::endl;

        m_phongShader->applyMaterial(m_material_list.at(i));
        m_shape_list.at(i)->draw();
    }


}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.

    std::cout<<"Running Sceneview::settingChange()"<<std::endl;

    if(m_primitive_list.size() == 0)
        return;
    int num_prim = m_primitive_list.size();
    m_shape_list.clear();
    m_material_list.clear();
    int param1 = settings.shapeParameter1;
    int param2 = settings.shapeParameter2;
    for(int i = 0; i < num_prim; i++)
    {
        //deal with shape
        if(m_primitive_list.at(i).type == PrimitiveType::PRIMITIVE_CUBE)
        {
            std::shared_ptr<Shape> cube_ptr = std::make_shared<Cube>(param1);
            m_shape_list.push_back(cube_ptr);
        }

        else if(m_primitive_list.at(i).type == PrimitiveType::PRIMITIVE_CONE)
        {
            std::shared_ptr<Shape> cone_ptr = std::make_shared<Cone>(param1, param2);
            m_shape_list.push_back(cone_ptr);
        }
        else if(m_primitive_list.at(i).type == PrimitiveType::PRIMITIVE_SPHERE)
        {
            std::shared_ptr<Shape> sphere_ptr = std::make_shared<Sphere>(param1, param2);
            m_shape_list.push_back(sphere_ptr);
        }
        else if(m_primitive_list.at(i).type == PrimitiveType::PRIMITIVE_CYLINDER)
        {
            std::shared_ptr<Cylinder> cylinder_prt = std::make_shared<Cylinder>(param1, param2);
           m_shape_list.push_back(cylinder_prt);
        }

        //deal with material
       CS123SceneMaterial token = m_primitive_list.at(i).material;
       token.cAmbient *= m_global.ka;
       token.cDiffuse *= m_global.kd;
       token.cSpecular*= m_global.ks;
        m_material_list.push_back(token);


    }


}


//-----------------------------shape Scene--------------------------------------------




void SceneviewScene::clearLights() {
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
       // std::ostringstream os;
      //  os << i;
        std::string indexString = "[" + std::to_string(i) + "]"; // e.g. [0], [1], etc.
        m_phongShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

