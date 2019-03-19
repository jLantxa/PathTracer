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

#ifndef _INCLUDE_PATHTRACER_LIGHT_H_
#define _INCLUDE_PATHTRACER_LIGHT_H_

#include <cstdint>
#include "Common.hpp"
#include "Vector3D.hpp"

uint8_t toColorInt(Real component);
uint32_t colorGetARGB(Vec3D& v);
Real colorClamp(Real x);

class Ray {
    public:
        /**
         * A source point and the direction of propagation.
         * If any of the vectors are not unit they will be normalised.
         */
        Ray(Vec3D origin_v, Vec3D direction_v);
        virtual ~Ray();

        /**
         * Get a point in the trajectory of the ray. The parameter
         * t represents the distance from the origin.
         */
        Vec3D point(Real t);

        /** Return the origin vector of the ray */
        Vec3D getOrigin();

        /** Return the direction vector of the ray */
        Vec3D getDirection();

    private:
        /** Origin vector */
        Vec3D mOrigin_v;
        Vec3D mDirection_v;
};

class LightSource {
    public:
        LightSource(Vec3D position_v, Vec3D color);
        ~LightSource();

        Vec3D getPosition();
        Vec3D getColor();

    private:
        Vec3D mPosition_v;
        Vec3D color;
};

struct Material {
    float emission = 0;
};

#endif // _INCLUDE_PATHTRACER_LIGHT_H_
