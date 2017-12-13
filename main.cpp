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

    GDALAllRegister();
    GDALDataset  * srcDataset;
    srcDataset = (GDALDataset *) GDALOpen( "/home/peter/Desktop/SP27GTIF.TIF", GA_ReadOnly );

    dataManager MyDataManager;

    MyDataManager.tiledata(srcDataset, 0);

    GDALDataset  * srcDataset2;
    srcDataset2 = (GDALDataset *) GDALOpen( "/home/peter/Desktop/SP27GTIF.TIF", GA_ReadOnly );

    MyDataManager.tiledata(srcDataset2, 5);

    return 0;
}
