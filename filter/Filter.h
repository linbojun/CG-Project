
#ifndef Filter_H
#define Filter_H

#include <assert.h>
#include <vector>
#include "RGBA.h"
class Canvas2D;

/**
 * @class Filter
 *
 * This is the superclass for all filters.
 *
 * It declares the abstract method apply, which is called by Canvas2D to apply the filter.
 */
class Filter {

public:
    virtual ~Filter();

    // Applies this filter on the given region.
    //
    // The apply method is called by FilterCanvas to apply this filter
    // to an entire image. This method is abstract, you will need to
    // redefine it each filter subclass.
    //

    virtual void apply(Canvas2D* canvas) = 0;
    unsigned char REAL2byte(float f);
    RGBA* Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel);
    RGBA* conv1D_col(RGBA* data, int width, int height, const std::vector<float> &kernel);
    RGBA* conv1D_row(RGBA* data, int width, int height, const std::vector<float> &kernel);
    void normalization(RGBA* data, int width, int height, std::vector<std::vector<float>*> *container);
    std::vector<std::shared_ptr<std::vector<float>>> normal(RGBA* data, int width, int height);

};

#endif


