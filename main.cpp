#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <iostream>
#include <string>
#include <sstream>

#include "datamanager.h"

using namespace std;


int main(int argc, char *argv[])
{
    std::cout << "call tiling with border" << std::endl;
    GDALAllRegister();
    GDALDataset  * srcDataset;
    srcDataset = (GDALDataset *) GDALOpen( "/home/peter/Desktop/SP27GTIF.TIF", GA_ReadOnly );

    dataManager MyDataManager;

    MyDataManager.tiledata(srcDataset);
    std::cout << "call tiling with border" << std::endl;
    GDALDataset  * srcDataset2;
    srcDataset2 = (GDALDataset *) GDALOpen( "/home/peter/Desktop/SP27GTIF.TIF", GA_ReadOnly );

    MyDataManager.tiledata(srcDataset2, 30);

    return 0;
}
