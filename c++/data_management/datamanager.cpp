#include "datamanager.h"



void dataManager::tiledata(GDALDataset * srcDataset, int Border){

    // iterate over Image
    int TileSize = 200;
    int StartRow = 0, EndRow = 0, StartColumn = 0, EndColumn = 0;

    int NbrColumns = srcDataset->GetRasterXSize();
    int NbrRows = srcDataset->GetRasterYSize();

    for(int i = 0; i < NbrRows; i += TileSize){
        for(int j = 0; j < NbrColumns; j += TileSize){

            StartRow = i;
            StartColumn = j;

            if(NbrColumns < StartColumn + TileSize){
                EndColumn = NbrColumns;
            }else{
                EndColumn = StartColumn + TileSize;
            }

            if(NbrRows < StartRow + TileSize){
                EndRow = NbrRows;
            }else{
                EndRow = StartRow + TileSize;
            }

            readTileData(StartRow, StartColumn, EndRow, EndColumn, srcDataset, Border);

        }

    }

    GDALClose( (GDALDatasetH) srcDataset );
}



void dataManager::readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn,
                               GDALDataset * poDataset, int Border){

    float * TileData;
    GDALRasterBand *poBand;
    poBand = poDataset->GetRasterBand( 1 );

    int NbrRowsBorderTile = (EndRow - StartRow) + 2 * Border;
    int NbrColumnsBorderTile = (EndColumn - StartColumn) + 2 * Border;

    int NbrColumns = poDataset->GetRasterXSize();
    int NbrRows = poDataset->GetRasterYSize();

    TileData = (float *) CPLMalloc(sizeof(float)*NbrRowsBorderTile*NbrColumnsBorderTile);
    for(int i = 0; i < NbrRowsBorderTile*NbrColumnsBorderTile; i++){
        TileData[i] = 0.;
    }

    if(Border != 0){

        // if no NoData-Space around the Tile
        if(StartRow != 0 && StartColumn != 0 && EndRow != poDataset->GetRasterYSize() && EndColumn != poDataset->GetRasterXSize()){

            poBand->RasterIO( GF_Read, StartColumn - Border, StartRow - Border, NbrColumnsBorderTile, NbrRowsBorderTile,
                              TileData, NbrColumnsBorderTile, NbrRowsBorderTile, GDT_Float32,
                              0, 0 );

        // if upper left corner Tile
        }else if(StartRow == 0 && StartColumn == 0){
            float * tmpTileData;
            int tmpRowSize = (EndRow - StartRow) + 1 * Border;
            int tmpColumnSize = (EndColumn - StartColumn) + 1 * Border;
            tmpTileData = (float *) CPLMalloc(sizeof(float)*tmpRowSize*tmpColumnSize);
            poBand->RasterIO( GF_Read, StartColumn, StartRow, tmpColumnSize, tmpRowSize,
                              tmpTileData, tmpColumnSize, tmpRowSize, GDT_Float32,
                              0, 0 );

            int Index = 0;

            for(int i = Border; i < NbrRowsBorderTile; i++){

                for(int j = Border; j < NbrColumnsBorderTile; j++){

                    Index = j + i * NbrColumnsBorderTile;
                    TileData[Index] = tmpTileData[j - Border + ((i - Border)*tmpColumnSize)];

                }

            }
        // if upper tile, no left or right corner
        }else if(StartRow == 0 && StartColumn != 0 && EndColumn != NbrColumns){
            float * tmpTileData;
            int tmpRowSize = (EndRow - StartRow) + 1 * Border;
            int tmpColumnSize = (EndColumn - StartColumn) + 2 * Border;
            tmpTileData = (float *) CPLMalloc(sizeof(float) * tmpRowSize * tmpColumnSize);
            poBand->RasterIO( GF_Read, StartColumn - Border, StartRow, tmpColumnSize, tmpRowSize,
                              tmpTileData, tmpColumnSize, tmpRowSize, GDT_Float32,
                              0, 0 );

            int Index = 0;

            for(int i = Border; i < NbrRowsBorderTile; i++){

                for(int j = 0; j < NbrColumnsBorderTile; j++){

                    Index = j + i * NbrColumnsBorderTile;
                    TileData[Index] = tmpTileData[j + ((i - Border)*tmpColumnSize)];

                }

            }
        // if upper right corner
        }else if(StartRow == 0 && StartColumn != 0 && EndColumn == NbrColumns){

            float * tmpTileData;
            int tmpRowSize = (EndRow - StartRow) + 1 * Border;
            int tmpColumnSize = (EndColumn - StartColumn) + 1 * Border;
            tmpTileData = (float *) CPLMalloc(sizeof(float)*tmpRowSize*tmpColumnSize);
            poBand->RasterIO( GF_Read, StartColumn - Border, StartRow, tmpColumnSize, tmpRowSize,
                              tmpTileData, tmpColumnSize, tmpRowSize, GDT_Float32,
                              0, 0 );

            int Index = 0;

            for(int i = Border; i < NbrRowsBorderTile; i++){

                for(int j = 0; j < NbrColumnsBorderTile - Border; j++){

                    Index = j + i * NbrColumnsBorderTile;
                    TileData[Index] = tmpTileData[j - Border + ((i - Border)*tmpColumnSize)];

                }

            }

        }else if(StartRow != 0 && StartColumn == 0 && EndColumn != NbrColumns && EndRow != NbrRows){
            float * tmpTileData;
            int tmpRowSize = (EndRow - StartRow) + 2 * Border;
            int tmpColumnSize = (EndColumn - StartColumn) + 1 * Border;
            tmpTileData = (float *) CPLMalloc(sizeof(float) * tmpRowSize * tmpColumnSize);
            poBand->RasterIO( GF_Read, StartColumn, StartRow - Border, tmpColumnSize, tmpRowSize,
                              tmpTileData, tmpColumnSize, tmpRowSize, GDT_Float32,
                              0, 0 );

            int Index = 0;

            for(int i = 0; i < NbrRowsBorderTile; i++){

                for(int j = Border; j < NbrColumnsBorderTile; j++){

                    Index = j + i * NbrColumnsBorderTile;
                    TileData[Index] = tmpTileData[j - Border + (i*tmpColumnSize)];

                }

            }

        }else if(StartRow != 0 && StartColumn != 0 && EndColumn == NbrColumns && EndRow != NbrRows){
            float * tmpTileData;
            int tmpRowSize = (EndRow - StartRow) + 2 * Border;
            int tmpColumnSize = (EndColumn - StartColumn) + 1 * Border;
            tmpTileData = (float *) CPLMalloc(sizeof(float) * tmpRowSize * tmpColumnSize);
            poBand->RasterIO( GF_Read, StartColumn - Border, StartRow, tmpColumnSize, tmpRowSize,
                              tmpTileData, tmpColumnSize, tmpRowSize, GDT_Float32,
                              0, 0 );

            int Index = 0;

            for(int i = 0; i < NbrRowsBorderTile; i++){

                for(int j = 0; j < NbrColumnsBorderTile - Border; j++){

                    Index = j + i * NbrColumnsBorderTile;
                    TileData[Index] = tmpTileData[j + (i*tmpColumnSize)];

                }

            }

        }


    }else{

        poBand->RasterIO( GF_Read, StartColumn, StartRow, NbrColumnsBorderTile, NbrRowsBorderTile,
                          TileData, NbrColumnsBorderTile, NbrRowsBorderTile, GDT_Float32,
                          0, 0 );
    }

    double adfGeoTransform[6];
    poDataset->GetGeoTransform( adfGeoTransform );
    adfGeoTransform[0] += (StartColumn - Border) * adfGeoTransform[1] ;
    adfGeoTransform[3] += (StartRow - Border) * adfGeoTransform[5];

    if (Border == 0){
        writeTileData(StartRow, StartColumn, NbrRowsBorderTile, NbrColumnsBorderTile, TileData, adfGeoTransform, poDataset, "NoBorder");
    }else{
        writeTileData(StartRow, StartColumn, NbrRowsBorderTile, NbrColumnsBorderTile, TileData, adfGeoTransform, poDataset, "Border");
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

    std::string FileName ("/home/fisc_p0/Desktop/TestData/test");
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

    poDstDataset = poDriver->Create( FileName.c_str(), NbrColumns, NbrRows, 1, GDT_Byte,
                                papszOptions );

    GDALRasterBand *poDstBand;
    poDstDataset->SetGeoTransform( adfGeoTransform );
    poDstDataset->SetProjection(srcDataset->GetProjectionRef());

    poDstBand = poDstDataset->GetRasterBand(1);

    poDstBand->RasterIO( GF_Write, 0, 0, NbrColumns, NbrRows,
                      TileData, NbrColumns, NbrRows, GDT_Float32, 0, 0 );

    GDALClose( (GDALDatasetH) poDstDataset );

}

dataManager::dataManager()
{

}

