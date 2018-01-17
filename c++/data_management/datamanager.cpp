#include "datamanager.h"



int dataManager::getTilePosition(int StartRow, int StartColumn, int EndRow, int EndColumn,
                                  int NbrColumns, int NbrRows){

    /* Find out whether a tile is neighbouring to a border and if yes which border
     *
     *
     *
     *
     */

    enum BorderType {NoBorder = 0, Left = 1, Right = 2, Up = 3, Down = 4,
                     UpLeft = 5, UpRight = 6, LowLeft = 7, LowRight = 8};

    BorderType MyBorder;

    // if no NoData-Space around the Tile
    if(StartRow != 0 && StartColumn != 0 && EndRow != NbrRows && EndColumn != NbrColumns){
        MyBorder = NoBorder;
    // if upper left corner Tile
    }else if(StartRow == 0 && StartColumn == 0){
        MyBorder = UpLeft;
    // if upper tile, no left or right corner
    }else if(StartRow == 0 && StartColumn != 0 && EndColumn != NbrColumns){
        MyBorder = Up;
    // if upper right corner
    }else if(StartRow == 0 && StartColumn != 0 && EndColumn == NbrColumns){
        MyBorder = UpRight;
    // if left border
    }else if(StartRow != 0 && StartColumn == 0 && EndColumn != NbrColumns && EndRow != NbrRows){
        MyBorder = Left;
    // if right boarder
    }else if(StartRow != 0 && StartColumn != 0 && EndColumn == NbrColumns && EndRow != NbrRows){
        MyBorder = Right;
    // if lower border
    }else if(StartRow != 0 && EndRow == NbrRows && StartColumn != 0 && EndColumn != NbrColumns){
        MyBorder = Down;
        // if lower left corner
     }else if(StartColumn == 0 && EndRow == NbrRows){
        MyBorder = LowLeft;
    // if lower right corner
    }else if(StartRow != 0 && StartColumn != 0 && EndColumn == NbrColumns && EndRow == NbrRows){
        MyBorder = LowRight;
    }

    return MyBorder;

}

void dataManager::tileData(GDALDataset * srcDataset, int Border){

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


}



void dataManager::readTileData(int StartRow, int StartColumn, int EndRow, int EndColumn,
                               GDALDataset * poDataset, int Border){

    float * TileData;
    GDALRasterBand * poBand;
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

        int BorderPosition = this->getTilePosition(StartRow, StartColumn, EndRow, EndColumn, NbrColumns, NbrRows);

        if(BorderPosition == 0){

            poBand->RasterIO( GF_Read, StartColumn - Border, StartRow - Border, NbrColumnsBorderTile, NbrRowsBorderTile,
                              TileData, NbrColumnsBorderTile, NbrRowsBorderTile, GDT_Float32,
                              0, 0 );

        }else{

            float * tmpTileData;
            int Index = 0;
            int tmpRowSize = 0, tmpColumnSize = 0;
            int tmpStartRow = StartRow, tmpStartColumn = StartColumn;
            int tmpIterRowStart = 0, tmpIterColStart = 0;
            int tmpIterRowStop = NbrRowsBorderTile, tmpIterColStop = NbrColumnsBorderTile;

            if(BorderPosition == 1 || BorderPosition == 2){
                tmpRowSize = (EndRow - StartRow) + 2 * Border;
                tmpColumnSize = (EndColumn - StartColumn) + 1 * Border;
            }else if (BorderPosition == 3 || BorderPosition == 4){
                tmpRowSize = (EndRow - StartRow) + 1 * Border;
                tmpColumnSize = (EndColumn - StartColumn) + 2 * Border;
            }else{
                tmpRowSize = (EndRow - StartRow) + 1 * Border;
                tmpColumnSize = (EndColumn - StartColumn) + 1 * Border;
            }

            switch (BorderPosition){
            /*
             * 1 = Left
             * 2 = Right
             * 3 = Top
             * 4 = Down
             * 5 = UpLeft
             * 6 = UpRight
             * 7 = DownLeft
             * 8 = DownRight
            */
            case 1:
                tmpStartRow -=Border;
                tmpIterColStart += Border;
                break;
            case 2:
                tmpStartColumn -= Border;
                tmpStartRow -= Border;
                tmpIterColStop -= Border;
                break;
            case 3:
                tmpStartColumn -= Border;
                tmpIterRowStart += Border;
                break;
            case 4:
                tmpStartColumn -= Border;
                tmpStartRow -= Border;
                break;
            case 5:
                tmpStartColumn = StartColumn;
                tmpIterRowStart += Border;
                tmpIterColStart += Border;
                break;
            case 6:
                tmpStartColumn -= Border;
                tmpIterRowStart += Border;
                tmpIterColStop -= Border;
                break;
            case 7:
                tmpStartRow -= Border;
                tmpIterColStart += Border;
                break;
            case 8:
                tmpStartColumn -= Border;
                tmpStartRow -= Border;
                tmpIterColStop -= Border;
                break;
            }
            tmpTileData = (float *) CPLMalloc(sizeof(float) * tmpRowSize * tmpColumnSize);
            poBand->RasterIO( GF_Read, tmpStartColumn, tmpStartRow, tmpColumnSize, tmpRowSize,
                              tmpTileData, tmpColumnSize, tmpRowSize, GDT_Float32,
                              0, 0 );


            for(int i = tmpIterRowStart; i < tmpIterRowStop; i++){

                for(int j = tmpIterColStart; j < tmpIterColStop; j++){

                    Index = j + i * NbrColumnsBorderTile;
                    TileData[Index] = tmpTileData[j - tmpIterColStart + ((i - tmpIterRowStart)*tmpColumnSize)];

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

    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    if( poDriver == NULL )
        exit( 1 );

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

    poDstDataset = poDriver->Create( FileName.c_str(), NbrColumns, NbrRows, 1, GDT_Byte,
                                papszOptions );

    GDALRasterBand * poDstBand;
    poDstDataset->SetGeoTransform( adfGeoTransform );
    poDstDataset->SetProjection(srcDataset->GetProjectionRef());

    poDstBand = poDstDataset->GetRasterBand(1);

    poDstBand->RasterIO( GF_Write, 0, 0, NbrColumns, NbrRows,
                      TileData, NbrColumns, NbrRows, GDT_Float32, 0, 0 );

    if(prefix == "Border"){
        this->FileListBorder.push_back(poDstDataset);

    }else{
        this->FileListNoBorder.push_back(poDstDataset);
    }


}


dataManager::dataManager(char * FileName, int BorderSize)
{

    this->BorderSize = BorderSize;
    this->FileName = FileName;

    GDALAllRegister();
    GDALDataset  * srcDataset;
    srcDataset = (GDALDataset *) GDALOpen( this->FileName, GA_ReadOnly );

    this->tileData(srcDataset, 0);
    this->tileData(srcDataset, BorderSize);
    GDALClose( (GDALDatasetH) srcDataset );
}

