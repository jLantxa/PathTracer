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

#include <cstdint>

#include "Common.hpp"
#include "Light.hpp"
#include "Geometry.hpp"

Color::Color() : A(1.0), R(0.0), G(0.0), B(0.0) { }

Color::Color(float r, float g, float b) : A(1.0), R(r), G(g), B(b) { }

Color::Color(float a, float r, float g, float b) : A(a), R(r), G(g), B(b) { }

Color::~Color() { }

void Color::set(Color& color) {
    A = color.A;
    R = color.R;
    G = color.G;
    B = color.B;
}

void Color::setRGB(Color& color) {
    R = color.R;
    G = color.G;
    B = color.B;
}

void Color::set(float r, float g, float b) {
    R = r;
    G = g;
    B = b;
}

void Color::set(float a, float r, float g, float b) {
    A = a;
    R = r;
    G = g;
    B = b;
}

uint32_t Color::getARGB() {
    uint32_t argb = 0x0;
    // Valid values from 0 to 1.0
    argb += (static_cast<int>(clamp(A) * 0xFF) << 24) & 0xFF000000;
    argb += (static_cast<int>(clamp(R) * 0xFF) << 16) & 0x00FF0000;
    argb += (static_cast<int>(clamp(G) * 0xFF) <<  8) & 0x0000FF00;
    argb += (static_cast<int>(clamp(B) * 0xFF))       & 0x000000FF;
    return argb;
}

float Color::clamp(float x) {
    if (x < 0) return 0;
    else if (x > 1) return 1;
    else return x;
}


Ray::Ray(Vec3D o, Vec3D dir) :
    origin(o),
    direction(dir.normalize()) { }

Ray::~Ray() {}

Vec3D Ray::getOrigin() {
    return origin;
}

Vec3D Ray::getDirection() {
    return direction;
}

Vec3D Ray::point(Real t) {
    return origin + t*direction;
}


LightSource::LightSource(Vec3D position, Color color) :
    position(position), color(color) { }

LightSource::~LightSource() { }

Vec3D LightSource::getPosition() {
    return position;
}

Color LightSource::getColor() {
    return color;
}