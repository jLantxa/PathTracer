/*
 * This source file is part of PathTracer
 *
 * Copyright 2019 Javier Lancha Vázquez
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

#include "IRenderer.hpp"
#include "GeometryRenderer.hpp"

#include "Camera.hpp"
#include "Light.hpp"
#include "Objects.hpp"
#include "Utils.hpp"

#include "debug.hpp"

static const char* TAG = "GeometryRenderer";

GeometryRenderer::GeometryRenderer() {

}

GeometryRenderer::~GeometryRenderer() {

}

void GeometryRenderer::render(struct Scene& scene, Camera& camera) {
    Surface& surface = camera.getSurface();
    const unsigned width = surface.getWidth();
    const unsigned height = surface.getHeight();
    Debug::Log::i(TAG, "Render scene: %dx%d", width, height);

    #pragma omp parallel for schedule(static)
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Ray ray = camera.getRayToPixel(i, j);
            surface[i][j] = traceRay(ray, scene);
        }
    }
}

Color GeometryRenderer::traceRay(Ray& ray, struct Scene& scene) {
    Real t;
    IObject3D* iObject = intersectObjects(ray, scene.objects, t);
    if (iObject == nullptr) {
        return Color();
    }
    return iObject->color();
}
