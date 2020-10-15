#include "Filter.h"
#include "Canvas2D.h"
#include "iostream"


Filter::~Filter()
{
}

unsigned char Filter::REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

RGBA* Filter::Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel)
{

    //std::cout<<"Filter.cpp  conv2d"<<std::endl;
    /*
    std::vector<std::vector<float>*> container;
    std::vector<float> red_channel;
    container.push_back(&red_channel);
    std::vector<float> green_channel;
    container.push_back(&green_channel);
    std::vector<float> blue_channel;
    container.push_back(&blue_channel);
    normalization(data, width, height, &container);
    */
    std::vector<std::vector<float>*>* normal_data = normal(data, width, height);
    std::vector<float>* red_channel = normal_data->at(0);
    std::vector<float>* green_channel = normal_data->at(1);
    std::vector<float>* blue_channel = normal_data->at(2);

    std::cout<<"filter.cpp finish normalization"<<std::endl;
    RGBA* result = new RGBA[width * height];
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
                    green_acc += (float)green_channel->at(d_heigh * width + d_wid) * kernel[k_heigh * kernel_wid + k_wid];
                    blue_acc += (float)blue_channel->at(d_heigh * width + d_wid) * kernel[k_heigh * kernel_wid + k_wid];

                }
            }
            unsigned char red_ = REAL2byte(red_acc);
            unsigned char green_ = REAL2byte(green_acc);
            unsigned char blue_ = REAL2byte(blue_acc);
            result[r * width + c].r = red_;
            result[r * width + c].g = green_;
            result[r * width + c].b = blue_;
        }
    }

    std::cout<<"finish conv2d"<<std::endl;
    //memcpy(data, result, width * height * sizeof(RGBA));
    return result;
}
RGBA* Filter::conv1D_row(RGBA* data, int width, int height, const std::vector<float> &kernel)
{
    std::vector<std::vector<float>*>* normal_data = normal(data, width, height);
    std::vector<float>* red_channel = normal_data->at(0);
    std::vector<float>* green_channel = normal_data->at(1);
    std::vector<float>* blue_channel = normal_data->at(2);

    int kernel_size = kernel.size();
    RGBA* result = new RGBA[width * height];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            float red_acc = 0, green_acc = 0, blue_acc = 0;
            for(int m = -kernel_size/2; m <= kernel_size/2; m++)
            {
                int d_width = j + m;
                if(d_width < 0 || d_width >= width)
                    continue;
                int k_index = m+kernel_size/2;
                red_acc += (float)red_channel->at(i * width + d_width) * kernel[k_index];
                green_acc += (float)green_channel->at(i * width + d_width) * kernel[k_index];
                blue_acc += (float)blue_channel->at(i * width + d_width) * kernel[k_index];
            }
            unsigned char red_ = REAL2byte(red_acc);
            unsigned char green_ = REAL2byte(green_acc);
            unsigned char blue_ = REAL2byte(blue_acc);
            result[i * width + j].r = red_;
            result[i * width + j].g = green_;
            result[i * width + j].b = blue_;
        }
    }
    delete normal_data;
    delete red_channel;
    delete green_channel;
    delete blue_channel;

    //memcpy(data, result, width * height * sizeof(RGBA));
    return result;
}




RGBA* Filter::conv1D_col(RGBA* data, int width, int height, const std::vector<float> &kernel)
{
    std::vector<std::vector<float>*>* normal_data = normal(data, width, height);
    std::vector<float>* red_channel = normal_data->at(0);
    std::vector<float>* green_channel = normal_data->at(1);
    std::vector<float>* blue_channel = normal_data->at(2);


    int kernel_size = kernel.size();
    RGBA* result = new RGBA[width * height];
    for(int j = 0; j < width; j++)
    {
        for(int i = 0; i < height; i++)
        {
            float red_acc = 0, green_acc = 0, blue_acc = 0;
            for(int m = -kernel_size/2; m <= kernel_size/2; m++)
            {
                int d_heigh = i+m;
                if(d_heigh < 0 || d_heigh >= height)
                    continue;
                int k_index = m+kernel_size/2;
                red_acc += (float)red_channel->at(d_heigh * width + j) * kernel[k_index];
                green_acc += (float)green_channel->at(d_heigh * width + j) * kernel[k_index];
                blue_acc += (float)blue_channel->at(d_heigh * width + j) * kernel[k_index];
            }
            unsigned char red_ = REAL2byte(red_acc);
            unsigned char green_ = REAL2byte(green_acc);
            unsigned char blue_ = REAL2byte(blue_acc);
            result[i * width + j].r = red_;
            result[i * width + j].g = green_;
            result[i * width + j].b = blue_;

        }
    }

    delete normal_data;
    delete red_channel;
    delete green_channel;
    delete blue_channel;
    //memcpy(data, result, width * height * sizeof(RGBA));
    return result;
}



std::vector<std::vector<float>*>* Filter::normal(RGBA* data, int width, int height)
{
    std::vector<std::vector<float>*>* ans = new std::vector<std::vector<float>*>();
    std::vector<float>* red_chan = new std::vector<float>();
    std::vector<float>* green_chan = new std::vector<float>();
    std::vector<float>* blue_chan = new std::vector<float>();
    ans->push_back(red_chan);
    ans->push_back(green_chan);
    ans->push_back(blue_chan);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j <width; j++)
        {
            float red = data[i * width + j].r;
            red /=255.0f;
            red_chan->push_back(red);
            float green = data[i * width + j].g;
            green /=255.0f;
            green_chan->push_back(green);
            float blue = data[i * width + j].b;
            blue /=255.0f;
            blue_chan->push_back(blue);
        }
    }
    return ans;


}

