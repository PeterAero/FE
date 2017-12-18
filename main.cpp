#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "datamanager.h"
#include "imageproc.h"

using namespace std;


int main(int argc, char *argv[])
{


    char * MyTif = "/home/peter/Desktop/TestData/SP27GTIF.TIF";
    int BorderSize = 5;

    dataManager MyDataManager(MyTif, BorderSize);

    imageProc MyImageProcessor;

    MyImageProcessor.filterImg(MyDataManager.FileListBorder[0]);





    return 0;
}
