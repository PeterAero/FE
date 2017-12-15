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
    int getTilePosition(int StartRow, int StartColumn, int EndRow, int EndColumn,
                                      int NbrColumns, int NbrRows);
    void readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn, GDALDataset * poDataset, int Border);
    void writeTileData(int StartRow, int StartColumn, int NbrRows, int NbrColumns,
                       float * TileData,
                       double adfGeoTransform[6],
                       GDALDataset * srcDataset, std::string Prefix);

};

#endif // DATAMANAGER_H
