//
//  DT_List.h
//  tacmap
//
//  Created by Parrish Myers on 6/25/16.
//  Copyright © 2016 Parrish Myers. All rights reserved.
//

#ifndef DT_List_h
#define DT_List_h

#include <stdio.h>

#include "DT_Triangle.h"

template<int Size>
class TriangleList {
public:
    Triangle * data[Size];
    int size = Size;
    int len = 0;

    TriangleList<>() : size(Size), len(0) { }
    
    void copy(TriangleList<Size> & tl) {
        clear();
        for (int i = 0; i < tl.len; i++) {
            add(tl[i]);
        }
    }
    
    void clear() {
        for (int i = 0; i < size; i++) {
            data[i] = nullptr;
        }
        len = 0;
    }
    
    bool inList(Triangle *a) {
        bool result = false;
        for (int i = 0; i < len; i++) {
            if (a == data[i])
                result = true;
        }
        return result;
    }
    
    void add(Triangle * a) {
        if (len < size) {
            if (false == inList(a)) {
                data[len] = a;
                len += 1;
            }
        } else {
            fprintf(stdout, "ERROR: TriangleList full\n");
            assert(false);
        }
    }
    
    Triangle * operator[] (int i) {
        if (0 <= i && i < size)
            return data[i];
        else
            return nullptr;
    }
};
#endif /* DT_List_h */
