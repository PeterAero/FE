#include "imageproc.h"

imageProc::imageProc()
{
}

void imageProc::filterImg(GDALDataset * poDataset, float * FilterKernel, int KernelSize){

    float * InputData;
    float * OutputData;
    GDALRasterBand * poBand;
    poBand = poDataset->GetRasterBand( 1 );
    int NbrColumns = poDataset->GetRasterXSize();
    int NbrRows = poDataset->GetRasterYSize();

    InputData = (float *) CPLMalloc(sizeof(float) * NbrRows * NbrColumns);
    OutputData = (float *) CPLMalloc(sizeof(float) * NbrRows * NbrColumns);

    poBand->RasterIO( GF_Read, 0, 0, NbrColumns, NbrRows,
                      InputData, NbrColumns, NbrRows, GDT_Float32,
                      0, 0 );

    for(int i = 0; i < NbrRows*NbrColumns; i++){
        OutputData[i] = 0.;
    }


    float * MyFilterKernel;
    int NbrRowsFilter = 5;
    int NbrColumnsFilter = 5;
    int FilterSize = 25;

    InputData = (float *) CPLMalloc(sizeof(float) * NbrRowsFilter * NbrColumnsFilter);
    for(int i = 0; i < 5 * 5; i++){
        MyFilterKernel[i] = 0.;
    }

    MyFilterKernel[2] = 1;
    MyFilterKernel[6] = 1;
    MyFilterKernel[8] = 1;
    MyFilterKernel[10] = 1;
    MyFilterKernel[14] = 1;
    MyFilterKernel[16] = 1;
    MyFilterKernel[18] = 1;
    MyFilterKernel[22] = 1;


    double tmpValue = 0.;
    int tmpIndexImage = 0;
    int tmpIndexKernel = 0;

    for(int i = 0; i < NbrRows - NbrRowsFilter; i++){

        for(int j = 0; j < NbrColumns - NbrColumnsFilter; j++){

            tmpValue = 0.;

            for(int k = 0; k < NbrRowsFilter; k++){
                for ( int l = 0; l < NbrColumnsFilter; l++){
                    tmpIndexImage = (i + k) + ((j * NbrColumns) + l);
                    tmpIndexKernel = k + l * NbrColumnsFilter;
                    tmpValue += InputData[tmpIndexImage] * MyFilterKernel[tmpIndexKernel];
                }
            }

            tmpValue /= FilterSize;
            tmpIndexImage = i + j * NbrColumns;
            OutputData[tmpIndexImage] = tmpValue;

        }

    }

}
