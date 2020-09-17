/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

#include <math.h>
#include <iostream>

LinearBrush::LinearBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    m_color = color;
    m_radius = radius;
    color_Brush = true;
    makeMask();
}


LinearBrush::~LinearBrush()
{
}

void LinearBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Linear brush here...

    float r_sqr = pow(m_radius,2);
    int R = m_radius;
    for(int row = m_radius; row >= -m_radius; row--)
    {
        for(int col = -m_radius; col <= m_radius; col++)
        {
            float distance = pow(row, 2) + pow(col, 2);
            if(distance >= r_sqr)
                m_mask.push_back(0);
            else
            {

              float r = m_radius;
              float dist_sqrt = sqrt(distance);
              float token = (r-dist_sqrt)/r;
               m_mask.push_back(token);

            }
        }
    }


}


