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
      RGBA * token;
    //calculate G_x

     std::vector<float>* G_x = Convolve2D_Sobel(canvas->data(), width, height, m_kernel_x);


    //calculate G_y
    std::vector<float>* G_y = Convolve2D_Sobel(canvas->data(), width, height, m_kernel_y);


    std::cout<<"created G_x, G_y"<<std::endl;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {

            float token_x = (float)(G_x->at(i * width + j));

            float token_y = (float)(G_y->at(i * width + j));
            float token = sqrt( pow(token_x, 2) + pow(token_y, 2) );
            token *= m_sense;
           // std::cout<<"tokrn_x: "<<token_x<<"token_y: "<<token_y<<" ,token: "<<token<<std::endl;

          //  token = token <= 255 ? token : 255;
            result[i * width + j].r = REAL2byte(token);
            result[i * width + j].g =  REAL2byte(token);
            result[i * width + j].b =  REAL2byte(token);
        }
    }
    delete G_x;
    delete G_y;
    memcpy(canvas->data(), result, width * height * sizeof(RGBA));


}

std::vector<float>* FilterSobel::Convolve2D_Sobel(RGBA* data, int width, int height, const std::vector<float> &kernel)
{

    //std::cout<<"Filter.cpp  conv2d"<<std::endl;

    std::vector<std::vector<float>*>* normal_data = normal(data, width, height);
    std::vector<float>* red_channel = normal_data->at(0);
    std::vector<float>* green_channel = normal_data->at(1);
    std::vector<float>* blue_channel = normal_data->at(2);

    std::cout<<"filter.cpp finish normalization"<<std::endl;
   // RGBA* result = new RGBA[width * height];
    std::vector<float>* result = new std::vector<float>();
    int token = sqrt(kernel.size());
    int kernel_wid = token;
    int kernel_heigh = token;

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
          //  size_t centerIndex = r * width + c;
            float red_acc = 0, green_acc = 0, blue_acc = 0;
            for(int i = -kernel_heigh/2; i <= kernel_heigh/2; i++)
            {
                for(int j = -kernel_wid/2; j <= kernel_wid/2; j++)
                {
                   int k_wid = j + kernel_wid/2;
                    int k_heigh = i + kernel_heigh/2;
                    int d_wid = c + j;
                    int d_heigh = r + i;
                    if(d_wid < 0 || d_wid >= width || d_heigh < 0 || d_heigh >= height)
                        continue;

                    red_acc += (float)red_channel->at(d_heigh * width + d_wid) * kernel[k_heigh * kernel_wid + k_wid];
                    //green_acc += (float)green_channel->at(d_heigh * width + d_wid) * kernel[k_heigh * kernel_wid + k_wid];
                    //blue_acc += (float)blue_channel->at(d_heigh * width + d_wid) * kernel[k_heigh * kernel_wid + k_wid];

                }
            }

            result->push_back(red_acc);
            //result[r * width + c].r = red_;
            //result[r * width + c].g = green_;
            //result[r * width + c].b = blue_;
        }
    }

    std::cout<<"finish conv2d"<<std::endl;
    return result;
}
