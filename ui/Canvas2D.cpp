/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"

#include <QCoreApplication>
#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "filter/FilterSobel.h"
#include "filter/FilterBlur.h"
#include "filter/FilterScaling.h"
#include "SceneviewScene.h"

Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr)
{
    if(settings.brushType == BRUSH_CONSTANT)
        m_brush = std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
    //    m_brush = new ConstantBrush(settings.brushColor, settings.brushRadius);
    else if(settings.brushType == BRUSH_LINEAR)
         m_brush = std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
    else if(settings.brushType == BRUSH_SMUDGE)
        m_brush = std::make_unique<SmudgeBrush>(settings.brushColor, settings.brushRadius);
    else if(settings.brushType == BRUSH_QUADRATIC)
         m_brush = std::make_unique<QuadraticBrush>(settings.brushColor, settings.brushRadius);
    else
         m_brush = std::make_unique<QuadraticBrush>(settings.brushColor, settings.brushRadius);


}

Canvas2D::~Canvas2D()
{

}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);

}

void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.

    //If the case of change color of smudge brush
    if(m_brush->color_Brush ==  false && settings.brushType == BRUSH_SMUDGE
            && settings.brushRadius == m_brush->getRadius())
        return;
    if(settings.brushType == BRUSH_CONSTANT)
        m_brush = std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
    //    m_brush = new ConstantBrush(settings.brushColor, settings.brushRadius);
    else if(settings.brushType == BRUSH_LINEAR)
         m_brush = std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
    else if(settings.brushType == BRUSH_SMUDGE)
        m_brush = std::make_unique<SmudgeBrush>(settings.brushColor, settings.brushRadius);
    else if(settings.brushType == BRUSH_QUADRATIC)
         m_brush = std::make_unique<QuadraticBrush>(settings.brushColor, settings.brushRadius);



    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel
    m_brush->brushDown(x, y, this);

    std::cout << "Canvas2d::mouseDown() called" << std::endl;


//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
     m_brush->brushDragged(x, y, this);

    std::cout << "Canvas2d::mouseDragged() called" << std::endl;

}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
     m_brush->brushUp(x, y, this);
    std::cout << "Canvas2d::mouseUp() called" << std::endl;
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    switch(settings.filterType) {
        case FILTER_EDGE_DETECT:
            m_filter = std::make_unique<FilterSobel>(settings.edgeDetectSensitivity);
            break;
        case FILTER_BLUR:
            m_filter = std::make_unique<FilterBlur>(settings.blurRadius);
            break;
        case FILTER_SCALE:
            m_filter = std::make_unique<FilterScaling>(settings.scaleX, settings.scaleY);
            // fill in the rest
    }
    m_filter->apply(this);
    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    std::cout<<"running set Scene"<<std::endl;

    m_rayScene.reset(scene);
   // m_rayScene = std::make_unique<RayScene>(scene);
   std::cout<<"finsih set Scene"<<std::endl;
}

void Canvas2D::renderImage(Camera *camera, int width, int height) {
     std::cout<<"running Canvas2D::renderImage"<<std::endl;
    if (m_rayScene) {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        this->resize(width, height);
        if(settings.useMultiThreading == true){
            m_rayScene->render_multithread(this, camera);
        }
        else{
            m_rayScene->render(this, camera);
        }


        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.
         //QCoreApplication::processEvents();

    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}




