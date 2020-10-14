#ifndef SCALE_H
#define SCALE_H
#include "Filter.h"

class FilterScaling : public Filter{
public:
    FilterScaling(float x, float y);
    virtual ~FilterScaling() override;
    virtual void apply(Canvas2D *canvas) override;


private:
    float _x, _y;
    int new_width, new_height;
   // RGBA conv_x_cell(std::vector<float> r_c, std::vector<float> g_c, std::vector<float> b_c, int pos);
    RGBA conv_x_cell(Canvas2D* canvas, int x_pos, int y_pos);
    RGBA conv_y_cell(Canvas2D* canvas, int x_pos, int y_pos);

    void conv_x(Canvas2D* canvas);
    void conv_y(Canvas2D* canvas);

};

#endif
