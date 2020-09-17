/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"
#include <vector>


SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!
    m_radius = radius;
    m_color = color;

    color_Brush = false;

    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{

    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.

}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //
    float r_sqr = pow(m_radius,2);
    int R = m_radius;
    int mask_size = 0;
    for(int row = m_radius; row >= -m_radius; row--)
    {
        for(int col = -m_radius; col <= m_radius; col++)
        {
            mask_size++;
            float distance = pow(row,2) + pow(col,2);
            float dist_sqrt = sqrt(distance);
            if(distance >= r_sqr)
                m_mask.push_back(0);
           else
            {
                float c = 1;
                float b = 0;
                float a = -1/r_sqr;
                m_mask.push_back(a*distance+b*dist_sqrt+c);
            }

        }
    }
}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {
    int width = canvas->width();
    int height = canvas->height();
    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //
    buffer_ptr = std::make_unique<std::vector<std::shared_ptr<RGBA>>>();
     RGBA *pix = canvas->data();
    int start_row = std::max(0, y-m_radius);
    int end_row =  std::min(height, y+m_radius);
    int start_col = std::max(0, x-m_radius);
    int end_col = std::min(width, x+m_radius);
    for(int i = y-m_radius; i<= y+m_radius; i++)
    {
        for(int j = x-m_radius; j <=  x+m_radius; j++)
        {
            if(i< start_row || i > end_row || j < start_col ||j > end_col )
            {
                std::shared_ptr<RGBA> token = std::make_shared<RGBA>();
                buffer_ptr->push_back(token);

            }
            else
            {
                RGBA cur_pix = pix[i*width+j];
                std::shared_ptr<RGBA> token = std::make_shared<RGBA>(cur_pix.r, cur_pix.g, cur_pix.b, cur_pix.a);
                buffer_ptr->push_back(token);
            }

        }
    }


}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    // now pick up paint again...
    RGBA *pix = canvas->data();
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
            float msk = m_mask.at(index);
            std::shared_ptr<RGBA> cur_paint = buffer_ptr->at(index++);
            float red = msk * alpha * cur_paint->r + (1-msk*alpha)*pix_color.r;
   //
           float green = msk * alpha * cur_paint->g + (1-msk*alpha)*pix_color.g;
           float blue = msk * alpha * cur_paint->b + (1-msk*alpha)*pix_color.b;
            pix[i*w+j] = RGBA(red, green, blue, 255);

        }
    }
    pickUpPaint(mouseX, mouseY, canvas);

}


