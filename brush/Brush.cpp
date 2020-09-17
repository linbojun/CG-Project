/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(RGBA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_radius(radius)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

RGBA Brush::getRGBA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setRGBA(const RGBA &rgba) {
    m_color = rgba;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    makeMask();
}




void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.
    //
    // Example: You'll want to delete or comment out this code, which
    // sets all the pixels on the canvas to red.
    //

    RGBA *pix = canvas->data();

    // Return an array containing the rows of the image one after another, starting from the top.
    // Each row is width() wide and there are height() rows, so this array will have
    // width() * height() elements.
    /*
    int size = canvas->width() * canvas->height();
    for (int i = 0; i < size; i++) {
        pix[i] = RGBA(255, 0, 0, 255);
    }
    */
    int w = canvas->width();
    int start_row = std::max(0, mouseY-m_radius);
    int end_row =  std::min(canvas->height(), mouseY+m_radius);
    int start_col = std::max(0, mouseX-m_radius);
    int end_col = std::min(canvas->width(), mouseX+m_radius);
    int index = 0;
    float alpha_255 = m_color.a;
    float alpha = alpha_255/255;
    for(int i = mouseY-m_radius; i<= mouseY+m_radius; i++)
    {
        for(int j = mouseX-m_radius; j <=  mouseX+m_radius; j++)
        {
            if(i< start_row || i > end_row || j < start_col ||j > end_col )
            {
                index++;
                continue;
            }
            RGBA pix_color = pix[i*w+j];
            float msk = m_mask.at(index++);

            float red = msk * alpha * m_color.r + (1-msk*alpha)*pix_color.r;
   //
           float green = msk * alpha * m_color.g + (1-msk*alpha)*pix_color.g;
           float blue = msk * alpha * m_color.b + (1-msk*alpha)*pix_color.b;
            pix[i*w+j] = RGBA(red, green, blue, 255);
        }
    }


    canvas->update();

}
