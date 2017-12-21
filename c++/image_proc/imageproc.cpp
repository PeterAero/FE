#include "imageproc.h"
#include <math.h>

using namespace std;

imageProc::imageProc()
{
    //this->NbrRowsFilterKernel = 0;
    //this->NbrColumnsFilterKernel = 0;
    this->NbrWeights = 0;
}

void imageProc::initMatrix(int size1, int size2){
    this->FilterKernel.resize(2 * size1 + 1, 2 * size2 + 1);
    for (int i = 0; i < this->FilterKernel.size1(); i++){
        for (int j = 0; j < this->FilterKernel.size2(); j++){
            this->FilterKernel(i,j) = 0;
        }
    }

}
void imageProc::printCurrentKernel(){

    for(int i = 0; i < this->FilterKernel.size1(); i++){

        for(int j = 0; j < this->FilterKernel.size2(); j++){

            std::cout << this->FilterKernel(i,j) << " ";

        }

        std::cout << std::endl;

    }

}
/*
template <typename T> void imageProc::createDonutMask(T innerRadius, T outerRadius){

    this->NbrRowsFilterKernel = 2 * outerRadius + 1;
    this->NbrColumnsFilterKernel = 2 * outerRadius + 1;
    this->FilterKernel = (float *) CPLMalloc(sizeof(float) * this->NbrRowsFilterKernel * this->NbrColumnsFilterKernel);

    for(int i = 0; i < this->NbrRowsFilterKernel * this->NbrColumnsFilterKernel; i++){
        this->FilterKernel[i] = 0.;
    }

    float x_center = outerRadius;
    float y_center = outerRadius;

    int Index = 0;
    float dist = 0;

    for(int i = 0; i < this->NbrRowsFilterKernel; i++){

        for(int j = 0; j < this->NbrColumnsFilterKernel; j++){

            Index = i + j * this->NbrColumnsFilterKernel;
            dist = sqrt(pow(x_center - i, 2.) + pow(y_center - j, 2.));

            if (dist <= outerRadius &&  dist >= innerRadius){
                this->FilterKernel[Index] = 1.;
                this->NbrWeights++;
            }

        }

    }

}
*/
template <typename T> void imageProc::createCircleMask(T Radius){

    //this->NbrRowsFilterKernel = 2 * Radius + 1;
    //this->NbrColumnsFilterKernel = 2 * Radius + 1;
    //this->FilterKernel = (float *) CPLMalloc(sizeof(float) * this->NbrRowsFilterKernel * this->NbrColumnsFilterKernel);


    //for(int i = 0; i < this->NbrRowsFilterKernel * this->NbrColumnsFilterKernel; i++){
    //    this->FilterKernel[i] = 0.;
    //}
    std::cout << this->FilterKernel << std::endl;
    int x_center = round(Radius);//round(this->NbrRowsFilterKernel/2.);
    int y_center = round(Radius);//round(this->NbrColumnsFilterKernel/2.);

    int Index = 0;
    double dist = 0;

    for(int i = 0; i < this->FilterKernel.size1(); i++){

        for(int j = 0; j < this->FilterKernel.size2(); j++){

            dist = pow(x_center - i, 2.) + pow(y_center - j, 2.);

            if (dist <= pow(Radius, 2.)){
                this->FilterKernel(i,j) = 1.;
                this->NbrWeights++;
            }

        }

    }

}

void imageProc::saveImg(GDALDataset * srcDataset, float * ImgData){

    const char * pszFormat = "GTiff";
    GDALDriver * poDriver;
    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    GDALDataset * poDstDataset;
    char **papszOptions = NULL;
    int NbrColumns = srcDataset->GetRasterXSize();
    int NbrRows = srcDataset->GetRasterYSize();

    std::string FileName ("/home/peter/Desktop/TestData/TPI.tif");
    std::cout << "FileName: "<< FileName << std::endl;
    poDstDataset = poDriver->Create( FileName.c_str(), NbrColumns, NbrRows, 1, GDT_Float32,
                                papszOptions );

    std::cout << "Create Output Image" << std::endl;
    GDALRasterBand * poDstBand;
    double adfGeoTransform[6];
    srcDataset->GetGeoTransform( adfGeoTransform );
    poDstDataset->SetGeoTransform( adfGeoTransform );
    poDstDataset->SetProjection(srcDataset->GetProjectionRef());

    poDstBand = poDstDataset->GetRasterBand(1);

    poDstBand->RasterIO( GF_Write, 0, 0, NbrColumns, NbrColumns,
                      ImgData, NbrColumns, NbrColumns, GDT_Float32, 0, 0 );


}
/*
void imageProc::computeTPI(float * InputData, float * OutputData, int NbrRows, int NbrColumns){

    double tmpValue = 0.;
    int tmpIndexImage = 0;
    int tmpIndexKernel = 0;

    for(int i = 0; i < NbrRows - this->NbrRowsFilterKernel; i++){

        for(int j = 0; j < NbrColumns - this->NbrColumnsFilterKernel; j++){

            tmpValue = 0.;

            for(int k = 0; k < this->NbrRowsFilterKernel; k++){
                for ( int l = 0; l < this->NbrColumnsFilterKernel; l++){
                    tmpIndexImage = (i + k) + ((j * NbrColumns) + l);
                    tmpIndexKernel = k + l * this->NbrColumnsFilterKernel;
                    tmpValue += InputData[tmpIndexImage] * this->FilterKernel[tmpIndexKernel];
                }
            }

            tmpValue /= this->NbrWeights;
            tmpIndexImage = i + this->NbrColumnsFilterKernel/2 + j * NbrColumns + this->NbrRowsFilterKernel/2*NbrColumns;
            OutputData[tmpIndexImage] = tmpValue;


        }

    }

}
*/
void imageProc::filterImg(GDALDataset * srcDataset){

    float * InputData;
    float * OutputData;
    GDALRasterBand * poBand;
    poBand = srcDataset->GetRasterBand( 1 );
    int NbrColumns = srcDataset->GetRasterXSize();
    int NbrRows = srcDataset->GetRasterYSize();

    InputData = (float *) CPLMalloc(sizeof(float) * NbrRows * NbrColumns);
    OutputData = (float *) CPLMalloc(sizeof(float) * NbrRows * NbrColumns);

    poBand->RasterIO( GF_Read, 0, 0, NbrColumns, NbrRows,
                      InputData, NbrColumns, NbrRows, GDT_Float32,
                      0, 0 );

    for(int i = 0; i < NbrRows*NbrColumns; i++){
        OutputData[i] = 0.;
    }


    /* * * * * * * * * creation of kernel mask for filtering * * * * * * * * */
    this->initMatrix(3, 3);
    this->printCurrentKernel();
    this->createCircleMask(3.);
    //this->createDonutMask(2., 7.);
    this->printCurrentKernel();

    //this->computeTPI(InputData, OutputData, NbrRows, NbrColumns);

    //this->saveImg(srcDataset, OutputData);
}
