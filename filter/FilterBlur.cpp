#include "Canvas2D.h"
#include "FilterBlur.h"
#include <cmath>
#include <iostream>

FilterBlur::FilterBlur(int radius_):
    radius(radius_){
    //initializeFilter();


}

FilterBlur::~FilterBlur()
{

}

void FilterBlur::apply(Canvas2D *canvas)
{
    int width  = canvas->width(), height = canvas->height();
    //RGBA result[width * height];
    //Convolve2D(canvas->data(), height, width, m_kernel);
   box_conv_row(canvas);
   box_conv_col(canvas);


}
/*
void FilterBlur::initializeFilter()
{
    float size = pow(2*radius+1, 2);
    //update m_kernel_y :: along  height
    float k = -1/(float)radius, b = 1;
    std::cout<<"k: "<<k<<std::endl;
    for(int y = -radius; y <= radius; y++)
    {
        //float k_y = -1/(float)radius, b_y = 1;
        //std::cout<<"k_y: "<<k_y<<std::endl;
        for(int x = -radius; x <= radius; x++)
        {
           float y_ele = k * std::abs(y) +b;
           float x_ele = k * std::abs(x) + b;
           m_kernel.push_back(y_ele * x_ele/ size);
        }
        //m_kernel_x.push_back()
    }

}
/*
void FilterBlur::box_conv(Canvas2D* canvas)
{
    float box_size = pow(2*radius+1, 2);
    int width = canvas->width(), height = canvas->height();
    RGBA result[width * height];
    std::vector<std::shared_ptr<std::vector<float>>> normal_data = normal(canvas->data(), width, height);
    std::shared_ptr<std::vector<float>> red_channel = normal_data.at(0);
    std::shared_ptr<std::vector<float>> green_channel = normal_data.at(1);
    std::shared_ptr<std::vector<float>> blue_channel = normal_data.at(2);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            float red_acc = 0, green_acc = 0, blue_acc = 0;
            if(i == 0 && j == 0)
            {
                for(int x = -radius; x<= radius; x++)
                {
                   for(int y = -radius; y <= radius; y++)
                   {
                        int d_height = i + x;
                        d_height = (d_height < 0) ? 0 : (d_height >= height) ? height -1 : d_height;
                        int d_width = j + y;
                        d_width = (d_width < 0) ? 0 : (d_width >=  width) ? width-1 : d_width;
                        red_acc += red_channel->at(d_height * width + d_width)/box_size;
                        green_acc += green_channel->at(d_height * width + d_width)/box_size;
                        blue_acc += blue_channel->at(d_height * width + d_width)/box_size;
                   }
                }
                char red_ = REAL2byte(red_acc);
                char green_ = REAL2byte(green_acc);
                char blue_ = REAL2byte(blue_acc);
                RGBA token(red_, green_, blue_);
                result[i * width + j] = token;
            }

        }
    }
}
*/

void FilterBlur::box_conv_row(Canvas2D* canvas)
{
    float box_size = 2*radius+1;
    int width = canvas->width(), height = canvas->height();
    RGBA result[width * height];
   // std::vector<float> result_r;
   // std::vector<float> result_g;
   // std::vector<float> result_b;

    std::vector<std::shared_ptr<std::vector<float>>> normal_data = normal(canvas->data(), width, height);
    std::shared_ptr<std::vector<float>> red_channel = normal_data.at(0);
    std::shared_ptr<std::vector<float>> green_channel = normal_data.at(1);
    std::shared_ptr<std::vector<float>> blue_channel = normal_data.at(2);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            float red_acc = 0, green_acc = 0, blue_acc = 0;
            if(j == 0){
                for(int x = -radius; x <= radius; x++)
                {
                    int d_width = j + x;
                    d_width = (d_width < 0) ? 0 : (d_width >= width) ? width-1 : d_width;
                    red_acc += red_channel->at(i * width + d_width)/box_size;
                    green_acc += green_channel->at(i * width + d_width)/box_size;
                    blue_acc += blue_channel->at(i * width + d_width)/box_size;
                }
                //result_r.push_back(red_acc);
                //result_g.push_back(green_acc);
                //result_b.push_back(blue_acc);
                char red_ = REAL2byte(red_acc);
                char green_ = REAL2byte(green_acc);
                char blue_ = REAL2byte(blue_acc);
                RGBA token(red_, green_, blue_);
                result[i * width + j] = token;
            }
            else{
                red_acc = (float)result[i * width + j -1].r/255.0;
                green_acc = (float)result[i * width + j -1].g/255.0;
                blue_acc = (float)result[i * width + j -1].b/255.0;

                int left_remove= j-1 - radius;
                left_remove = left_remove < 0 ? 0 : left_remove;


                red_acc -= red_channel->at(i * width + left_remove)/box_size;
                green_acc -= green_channel->at(i * width + left_remove)/box_size;
               // token1 = blue_channel->at(left_remove)/box_size;
                blue_acc -= blue_channel->at(i * width +left_remove)/box_size;

                int right_add= j+radius;
                right_add = right_add >= width ? width-1 : right_add;

                red_acc += red_channel->at(i * width + right_add)/box_size;
                green_acc += green_channel->at(i * width + right_add)/box_size;
                blue_acc += blue_channel->at(i * width +right_add)/box_size;

                char red_ = REAL2byte(red_acc);
                char green_ = REAL2byte(green_acc);
                char blue_ = REAL2byte(blue_acc);
                std::cout<<"red_acc: "<<red_acc<<std::endl;
                std::cout<<"green_acc: "<<green_acc<<std::endl;
                std::cout<<"blue_acc: "<<blue_acc<<std::endl;
                RGBA token(red_, green_, blue_);
                result[i * width + j] = token;


            }
        }
    }
    memcpy(canvas->data(), result, width * height * sizeof(RGBA));

}

