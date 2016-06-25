//
//  DT_Constants.h
//  tacmap_cmd
//
//  Created by Parrish Myers on 5/25/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#ifndef DT_Constants_h
#define DT_Constants_h

class Constants {
public:
    constexpr static const long DAG_SIZE = 3000;
    constexpr static const long POOL_SIZE = 12000;
    constexpr static const double DIST_THRESH = 0.1f;
    constexpr static const double VECT_THRESH = 0.001;
    constexpr static const int splitListSize = 4;
    constexpr static const int adjListSize = 8;
};

#endif /* DT_Constants_h */
