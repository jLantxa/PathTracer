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

#include <SDL2/SDL.h>

#include "RayTracer.hpp"
#include "Common.hpp"
#include "Geometry.hpp"
#include "Objects.hpp"
#include "Camera.hpp"

void drawCanvas(Canvas* canvas) {
    unsigned width = canvas->getWidth();
    unsigned height = canvas->getHeight();

    SDL_Window* window = NULL;
    window = SDL_CreateWindow
    (
        "Ray Tracer test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set render color to red (background will be rendered in this color)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int rgb = (*canvas)[i][j];
            int r = (rgb &0x00FF0000) >> 16;
            int g = (rgb &0x0000FF00) >> 8;
            int b = (rgb &0x000000FF);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }


    SDL_RenderPresent(renderer);

    SDL_Delay(3000) ;

    SDL_DestroyWindow(window);
    SDL_Quit();
}

Sphere* createSphere(Color color, Vec3D& center, Real radius) {
    return new Sphere(color, center, radius);
}

Plane* createPlane(Color color, Vec3D& center, Vec3D& normal) {
    return new Plane(color, center, normal);
}

void buildScene(struct Scene* scene) {
    Color color;
    Vec3D v1, v2;
    Real r;

    color.set(1, 0, 0);
    v1.set(-4, 0, -8);
    r = 1.5;
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0, 1, 0);
    v1.set(0, 0, -8);
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0, 0, 1);
    v1.set(4, 0, -8);
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, -0.7, 0);
    v2.set(0, 1, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.75, 0, 0.6);
    v1.set(-10, 0, 0);
    v2.set(1, 0, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.6, 0, 0.75);
    v1.set(10, 0, 0);
    v2.set(-1, 0, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.6, 0.2, 0.75);
    v1.set(0, 0, -20);
    v2.set(0, 0, 1);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.6, 0.2, 0.75);
    v1.set(0, 0, 20);
    v2.set(0, 0, 1);
    scene->objects.push_back(createPlane(color, v1, v2));
}

int main (int argc, char* argv[]) {
    Canvas* canvas;
    struct Scene scene;

    Camera camera(640, 480, 90);

    buildScene(&scene);
    canvas = RayTracer::renderScene(scene, camera);
    drawCanvas(canvas);
    return 0;
}