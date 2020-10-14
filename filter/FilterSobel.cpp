#include "FilterSobel.h"
#include "Canvas2D.h"
#include "iostream"

FilterSobel::FilterSobel(float param):
    m_sense(param)
{
    m_kernel_x_row = {-1, 0, 1};
    m_kernel_x_col = {1, 2, 1};
    m_kernel_y_row = {1, 2, 1};
    m_kernel_y_col = {1, 0, -1};
    m_kernel_x = {-1, 0, 1,
                  -2, 0, 2,
                   -1, 0, 1};

    m_kernel_y = {-1, -2, -1,
                   0, 0, 0,
                   1, 2, 1};
    //update_filter(m_kernel_x_row);
    //update_filter(m_kernel_x_col);
    //update_filter(m_kernel_y_row);
    //update_filter(m_kernel_y_col);


}

FilterSobel::~FilterSobel(){

}

void FilterSobel::apply(Canvas2D* canvas)
{
    grayFilter.apply(canvas);


    std::cout<<"FilterSobel apply"<<std::endl;
     int width = canvas->width(), height = canvas->height();
      RGBA result[width * height];
    //calculate G_x

    RGBA* G_x = Convolve2D(canvas->data(), width, height, m_kernel_x);
    //conv1D_row(canvas->data(), canvas->width(), canvas->height(), m_kernel_x_row);
   // RGBA* G_x = conv1D_col(canvas->data(), canvas->width(), canvas->height(),m_kernel_x_col);

    //calculate G_y
    RGBA* G_y = Convolve2D(canvas->data(), width, height, m_kernel_y);
    //conv1D_row(canvas->data(), canvas->width(), canvas->height(), m_kernel_y_row);

    //RGBA * G_y =conv1D_col(canvas->data(), canvas->width(), canvas->height(), m_kernel_y_col);

    std::cout<<"created G_x, G_y"<<std::endl;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            RGBA GX_token = G_x[i * width + j];
            RGBA GY_token = G_y[i * width + j];
            float token_x = (float)(G_x[i * width + j].r);

            float token_y = (float)(G_y[i * width + j].r);
            float token = sqrt( pow(token_x, 2) + pow(token_y, 2) );
             token *= m_sense;
            std::cout<<"tokrn_x: "<<token_x<<"token_y: "<<token_y<<" ,token: "<<token<<std::endl;

            token = token <= 255 ? token : 255;
            result[i * width + j].r = static_cast<int>(token);
            std::cout<<"result ele: "<<result[i * width + j].r<<std::endl;

            token_x = (float)(G_x[i * width + j].g);
            token_y = (float)(G_y[i * width + j].g);
            token = sqrt( pow(token_x, 2) + pow(token_y, 2) );
            token *= m_sense;
            token = token <= 255 ? token : 255;

            result[i * width + j].g = static_cast<int>(token);

            token_x = (float)(G_x[i * width + j].b);
            token_y = (float)(G_y[i * width + j].b);
            token = sqrt( pow(token_x, 2) + pow(token_y, 2) );
            token *= m_sense;
            token = token <= 255 ? token : 255;


            result[i * width + j].b = static_cast<int>(token);
        }
    }
    memcpy(canvas->data(), result, width * height * sizeof(RGBA));

}


