#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "datamanager.h"

using namespace std;


int main(int argc, char *argv[])
{

    char * MyTif = "/home/fisc_p0/Desktop/TestData/SP27GTIF.TIF";
    int BorderSize = 5;

    dataManager MyDataManager(MyTif, BorderSize);

    return 0;
}
