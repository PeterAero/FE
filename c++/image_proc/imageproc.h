#ifndef IMAGEPROC_H
#define IMAGEPROC_H
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()

class imageProc
{
public:
    imageProc();
    void filterImg(GDALDataset * poDataset, float * FilterKernel);
};

#endif // IMAGEPROC_H
