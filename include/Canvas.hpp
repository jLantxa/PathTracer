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

#ifndef _INCLUDE_RAYTRACER_CANVAS_H_
#define _INCLUDE_RAYTRACER_CANVAS_H_

#include "Light.hpp"

/* A container for an render result */
class Canvas {
    public:
        Canvas(unsigned width, unsigned height);
        virtual ~Canvas();
        
        unsigned getWidth();
        unsigned getHeight();

        Vec3D* operator[](unsigned i);
        void toPPM(const char* filename);

        unsigned spp = 1;

    private:
        unsigned width;
        unsigned height;

        Vec3D** color;
};

#endif // _INCLUDE_RAYTRACER_CANVAS_H_