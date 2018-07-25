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

#ifndef _INCLUDE_RAYTRACER_LIGHT_H_
#define _INCLUDE_RAYTRACER_LIGHT_H_

#include <cstdint>
#include "Common.hpp"
#include "Geometry.hpp"

/** ARGB color class */
class Color {
    public:
        float A, R, G, B;

        Color();
        Color(float r, float g, float b);
        Color(float a, float r, float g, float b);
        ~Color();

        /** Copy argb components from a color object */
        void set(Color& color);
        /** Copy RGB components from a color object */
        void setRGB(Color& color);
        /** Set RGB components. A remains the same. */
        void set(float r, float g, float b);
        /** Set ARGB components */
        void set(float a, float r, float g, float b);

        uint8_t intA();
        uint8_t intR();
        uint8_t intG();
        uint8_t intB();

        /** Return ARGB value as int32_t */
        uint32_t getARGB();

    private:
        float clamp(float x);
};

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
        LightSource(Vec3D position_v, Color color);
        ~LightSource();

        Vec3D getPosition();
        Color getColor();
    
    private:
        Vec3D mPosition_v;
        Color color;
};

#endif // _INCLUDE_RAYTRACER_LIGHT_H_
