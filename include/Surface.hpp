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

#ifndef _INCLUDE_PATHTRACER_SURFACE_H_
#define _INCLUDE_PATHTRACER_SURFACE_H_

#include "Light.hpp"
#include "Utils.hpp"

/* A container for a render result */
class Surface {
    public:
        Surface(unsigned width, unsigned height);
        virtual ~Surface();

        unsigned getWidth();
        unsigned getHeight();

        Color* operator[](unsigned i);
        void toPPM(const char* filename);

        void applyGammaCorrection(Real gamma);
        void clear();

    private:
        unsigned width;
        unsigned height;

        Color** color;
};

#endif // _INCLUDE_PATHTRACER_SURFACE_H_
