#include "FilterScaling.h"
#include "Canvas2D.h"
#include "iostream"
#include <math.h>
#include "SupportCanvas2D.h"


FilterScaling::FilterScaling(float x, float y):
    _x(x),
    _y(y)
{

}


FilterScaling::~FilterScaling()
{

}

void FilterScaling::apply(Canvas2D *canvas)
{
    std::cout<<"run scaling apply"<<std::endl;
   //conv_x(canvas);
    conv_y(canvas);

}
void FilterScaling::conv_x(Canvas2D* canvas)
{
    if(_x == 1)
        return;
     std::cout<<"run scaling conv_x"<<std::endl;
    int old_width = canvas->width();
    int old_height = canvas->height();
    new_width = old_width * _x;
    RGBA result[new_width * old_height];
    for(int i = 0; i < old_height; i++)
    {
        /*
        std::vector<float> red_chan;
        std::vector<float> green_chan;
        std::vector<float> blue_chan;
        for(int j = 0; j < old_width; j ++)
        {
            red_chan.push_back((float)canvas->data()[i * old_width + j].r);
            green_chan.push_back((float)canvas->data()[i * old_width + j].g);
            blue_chan.push_back((float)canvas->data()[i * old_width + j].b);
        }
        */
        for(int j = 0; j < new_width; j++)
        {
            result[i * new_width + j] = conv_x_cell(canvas, j, i);
        }
        std::cout<<"finish line: "<<i<<std::endl;
    }
    std::cout<<"finish whole image"<<std::endl;
    canvas->resize(new_width, old_height);
    memcpy(canvas->data(), result, new_width * old_height * sizeof(RGBA));
}

RGBA FilterScaling::conv_x_cell(Canvas2D* canvas, int x_pos, int y_pos)
{
     std::cout<<"run scaling conv_x_csll on"<<x_pos<<", "<<y_pos<<std::endl;
    int old_width = canvas->width(), old_height = canvas->height();
    std::cout<<"old_width: "<<old_width<<", old_height"<<old_height<<", old_size"<<old_width*old_height<<std::endl;
    RGBA* data = canvas->data();
    if(_x == 1)
        return canvas->data()[y_pos * old_width + x_pos];
    float center_x = x_pos/_x + (1-_x)/(2*_x);
    float kernel_len = (_x > 1) ? 2 : 2/_x;

    float k = -2/kernel_len, b = 1;
    int left = (int)ceil(center_x - kernel_len/2), right = (int)floor(center_x+kernel_len/2);
    std::cout<<"x_center: "<<center_x<<" ,left"<<left<<" right: "<<right<<std::endl;
    float red_acc = 0, green_acc = 0, blue_acc = 0;
    for(int i = left; i <= right; i++)
    {
        int index = i;
        float dist = std::abs(center_x - i);
        if(i < 0)
            index = 0;
        if(i >= old_width)
            index = old_width-1;
        std::cout<<"weight: "<<k * dist + b<<std::endl;

        //float token = data[y_pos * new_width + i].r;
        std::cout<<"r:"<<(float)data[y_pos * old_width + index].r<<std::endl;
        std::cout<<"g:"<<(float)data[y_pos * old_width + index].g<<std::endl;
        std::cout<<"b:"<<(float)data[y_pos * old_width + index].b<<std::endl;
        red_acc += (float)data[y_pos * old_width + index].r * (k * dist + b);
        green_acc += (float)data[y_pos * old_width + index].g * (k * dist + b);
        blue_acc += (float)data[y_pos * old_width +index].b * (k * dist + b);

    }

    char red_ = (int)red_acc/kernel_len;
    char green_ = (int)green_acc/kernel_len;
    char blue_ = (int)blue_acc/kernel_len;
    RGBA token(red_, green_, blue_);
    return token;
}
/*

RGBA FilterScaling::conv_x_cell(std::vector<float> r_c, std::vector<float> g_c, std::vector<float> b_c, int pos)
{
    int old_width = g_c.size();
    std::cout<<"old_width: g_c.size"<<g_c.size()<<"r_c.size()"<<r_c.size()<<", b_c size: "<<b_c.size()<<std::endl;
    float center_x = pos/_x + (1-_x)/(2*_x);
    float kernel_len = (_x > 1) ? 2 : 2/_x;
    float k = -2/kernel_len, b = 1;
    int left = (int)ceil(center_x - kernel_len/2), right = (int)floor(center_x+kernel_len/2);
    float red_acc = 0, green_acc = 0, blue_acc = 0;
    for(int i = left; i <= right; i++)
    {
        int index = i;
        if(i < 0)
            index = 0;
        if(i >= old_width)
            index = old_width-1;
        std::cout<<"index: "<<index<<std::endl;
        float dist = std::abs(center_x - i);
        red_acc += r_c[index] * (k * dist + b);
        green_acc += g_c[index] * (k * dist + b);
        blue_acc += b_c[index] * (k * dist + b);
    }
    float real_len = right - left;
    char red_ = (int)red_acc/real_len;
    char green_ = (int)green_acc/real_len;
    char blue_ = (int)blue_acc/real_len;
    RGBA token(red_, green_, blue_);
    std::cout<<"conv_x_cell_done"<<std::endl;
    return token;
}

*/

