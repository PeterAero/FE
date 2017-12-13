#include "datamanager.h"



void dataManager::tiledata(GDALDataset * srcDataset, int Border){

    // iterate over Image
    int TileSize = 200;
    int StartRow = 0, EndRow = 0, StartColumn = 0, EndColumn = 0;

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

            readTileData(StartRow, StartColumn, EndRow, EndColumn, srcDataset, Border);

        }

    }

    GDALClose( (GDALDatasetH) srcDataset );
}

/*
void dataManager::tiledata(GDALDataset * srcDataset){

    // iterate over Image
    int TileSize = 400;
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
*/
/*
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
*/
void dataManager::readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn,
                               GDALDataset * poDataset, int Border){

    float * TileData;
    GDALRasterBand *poBand;
    poBand = poDataset->GetRasterBand( 1 );

    int nXSize = (EndRow - StartRow) + 2 * Border;
    int nYSize = (EndColumn - StartColumn) + 2 * Border;

    TileData = (float *) CPLMalloc(sizeof(float)*nXSize*nYSize);
    for(int i = 0; i < nXSize*nYSize; i++){
        TileData[i] = 0.;
    }

    if(Border != 0){

        if(StartRow != 0 && StartColumn != 0 && EndRow != poDataset->GetRasterXSize() && EndColumn != poDataset->GetRasterYSize()){

            poBand->RasterIO( GF_Read, StartRow - Border, StartColumn - Border, nXSize, nYSize,
                              TileData, nXSize, nYSize, GDT_Float32,
                              0, 0 );
        }else if(StartRow == 0 && StartColumn == 0){
            float * tmpTileData;
            int nXSize = (EndRow - StartRow) + 1 * Border;
            int nYSize = (EndColumn - StartColumn) + 1 * Border;
            tmpTileData = (float *) CPLMalloc(sizeof(float)*nXSize*nYSize);
            poBand->RasterIO( GF_Read, StartRow, StartColumn, nXSize, nYSize,
                              TileData, nXSize, nYSize, GDT_Float32,
                              0, 0 );
            //TODO overwrite TileData with tmpTileData
            for(int i = 0; i < nXSize*nYSize; i++){
                TileData[i] = 0.;
            }

        }

    }else{

        poBand->RasterIO( GF_Read, StartRow, StartColumn, nXSize, nYSize,
                          TileData, nXSize, nYSize, GDT_Float32,
                          0, 0 );
    }

    double adfGeoTransform[6];
    poDataset->GetGeoTransform( adfGeoTransform );
    adfGeoTransform[0] += (StartRow - Border) * adfGeoTransform[1] ;
    adfGeoTransform[3] += (StartColumn - Border) * adfGeoTransform[5];

    if (Border == 0){
        writeTileData(StartRow, StartColumn, nXSize, nYSize, TileData, adfGeoTransform, poDataset, "NoBorder");
    }else{
        writeTileData(StartRow, StartColumn, nXSize, nYSize, TileData, adfGeoTransform, poDataset, "Border");
    }

}

/*
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

    //poBand->RasterIO( GF_Read, StartRow, StartColumn, nXSize, nYSize,
    //                  TileData, nXSize, nYSize, GDT_Float32,
    //                  0, 0 );

    double adfGeoTransform[6];
    poDataset->GetGeoTransform( adfGeoTransform );
    adfGeoTransform[0] += (StartRow * adfGeoTransform[1]);
    adfGeoTransform[3] += (StartColumn * adfGeoTransform[5]);

    if (top == true && left == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        //adfGeoTransform[0] -= Border*adfGeoTransform[1];
        //adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(top == true && right == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        //adfGeoTransform[0] -= Border*adfGeoTransform[1];
        //adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(bottom == true && left == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        //adfGeoTransform[0] -= Border*adfGeoTransform[1];
        //adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(bottom == true && right == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        //adfGeoTransform[0] -= Border*adfGeoTransform[1];
        //adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(top == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        //adfGeoTransform[0] -= Border*adfGeoTransform[1];
        //adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(bottom == true){

        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        //adfGeoTransform[0] -= Border*adfGeoTransform[1];
        //adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(left == true){
        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        //adfGeoTransform[0] -= Border*adfGeoTransform[1];
        //adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else if(right == true){
        float * TileDataBuffer;
        TileDataBuffer = (float *) CPLMalloc(sizeof(float)* (nXSize + Border) * (nYSize + Border));
        for(int i = 0; i < nXSize*nYSize; i++){
            TileDataBuffer[i] = 0;
        }
        //adfGeoTransform[0] -= Border*adfGeoTransform[1];
        //adfGeoTransform[3] -= Border*adfGeoTransform[5];
        writeTileData(StartRow, StartColumn, nXSize + Border, nYSize + Border, TileDataBuffer, adfGeoTransform, poDataset, "Border");

    }else{
        writeTileData(StartRow, StartColumn, nXSize, nYSize, TileData, adfGeoTransform, poDataset, "Border");
    }


}
*/
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