void FilterBlur::box_conv_col(Canvas2D* canvas)
{
    float box_size = 2*radius+1;
    int width = canvas->width(), height = canvas->height();
    RGBA result[width * height];
   // std::vector<float> result_r;
   // std::vector<float> result_g;
   // std::vector<float> result_b;

    std::vector<std::shared_ptr<std::vector<float>>> normal_data = normal(canvas->data(), width, height);
    std::shared_ptr<std::vector<float>> red_channel = normal_data.at(0);
    std::shared_ptr<std::vector<float>> green_channel = normal_data.at(1);
    std::shared_ptr<std::vector<float>> blue_channel = normal_data.at(2);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            float red_acc = 0, green_acc = 0, blue_acc = 0;
            if(i == 0){
                for(int x = -radius; x <= radius; x++)
                {
                    int d_height = i + x;
                    d_height = (d_height < 0) ? 0 : (d_height >= height) ? height-1 : d_height;
                    red_acc += red_channel->at(d_height * width + j);
                    green_acc += green_channel->at(d_height * width + j);
                    blue_acc += blue_channel->at(d_height * width + j);
                }
                red_acc /= box_size;
                green_acc /= box_size;
                blue_acc /= box_size;
                //result_r.push_back(red_acc);
                //result_g.push_back(green_acc);
                //result_b.push_back(blue_acc);
                char red_ = REAL2byte(red_acc);
                char green_ = REAL2byte(green_acc);
                char blue_ = REAL2byte(blue_acc);
                RGBA token(red_, green_, blue_);
                result[i * width + j] = token;
            }
            else{
                red_acc = (float)result[(i-1) * width + j].r/255.0;
                green_acc = (float)result[(i-1) * width + j].g/255.0;
                blue_acc = (float)result[(i-1) * width + j].b/255.0;

                int up_remove= (i-1 - radius) < 0 ? 0 :  (i-1 - radius);

                red_acc -= red_channel->at(up_remove * width + j)/box_size;
                green_acc -= green_channel->at(up_remove * width + j)/box_size;
               // token1 = blue_channel->at(left_remove)/box_size;
                blue_acc -= blue_channel->at(up_remove * width + j)/box_size;

                int bot_add= (i + radius) >= height ? height-1 :  (i + radius);


                red_acc += red_channel->at(bot_add * width + j)/box_size;
                green_acc += green_channel->at(bot_add * width + j)/box_size;
                blue_acc += blue_channel->at(bot_add * width + j)/box_size;

                char red_ = REAL2byte(red_acc);
                char green_ = REAL2byte(green_acc);
                char blue_ = REAL2byte(blue_acc);
                std::cout<<"red_acc: "<<red_acc<<std::endl;
                std::cout<<"green_acc: "<<green_acc<<std::endl;
                std::cout<<"blue_acc: "<<blue_acc<<std::endl;
                RGBA token(red_, green_, blue_);
                result[i * width + j] = token;


            }
        }
    }
    memcpy(canvas->data(), result, width * height * sizeof(RGBA));

}


