/*
 * This source file is part of raytracer
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

#ifndef _INCLUDE_RAYTRACER_RAYTRACER_H_
#define _INCLUDE_RAYTRACER_RAYTRACER_H_

#include "Objects.hpp"
#include "Light.hpp"
#include "Camera.hpp"

#include <vector>

/** A container of objects and light sources */
struct Scene {
    std::vector<Object3D*> objects;
    std::vector<LightSource*> lights;
};

/* A container for an ARGB surface */
class Canvas {
    public:
        Canvas();
        Canvas(unsigned width, unsigned height);
        virtual ~Canvas();
        unsigned width;
        unsigned height;

        int& operator[](unsigned i);
        void resize(unsigned newWidth, unsigned newHeight);

    private:
        int** rgb;
        void freeRGB();
        void initRGB(unsigned width, unsigned height);
};

class RayTracer {
    public:
        RayTracer();
        virtual ~RayTracer();
        static void renderScene(struct Scene& scene, Camera& camera, struct Canvas* canvas);
    
    private:
        static Color castRayFromPixel(Ray& ray, struct Scene& scene);
};

#endif // _INCLUDE_RAYTRACER_RAYTRACER_H_