void FilterScaling::conv_y(Canvas2D* canvas)
{
    if(_y == 1)
        return;
     std::cout<<"run scaling conv_x"<<std::endl;
    int old_width = canvas->width();
    int old_height = canvas->height();
    new_height = old_height * _y;
    RGBA result[old_width * new_height];
    for(int i = 0; i < new_height; i++)
    {
        /*
        std::vector<float> red_chan;
        std::vector<float> green_chan;
        std::vector<float> blue_chan;
        for(int j = 0; j < old_width; j ++)
        {
            red_chan.push_back((float)canvas->data()[i * old_width + j].r);
            green_chan.push_back((float)canvas->data()[i * old_width + j].g);
            blue_chan.push_back((float)canvas->data()[i * old_width + j].b);
        }
        */
        for(int j = 0; j < old_width; j++)
        {
            result[i * old_width + j] = conv_y_cell(canvas, j, i);
        }
       // std::cout<<"finish line: "<<i<<std::endl;
    }
    std::cout<<"finish whole image"<<std::endl;
    canvas->resize(old_width, new_height);
    memcpy(canvas->data(), result, old_width * new_height * sizeof(RGBA));
}

RGBA FilterScaling::conv_y_cell(Canvas2D* canvas, int x_pos, int y_pos)
{

    int old_width = canvas->width(), old_height = canvas->height();
    RGBA* data = canvas->data();
    float center_y = y_pos/_y + (1-_y)/(2*_y);
    float kernel_len = (_y > 1) ? 2 : 2/_y;

    float k = -2/kernel_len, b = 1;
    int left = (int)ceil(center_y - kernel_len/2), right = (int)floor(center_y+kernel_len/2);
   // std::cout<<"x_center: "<<center_x<<" ,left"<<left<<" right: "<<right<<std::endl;
    float red_acc = 0, green_acc = 0, blue_acc = 0;
    for(int i = left; i <= right; i++)
    {
        int index = i;
        float dist = std::abs(center_y - i);
        if(i < 0)
            index = 0;
        if(i >= old_height)
            index = old_height-1;
        std::cout<<"weight: "<<k * dist + b<<std::endl;

        //float token = data[y_pos * new_width + i].r;
        //std::cout<<"r:"<<(float)data[y_pos * old_width + index].r<<std::endl;
        //std::cout<<"g:"<<(float)data[y_pos * old_width + index].g<<std::endl;
        //std::cout<<"b:"<<(float)data[y_pos * old_width + index].b<<std::endl;
        red_acc += (float)data[index* old_width + x_pos].r * (k * dist + b);
        green_acc += (float)data[index * old_width + x_pos].g * (k * dist + b);
        blue_acc += (float)data[index * old_width +x_pos].b * (k * dist + b);

    }
    char red_ = (int)red_acc/kernel_len;
    char green_ = (int)green_acc/kernel_len;
    char blue_ = (int)blue_acc/kernel_len;
    RGBA token(red_, green_, blue_);
    return token;
}
