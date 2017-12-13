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

    GDALDataset  * srcDataset;
    GDALAllRegister();
    srcDataset = (GDALDataset *) GDALOpen( "/home/peter/Desktop/SP27GTIF.TIF", GA_ReadOnly );

    dataManager MyDataManager;
    MyDataManager.tiledata(srcDataset);

    return 0;
}
