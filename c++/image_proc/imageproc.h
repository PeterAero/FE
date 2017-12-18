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
    void createCircleMask(float Radius);

private:
    void saveImg(GDALDataset * poDataset, float * ImgData);
    float * FilterKernel;
    int NbrRowsFilterKernel;
    int NbrColumnsFilterKernel;

};

#endif // IMAGEPROC_H
