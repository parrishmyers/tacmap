//
//  DT_List.h
//  tacmap
//
//  Created by Parrish Myers on 6/25/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#ifndef DT_List_h
#define DT_List_h

template<typename T, int Size>
class List {
    T data[Size];
    int size;
    int len;
    
    List<>() : size(Size), len(0) { }
    
    void clear() {
        for (int i = 0; i < size) {
            data[i] = T();
        }
    }
};
#endif /* DT_List_h */
