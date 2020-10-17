
#ifndef BLUR_H
#define BLUR_H


#include "Filter.h"

class FilterBlur : public Filter {
public:
    FilterBlur(int r);
    virtual ~FilterBlur() override;
    virtual void apply(Canvas2D *canvas) override;


private:
    void box_conv(Canvas2D* canvas);

    void box_conv_row(Canvas2D* canvas);
    void box_conv_col(Canvas2D* canvas);
    void initializeFilter();
    std::vector<float> m_kernel_x;
    //std::vector<float> m_kernel_y;
   // std::vector<float> m_kernel;


    int radius;
};


#endif
