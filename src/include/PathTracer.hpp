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

#ifndef _INCLUDE_RAYTRACER_RAYTRACER_H_
#define _INCLUDE_RAYTRACER_RAYTRACER_H_

#include "Common.hpp"
#include "Canvas.hpp"
#include "Objects.hpp"
#include "Light.hpp"
#include "Camera.hpp"

#include <cstdint>
#include <vector>

/** A container of objects and light sources */
struct Scene {
    std::vector<Object3D*> objects;
    std::vector<LightSource*> lights;
    Color backgroundColor;
};

class PathTracer {
    public:
        PathTracer(unsigned depth);
        virtual ~PathTracer();
        Canvas* renderScene(unsigned spp, struct Scene& scene, Camera& camera);
    
    private:
        Vec3D traceRay(unsigned depth, Ray& ray, struct Scene& scene);
        Object3D* intersect(Ray& ray, Scene& scene, Real& t);
        unsigned mMaxDepth;

        // Random seed for erand48
        uint16_t Xi[3];
};

#endif // _INCLUDE_RAYTRACER_RAYTRACER_H_