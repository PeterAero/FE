#ifndef IMAGEPROC_H
#define IMAGEPROC_H
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>



class imageProc
{
public:
    imageProc();
    void filterImg(GDALDataset * poDataset);
    template <typename T> void createCircleMask(T Radius);
    template <typename T> void createDonutMask(T innerRadius, T outerRadius);

private:
    void saveImg(GDALDataset * poDataset, float * ImgData);
    void printCurrentKernel();
    void computeTPI(float * InputData, float * OutputData, int NbrRows, int NbrColumns);
    void initMatrix(int size1, int size2);
    int NbrWeights;
    boost::numeric::ublas::matrix<double> FilterKernel;
};

#endif // IMAGEPROC_H
