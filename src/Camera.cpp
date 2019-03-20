/*
 * This source file is part of PathTracer
 *
 * Copyright 2018, 2019 Javier Lancha Vázquez
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

#include "debug.hpp"
#include "Light.hpp"
#include "Vector3D.hpp"

#include <cmath>

static const char* TAG = "Camera";

static const Real CAMERA_GAMMA = 1/2.2;

Camera::Camera(unsigned width, unsigned height, float fov) :
        width(width),
        height(height),
        aspectRatio(1.0 * width / height),
        fov(degToRad(fov)),
        surface(width, height),
        gammaCorrectionEnabled(false)
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
        w(facing.normalize()),
        surface(width, height),
        gammaCorrectionEnabled(false)
{
    v = Vec3D(0, 1, 0); // Up (to the sky)
    u = v.cross(w);     // Right
    v = w.cross(u);     // Recalculate Up to be orthogonal to the facing direction
}

Camera::~Camera() {
    Debug::Log::v(TAG, "Delete camera");
}

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

void Camera::setResolution(unsigned w, unsigned h) {
    surface = Surface(w, h);
}

void Camera::setGammaCorrectionEnabled(bool enabled) {
    gammaCorrectionEnabled = enabled;
}

void Camera::onRenderFinished() {
    Debug::Log::i(TAG, "onRenderFinished()");
    if (gammaCorrectionEnabled) {
        Debug::Log::i(TAG, "Apply gamma correction g=%.2f", CAMERA_GAMMA);
        surface.applyGammaCorrection(CAMERA_GAMMA);
    }
}

Surface& Camera::getSurface() {
    return surface;
}

Vec3D Camera::getVectorToPixel(unsigned i, unsigned j) {
    Real right = (1 - 2 * (i + 0.5) / width) * tan(fov/2);
    Real up = (1 - 2*(j + 0.5) / height) * tan(fov/2)/aspectRatio;
    Vec3D vector = w + right*u + up*v;
    return (vector).normalize();
}

Ray Camera::getRayToPixel(unsigned i, unsigned j) {
    Vec3D dir = getVectorToPixel(i, j);
    Ray ray(position, dir);
    return ray;
}
