//
//  main.cpp
//  tacmap_cmd
//
//  Created by Parrish Myers on 4/7/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#include <cstdio>

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "boost/filesystem.hpp"

#include "DelaunayTriangulation.h"

static const std::string DataPath = {"/Users/pmyers/Projects/tacmap/data/50n000e_20101117_gmted_bln075.tif"};

int main(int argc, const char * argv[]) {
    boost::filesystem::path p = boost::filesystem::current_path();
    
    fprintf(stdout,"cwp := %s\n",p.c_str());
    
    GDALAllRegister();
    
//    for (int i = 0; i < GDALGetDriverCount(); i++) {
//        GDALDriver * d = (GDALDriver *)GDALGetDriver(i);
//        const char * desc = d->GetDescription();
//        fprintf(stdout, "GDAL: %s\n",desc);
//    }
    
    GDALDataset *poDataset = (GDALDataset *) GDALOpen(DataPath.c_str(), GA_ReadOnly );
    std::string SupportedDriver = {"GTiff"};
    
    if (poDataset != NULL) {
        GDALDriver * drv = poDataset->GetDriver();
        assert(0 == SupportedDriver.compare(drv->GetDescription()));
        assert (1 == poDataset->GetRasterCount());
        
        fprintf(stdout,"RasterXSize := %d\n",poDataset->GetRasterXSize());
        fprintf(stdout,"RasterYSize := %d\n",poDataset->GetRasterYSize());
        fprintf(stdout,"ProjectionRef := %s\n",poDataset->GetProjectionRef());

        double adfGeoTransform[6];
        if ( poDataset->GetGeoTransform(adfGeoTransform) == CE_None ) {
            fprintf(stdout, "Origin = (%.6f, %.6f)\n",
                    adfGeoTransform[0],adfGeoTransform[3]); // upper left courner
            fprintf(stdout, "Pixel Size = (%.6f, %.6f)\n",
                    adfGeoTransform[1],adfGeoTransform[5]); // pixel width/height
        }
        
        GDALRasterBand *poBand = poDataset->GetRasterBand(1);
        int nBlockXSize, nBlockYSize;
        poBand->GetBlockSize(&nBlockXSize, &nBlockYSize);
        
        std::string SupportedDataType = {"Int16"};
        assert (GDT_Int16 == poBand->GetRasterDataType());
        
        printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
               nBlockXSize, nBlockYSize,
               GDALGetDataTypeName(poBand->GetRasterDataType()),
               GDALGetColorInterpretationName(
               poBand->GetColorInterpretation()) );
        
        FILE * patch = fopen("patch.txt","w");
        for (int i = 0; i < poBand->GetYSize(); i++) {
            int nXSize = poBand->GetXSize();
            float *pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
            poBand->RasterIO( GF_Read, 0, 0, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0);
            
            for (int j = 0; j < nXSize; j++) {
                fprintf(patch, "%f  ", pafScanline[j]);
            }
            fprintf(patch, "\n");
            
            CPLFree(pafScanline);
        }
        fclose(patch);
        
        GDALClose(poDataset);
    }
    
    DelaunayTriangulation dt;
    
    //std::srand(static_cast<unsigned int>(std::time(0))); // use current time as seed for random generator
    /*
    std::srand(static_cast<unsigned int>(3652123216145));
    for (int i = 0; i < 10 ; i++) {
        double x = 180.0 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
        double y = 180.0 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
        
        dt.addPt(x, y, 0.0);
    }
    */
    
    /*
    dt.addPt(-0.02222276248244826, -0.4979727817680433,  0.0);
    dt.addPt(-0.4285431913366012,   0.4745826469497594,  0.0);
    dt.addPt( 0.3105396575392593,   0.2400179190933871,  0.0);
    dt.addPt(-0.01883958887200765,  0.3630260628303755,  0.0);
    dt.addPt( 0.3790312361708201,   0.3779794437605696,  0.0);
    dt.addPt(-0.2994955874043476,   0.3776609263174803,  0.0);
    dt.addPt( 0.3471817493878135,   0.08365533089605659, 0.0);
    dt.addPt(-0.00485819764887746,  0.3482682405489201,  0.0);
    dt.addPt( 0.3443122672329771,  -0.1437312230875075,  0.0);
    dt.addPt( 0.309330780347186,   -0.07758103877080702, 0.0);
    
    dt.compute();
     */
    
    return 0;
}
