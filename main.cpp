#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "datamanager.h"
#include "imageproc.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/filesystem.hpp>


using namespace std;

void get_all(const boost::filesystem::path& root, const string& ext, vector<boost::filesystem::path>& ret)
{
    if(!boost::filesystem::exists(root) || !boost::filesystem::is_directory(root)) return;

    boost::filesystem::recursive_directory_iterator it(root);
    boost::filesystem::recursive_directory_iterator endit;


    while(it != endit)
    {
        if(boost::filesystem::is_regular_file(*it) && it->path().extension() == ext){

            ret.push_back(it->path().filename());
            //std::cout <<  it->path().filename() << std::endl;

        }
        ++it;

    }


}


void createComposite(const vector<boost::filesystem::path>& ret){

    // first -> get Extent

    for(int i = 0; i < ret.size(); i++){

        std::cout <<  ret[i] << std::endl;

    }

    // second -> create Output Image

    // third -> write Output Image

}


int main(int argc, char *argv[])
{

    const boost::filesystem::path PathOfTiff("/home/fisc_p0/04_Praktikanten/Peccia_Federico/Input");

    const string MyString(".tif");

    vector<boost::filesystem::path> ret;

    get_all(PathOfTiff, MyString, ret);

    createComposite(ret);


/*
    char * MyTif = "/home/peter/Desktop/TestData/SP27GTIF.TIF";
    int BorderSize = 5;

    dataManager MyDataManager(MyTif, BorderSize);

    imageProc MyImageProcessor;

    MyImageProcessor.filterImg(MyDataManager.FileListBorder[0]);
*/




    return 0;
}
