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
        
        GDALClose(poDataset);
    }
    
    
    return 0;
}
