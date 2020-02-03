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

#ifndef _INCLUDE_PATHTRACER_VISUALIZER_H_
#define _INCLUDE_PATHTRACER_VISUALIZER_H_

#include <SDL2/SDL.h>

#include "Camera.hpp"
#include "Surface.hpp"

class VisualizerWindow {
public:
    VisualizerWindow(Camera& camera);
    virtual ~VisualizerWindow();

    void update();

private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    Camera& mCamera;

    const char* mTitle = "PathTracer";
    unsigned int mWidth;
    unsigned int mHeight;

    void init();
    void render();
};

void waitForQuit();

#endif // _INCLUDE_PATHTRACER_VISUALIZER_H_
