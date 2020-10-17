#include "FilterGray.h"
#include "Canvas2D.h"
#include <iostream>

unsigned char FilterGray::RGBAToGray(const RGBA &pixel) {

    float R = (float)pixel.r/255.0;
    float G = (float)pixel.g/255.0;
    float B = (float)pixel.b/255.0;
    float Y = 0.299*R + 0.587*G + 0.114*B;
    Y =  REAL2byte(Y);
   // std::cout<<"Y: "<<Y<<std::endl;
    return Y;

}

FilterGray::~FilterGray()
{
}

void FilterGray::apply(Canvas2D *canvas) {
    int width = canvas->width();

    RGBA* current_pixel = nullptr;
    RGBA* current_row = canvas->data();

    RGBA* data = canvas->data();
    size_t currentIndex = 0;

    for (int r = 0; r < canvas->height(); r++) {
        current_pixel = current_row;
        currentIndex = r * width;

        for (int c = 0; c < canvas->width(); c++) {

            unsigned char gray = RGBAToGray(*current_pixel);

            current_pixel->r = gray;
            current_pixel->g = gray;
            current_pixel->b = gray;

            /* Advance to the next pixel */
            current_pixel++;
            currentIndex++;
        }
        current_row += width;
    }
}

