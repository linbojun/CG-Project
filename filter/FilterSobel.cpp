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
   //result =  conv1D_row(canvas->data(), canvas->width(), canvas->height(), m_kernel_x_row);
   //memcpy(canvas->data(), result, width * height * sizeof (RGBA));

   // RGBA* G_x = conv1D_col(result, canvas->width(), canvas->height(),m_kernel_x_col);
    //memcpy(canvas->data(), G_x, width * height * sizeof (RGBA));

    //calculate G_y
    RGBA* G_y = Convolve2D(canvas->data(), width, height, m_kernel_y);
    //result = conv1D_row(canvas->data(), canvas->width(), canvas->height(), m_kernel_y_row);
    //memcpy(canvas->data(), result, width * height * sizeof (RGBA));
    //RGBA * G_y =conv1D_col(result, canvas->width(), canvas->height(), m_kernel_y_col);

    std::cout<<"created G_x, G_y"<<std::endl;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {

            float token_x = (float)(G_x[i * width + j].r);

            float token_y = (float)(G_y[i * width + j].r);
            float token = sqrt( pow(token_x, 2) + pow(token_y, 2) );
            token *= m_sense;
            std::cout<<"tokrn_x: "<<token_x<<"token_y: "<<token_y<<" ,token: "<<token<<std::endl;

            token = token <= 255 ? token : 255;
            result[i * width + j].r = static_cast<int>(token);
            result[i * width + j].g = static_cast<int>(token);
            token = token <= 255 ? token : 255;


            result[i * width + j].b = static_cast<int>(token);
        }
    }
    delete G_x;
    delete G_y;
    memcpy(canvas->data(), result, width * height * sizeof(RGBA));


}

/*
//-------------------------------------------------------
void FilterSobel::apply(Canvas2D *canvas){

    std::cout<<"his sobel aplly"<<std::endl;
    glm::vec3 x1 = glm::vec3( 1.0f,  2.0f,  1.0f);

    glm::vec3 x2 = glm::vec3(-1.0f,  0.0f,  1.0f);

    glm::vec3 y1 = glm::vec3( 1.0f,  2.0f,  1.0f);

    glm::vec3 y2 = glm::vec3( 1.0f,  0.0f, -1.0f);




     grayFilter.apply(canvas);




    float* x = Conv2dByVecs(canvas, x1, x2);

    float* y = Conv2dByVecs(canvas, y1, y2);




    int width = canvas->width();

    int height = canvas->height();

    RGBA* data = canvas->data();




    for(int i = 1; i < height; i++) {

        for (int j = 1; j < width; j++){

            int idx = i * width + j;

            float f = sqrt(pow(x[idx], 2) + pow(y[idx], 2)) / 255.0f;

            f = f * m_sense;

            unsigned char v = REAL2byte(f);

            data[idx] = RGBA(v, v, v, 255);

        }

    }




    delete[] x;

    delete[] y;

}
*/
