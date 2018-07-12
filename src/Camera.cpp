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

 #include "Common.hpp"
 #include "Camera.hpp"

 #include "Geometry.hpp"
 #include "Light.hpp"

 #include <cmath>

Camera::Camera(unsigned width, unsigned height, float fov) :
        width(width),
        height(height),
        aspectRatio(1.0 * width / height),
        fov(degToRad(fov))
{
    position = Vec3D();
    u = Vec3D(1, 0, 0);     // Right
    v = Vec3D(0, 1, 0);     // Up
    w = Vec3D(0, 0, -1);    // Facing
}

Camera::Camera(unsigned width, unsigned height, float fov, Vec3D pos, Vec3D facing) :
        width(width),
        height(height),
        aspectRatio(1.0 * width / height),
        fov(degToRad(fov)),
        position(pos),
        w(facing.normalize())
{
    v = Vec3D(0, 1, 0); // Up (to the sky)
    u = v.cross(w);     // Right
    v = w.cross(u);     // Recalculate Up to be orthogonal to the facing direction
}

Camera::~Camera() { }

float Camera::getWidth() {
    return width;
}

float Camera::getHeight() {
    return height;
}

float Camera::getFov() {
    return radToDeg(fov);
}

float Camera::getFovInRad() {
    return fov;
}

float Camera::getAspectRatio() {
    return aspectRatio;
}


Vec3D Camera::getVectorToPixel(unsigned i, unsigned j) {
    Real x = (2 * ((i + 0.5) / width) - 1) * tan(fov/2);
    Real y = (1 - 2*(j + 0.5) / height) * tan(fov/2)/aspectRatio;
    Vec3D vector(x, y, -1);
    return (vector - position).normalize();
}

Ray Camera::getRayToPixel(unsigned i, unsigned j) {
    Vec3D dir = getVectorToPixel(i, j);
    Ray ray(position, dir);
    return ray;
}