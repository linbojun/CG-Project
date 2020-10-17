#ifndef SOBEL_H
#define SOBEL_H

#include "Filter.h"
#include "FilterGray.h"

class FilterSobel : public Filter{
private:
    float m_sense;
    std::vector<float> m_kernel_x_row;
    std::vector<float> m_kernel_x_col;
    std::vector<float> m_kernel_y_row;
    std::vector<float> m_kernel_y_col;
    std::vector<float> m_kernel_x;
    std::vector<float> m_kernel_y;
    FilterGray grayFilter;
    //void update_filter(std::vector<float> token);
    std::vector<float>* Convolve2D_Sobel(RGBA* data, int width, int height, const std::vector<float> &kernel);


public:
    FilterSobel(float para);
    virtual ~FilterSobel() override;
    virtual void apply(Canvas2D *canvas) override;



};

#endif
