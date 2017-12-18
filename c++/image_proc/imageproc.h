#ifndef IMAGEPROC_H
#define IMAGEPROC_H
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <iostream>

class imageProc
{
public:
    imageProc();
    void filterImg(GDALDataset * poDataset);
    template <typename T> void createCircleMask(T Radius);
    void createDonutMask(float innerRadius, float outerRadius);

private:
    void saveImg(GDALDataset * poDataset, float * ImgData);
    float * FilterKernel;
    int NbrRowsFilterKernel;
    int NbrColumnsFilterKernel;
    int NbrWeights;

};

#endif // IMAGEPROC_H
