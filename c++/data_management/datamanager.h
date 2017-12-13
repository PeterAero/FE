#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <iostream>
#include <string>
#include <sstream>

class dataManager
{

public:
    dataManager();
    void tiledata(GDALDataset * srcDataset);
    void tiledata(GDALDataset * srcDataset, int Border);

private:
    void readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn, GDALDataset * poDataset);
    void readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn, GDALDataset * poDataset, int Border);
    void writeTileData(int StartRow, int StartColumn, int NbrRows, int NbrColumns,
                       float * TileData,
                       double adfGeoTransform[6],
                       GDALDataset * srcDataset);

};

#endif // DATAMANAGER_H
