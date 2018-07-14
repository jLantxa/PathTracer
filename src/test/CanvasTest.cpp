/*
 * This source file is part of PathTracer
 *
 * Copyright 2018 Javier Lancha Vázquez
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "PathTracer.hpp"
#include "Canvas.hpp"

#include <assert.h>
#include <iostream>

int main (int argc, char* argv[]) {
    unsigned width, height;

    if (argc != 3) {
        std::cout << "ERROR: Specify canvas size." << std::endl;
        return -1;
    } else {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        if (width <= 0 || height <= 0) {
            std::cout << "ERROR: Canvas cannot have null size." << std::endl;
            return -1;
        } else if (width*height > 10000 * 10000) {
            std::cout << "ERROR: You specified a canvas of size " <<
                width << "x" << height << "." << std::endl <<
                "You do not need to test a canvas that big..." << std::endl;
            return -1;
        }
    }
    
    Canvas* canvas = new Canvas(width, height);

    // Write
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            (*canvas)[i][j] = i^j;
        }
    }

    // Read
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            assert((*canvas)[i][j] == (i^j));
        }
    }

    std::cout << "Test passed" << std::endl;

    return 0;
}