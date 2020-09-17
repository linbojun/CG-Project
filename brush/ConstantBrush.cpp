/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"
#include <math.h>

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    m_color = color;
    m_radius = radius;
    color_Brush = true;
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...
    int row = -m_radius;
    int col = -m_radius;
    int r_sqr = pow(m_radius, 2);
    for(row = m_radius; row >= -m_radius; row--)
    {
        for(col = -m_radius; col <= m_radius; col++)
        {
            int distance = pow(row, 2) + pow(col, 2);
            if(distance >= r_sqr)
                m_mask.push_back(0);
            else
                m_mask.push_back(1);
        }
    }


}



