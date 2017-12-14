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

    char * MyTif = "/home/fisc_p0/Desktop/TestData/SP27GTIF.TIF";
    GDALAllRegister();
    GDALDataset  * srcDataset;
    srcDataset = (GDALDataset *) GDALOpen( MyTif, GA_ReadOnly );

    dataManager MyDataManager;

    MyDataManager.tiledata(srcDataset, 0);

    GDALDataset  * srcDataset2;
    srcDataset2 = (GDALDataset *) GDALOpen( MyTif, GA_ReadOnly );

    MyDataManager.tiledata(srcDataset2, 5);

    return 0;
}
