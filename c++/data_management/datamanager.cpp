#include "datamanager.h"



void dataManager::tiledata(GDALDataset * srcDataset, int Border){

    // iterate over Image
    int TileSize = 200;
    int StartRow = 0, EndRow = 0, StartColumn = 0, EndColumn = 0;
    bool top = false, bottom = false, left = false, right = false;
    std::cout << "Enter Tiling with Border" << std::endl;

    for(int i = 0; i < srcDataset->GetRasterXSize(); i += TileSize){
        for(int j = 0; j < srcDataset->GetRasterYSize(); j += TileSize){

            if(i == 0){
                StartRow = i;
                top = true;
            }else{
                StartRow = i - Border;
            }

            if(j == 0){
                StartColumn = j;
                left = true;
            }else{
                StartColumn = j - Border;
            }

            if(srcDataset->GetRasterYSize() < StartColumn + TileSize + Border){
                EndColumn = srcDataset->GetRasterYSize();
                right = true;
            }else{
                EndColumn = StartColumn + TileSize + Border;
            }

            if(srcDataset->GetRasterXSize() < StartRow + TileSize + Border){
                EndRow = srcDataset->GetRasterXSize();
                bottom = true;
            }else{
                EndRow = StartRow + TileSize;
            }
            std::cout << "top: " << top << std::endl;
            std::cout << "bottom: " << top << std::endl;
            std::cout << "left: " << top << std::endl;
            std::cout << "right: " << top << std::endl;
            readTileData(StartRow, StartColumn, EndRow, EndColumn, srcDataset, Border, top, bottom, left, right);

        }

    }

    GDALClose( (GDALDatasetH) srcDataset );
}

void dataManager::tiledata(GDALDataset * srcDataset){

    // iterate over Image
    int TileSize = 200;
    int StartRow, EndRow, StartColumn, EndColumn = 0;

    for(int i = 0; i < srcDataset->GetRasterXSize(); i += TileSize){
        for(int j = 0; j < srcDataset->GetRasterYSize(); j += TileSize){

            StartRow = i;
            StartColumn = j;

            if(srcDataset->GetRasterYSize() < StartColumn + TileSize){
                EndColumn = srcDataset->GetRasterYSize();
            }else{
                EndColumn = StartColumn + TileSize;
            }

            if(srcDataset->GetRasterXSize() < StartRow + TileSize){
                EndRow = srcDataset->GetRasterXSize();
            }else{
                EndRow = StartRow + TileSize;
            }
            readTileData(StartRow, StartColumn, EndRow, EndColumn, srcDataset);
        }
    }

    GDALClose( (GDALDatasetH) srcDataset );
}

void dataManager::readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn, GDALDataset * poDataset){

    float * TileData;
    GDALRasterBand *poBand;
    poBand = poDataset->GetRasterBand( 1 );

    int nXSize = EndRow - StartRow;
    int nYSize = EndColumn - StartColumn;

    TileData = (float *) CPLMalloc(sizeof(float)*nXSize*nYSize);

    poBand->RasterIO( GF_Read, StartRow, StartColumn, nXSize, nYSize,
                      TileData, nXSize, nYSize, GDT_Float32,
                      0, 0 );

    double adfGeoTransform[6];
    poDataset->GetGeoTransform( adfGeoTransform );
    adfGeoTransform[0] += StartRow * adfGeoTransform[1];
    adfGeoTransform[3] += StartColumn * adfGeoTransform[5];

    writeTileData(StartRow, StartColumn, nXSize, nYSize, TileData, adfGeoTransform, poDataset, "NoBorder");

}

void dataManager::readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn, GDALDataset * poDataset,
                               int Border, bool top, bool bottom, bool left, bool right){


    float * TileData;
    GDALRasterBand * poBand;
    poBand = poDataset->GetRasterBand( 1 );

    int nXSize = EndRow - StartRow;
    int nYSize = EndColumn - StartColumn;

    TileData = (float *) CPLMalloc(sizeof(float)*nXSize*nYSize);

    for(int i = 0; i < nXSize*nYSize; i++){
        TileData[i] = 0.;
    }
    /*
    poBand->RasterIO( GF_Read, StartRow, StartColumn, nXSize, nYSize,
                      TileData, nXSize, nYSize, GDT_Float32,
                      0, 0 );
    */
    double adfGeoTransform[6];
    poDataset->GetGeoTransform( adfGeoTransform );
    adfGeoTransform[0] += StartRow * adfGeoTransform[1];
    adfGeoTransform[3] += StartColumn * adfGeoTransform[5];

    if (top == true && left == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        adfGeoTransform[0] -= Border*adfGeoTransform[1];
        adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(top == true && right == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        adfGeoTransform[0] -= Border*adfGeoTransform[1];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(bottom == true && left == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        adfGeoTransform[0] -= Border*adfGeoTransform[1];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(bottom == true && right == true){

    }else if(top == true){

    }else if(bottom == true){

    }else if(left == true){

    }else if(right == true){

    }else{
        writeTileData(StartRow, StartColumn, nXSize, nYSize, TileData, adfGeoTransform, poDataset, "Border");
    }


}

void dataManager::writeTileData(int StartRow, int StartColumn, int NbrRows, int NbrColumns,
                   float * TileData,
                   double adfGeoTransform[6],
                   GDALDataset * srcDataset,
                   std::string prefix){

    const char * pszFormat = "GTiff";
    GDALDriver * poDriver;
    //char **papszMetadata;
    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    if( poDriver == NULL )
        exit( 1 );
    //papszMetadata = poDriver->GetMetadata();

    GDALDataset * poDstDataset;
    char **papszOptions = NULL;

    std::string FileName ("/home/peter/Desktop/TestData/test");
    std::string FileSuffix (".tif");
    std::stringstream s1, s2;
    std::string StartRowStr, StartColumnStr;

    if (StartRow == 0){
        StartRowStr = "000";
    }else{
        s1 << StartRow;
        StartRowStr = s1.str();
    }
    if (StartColumn == 0){
        StartColumnStr = "000";
    }else{
        s2 << StartColumn;
        StartColumnStr = s2.str();
    }

    FileName += prefix + StartRowStr + StartColumnStr + FileSuffix;

    poDstDataset = poDriver->Create( FileName.c_str(), NbrRows, NbrColumns, 1, GDT_Byte,
                                papszOptions );

    GDALRasterBand *poDstBand;
    poDstDataset->SetGeoTransform( adfGeoTransform );
    poDstDataset->SetProjection(srcDataset->GetProjectionRef());

    poDstBand = poDstDataset->GetRasterBand(1);

    poDstBand->RasterIO( GF_Write, 0, 0, NbrRows, NbrColumns,
                      TileData, NbrRows, NbrColumns, GDT_Float32, 0, 0 );

    GDALClose( (GDALDatasetH) poDstDataset );

}

dataManager::dataManager()
{

}

