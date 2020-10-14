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
   std::vector<std::shared_ptr<std::vector<float>>> normal_data = normal(data, width, height);
   std::shared_ptr<std::vector<float>> red_channel = normal_data.at(0);
   std::shared_ptr<std::vector<float>> green_channel = normal_data.at(1);
   std::shared_ptr<std::vector<float>> blue_channel = normal_data.at(2);


    std::cout<<"filter.cpp finish normalization"<<std::endl;
    RGBA result[width * height];
    int token = sqrt(kernel.size());
    int kernel_wid = token;
    int kernel_heigh = token;

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            size_t centerIndex = r * width + c;
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

                    //red_acc += red_channel[d_heigh * width + d_wid] * kernel[k_heigh * kernel_wid + k_wid];
                    //green_acc += green_channel[d_heigh * width + d_wid] * kernel[k_heigh * kernel_wid + k_wid];
                    //blue_acc += blue_channel[d_heigh * width + d_wid] * kernel[k_heigh * kernel_wid + k_wid];

                    red_acc += red_channel->at(d_heigh * width + d_wid) * kernel[k_heigh * kernel_wid + k_wid];
                    green_acc += green_channel->at(d_heigh * width + d_wid) * kernel[k_heigh * kernel_wid + k_wid];
                    blue_acc += blue_channel->at(d_heigh * width + d_wid) * kernel[k_heigh * kernel_wid + k_wid];


                }
            }

            char red_ = REAL2byte(red_acc);
            char green_ = REAL2byte(green_acc);
            char blue_ = REAL2byte(blue_acc);
            RGBA token(red_, green_, blue_);
            result[centerIndex] = token;
        }
    }

    std::cout<<"finish conv2d"<<std::endl;
    //memcpy(data, result, width * height * sizeof(RGBA));
    return result;
}


RGBA* Filter::conv1D_row(RGBA* data, int width, int height, const std::vector<float> &kernel)
{
    std::vector<std::vector<float>*> container;
    std::vector<float> red_channel;
    container.push_back(&red_channel);
    std::vector<float> green_channel;
    container.push_back(&green_channel);
    std::vector<float> blue_channel;
    container.push_back(&blue_channel);
    normalization(data, width, height, &container);

    int kernel_size = kernel.size();
    RGBA result[width * height];
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
                red_acc += red_channel[i * width + d_width] * kernel[k_index];
                green_acc += green_channel[i * width + d_width] * kernel[k_index];
                blue_acc += blue_channel[i * width + d_width] * kernel[k_index];
            }
            char red_ = REAL2byte(red_acc);
            char green_ = REAL2byte(green_acc);
            char blue_ = REAL2byte(blue_acc);
            RGBA token(red_, green_, blue_);
            result[i * width + j] = token;
        }
    }
    memcpy(data, result, width * height * sizeof(RGBA));
    return result;
}

RGBA* Filter::conv1D_col(RGBA* data, int width, int height, const std::vector<float> &kernel)
{
    std::vector<std::vector<float>*> container;
    std::vector<float> red_channel;
    container.push_back(&red_channel);
    std::vector<float> green_channel;
    container.push_back(&green_channel);
    std::vector<float> blue_channel;
    container.push_back(&blue_channel);
    normalization(data, width, height, &container);

    int kernel_size = kernel.size();
    RGBA result[width * height];
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
                red_acc += red_channel[d_heigh * width + j] * kernel[k_index];
                green_acc += green_channel[d_heigh * width + j] * kernel[k_index];
                blue_acc += blue_channel[d_heigh * width + j] * kernel[k_index];
            }
            char red_ = REAL2byte(red_acc);
            char green_ = REAL2byte(green_acc);
            char blue_ = REAL2byte(blue_acc);
            RGBA token(red_, green_, blue_);
            result[i * width + j] =token;
        }
    }
    memcpy(data, result, width * height * sizeof(RGBA));
    return result;
}

void Filter::normalization(RGBA* data, int width, int height, std::vector<std::vector<float>*> *container)
{

    std::vector<float> *red_channel = container->at(0);
    std::vector<float> *green_channel = container->at(1);
    std::vector<float> *blue_channel = container->at(2);


    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j <width; j++)
        {
            RGBA token = data[i * width + j];
            float red = data[i * width + j].r;
            red /=255.0;
            red_channel->push_back(red);
            float green = data[i * width + j].g;
            green /=255.0;
            green_channel->push_back(green);
            float blue = data[i * width + j].b;
            blue /=255.0;
            blue_channel->push_back(blue);
        }
    }

    std::cout<<"inside normalization"<<std::endl;
    return;
}

std::vector<std::shared_ptr<std::vector<float>>> Filter::normal(RGBA* data, int width, int height)
{
    std::vector<std::shared_ptr<std::vector<float>>> ans;
    std::shared_ptr<std::vector<float>> red_chan = std::make_shared<std::vector<float>>();
    std::shared_ptr<std::vector<float>> green_chan = std::make_shared<std::vector<float>>();
    std::shared_ptr<std::vector<float>> blue_chan = std::make_shared<std::vector<float>>();
    ans = {red_chan, green_chan, blue_chan};

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j <width; j++)
        {
            RGBA token = data[i * width + j];
            float red = data[i * width + j].r;
            red /=255.0;
            red_chan->push_back(red);
            float green = data[i * width + j].g;
            green /=255.0;
            green_chan->push_back(green);
            float blue = data[i * width + j].b;
            blue /=255.0;
            blue_chan->push_back(blue);
        }
    }
    return ans;


}

