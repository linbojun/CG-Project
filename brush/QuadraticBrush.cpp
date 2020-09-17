/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>
#include <iostream>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    int token_R = radius;
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    m_color = color;

    color_Brush = true;
    m_radius = radius;

    int token = 0;
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Quadratic brush here...

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


