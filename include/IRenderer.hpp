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

#ifndef _INCLUDE_PATHTRACER_RENDERER_H_
#define _INCLUDE_PATHTRACER_RENDERER_H_

#include "Common.hpp"

#include "Camera.hpp"
#include "Objects.hpp"
#include "Surface.hpp"

#include <vector>

/** A container of objects and light sources */
// TODO: Delete objects
struct Scene {
    std::vector<IObject3D*> objects;
    Color backgroundColor;
};

class IRenderer {
public:
    virtual ~IRenderer() = default;

    void renderScene(struct Scene& scene, Camera& camera) {
        render(scene, camera);
        camera.onRenderFinished();
    }

protected:
    virtual void render(struct Scene& scene, Camera& camera) = 0;
};

#endif // _INCLUDE_PATHTRACER_RENDERER_H_
