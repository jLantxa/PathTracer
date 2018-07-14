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

#include <iostream>

#include "RayTracer.hpp"
#include "Common.hpp"
#include "Geometry.hpp"
#include "Objects.hpp"
#include "Camera.hpp"

#include <chrono>
#include <iostream>

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

Triangle* createTriangle(Color color, Vec3D& A, Vec3D& B, Vec3D& C) {
    return new Triangle(color, A, B, C);
}

void buildScene(struct Scene* scene) {
    Color color;
    Vec3D v1, v2, v3;
    Real r;

    color.set(1, 0, 0);
    v1.set(-60, 0, -200);
    r = 25;
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0, 1, 0);
    v1.set(0, 0, -200);
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0, 0, 1);
    v1.set(60, 0, -200);
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, -30, 0);
    v2.set(0, 1, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.75, 0, 0.6);
    v1.set(-100, 0, 0);
    v2.set(1, 0, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.6, 0, 0.75);
    v1.set(100, 0, 0);
    v2.set(-1, 0, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.6, 0.2, 0.75);
    v1.set(0, 0, -300);
    v2.set(0, -0.1, 1);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.6, 0.2, 0.75);
    v1.set(0, 0, 100);
    v2.set(0, -0.1, 1);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(1.0, 0.6, 0.6);
    v1.set(-20, -30, -100);
    v2.set(20, -30, -100);
    v3.set(0, 5, -150);
    scene->objects.push_back(createTriangle(color, v1, v2, v3));
}

int main (int argc, char* argv[]) {
    unsigned width, height;
    float fov;

    if (argc != 4) {
        std::cout << "ERROR: Specify resolution and FOV" << std::endl;
        return -1;
    } else {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        fov = atof(argv[3]);
        if (width <= 0 || height <= 0) {
            std::cout << "ERROR: Canvas cannot have null size." << std::endl;
            return -1;
        } else if (width*height > 1920 * 1080) {
            std::cout << "ERROR: You specified a canvas of size " <<
                width << "x" << height << "." << std::endl <<
                "Please, use a lower resolution." << std::endl;
            return -1;
        } else if (fov <= 0 | fov >= 180) {
            std::cout << "ERROR: Suported FOVs range from 0º to 180º." << std::endl;
            return -1;
        }
    }

    Canvas* canvas;
    struct Scene scene;

    Camera camera(width, height, fov);

    buildScene(&scene);

    std::cout << "Rendering scene..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    canvas = RayTracer::renderScene(scene, camera);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    float seconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000000.0;
    std::cout << "Finished rendering scene." << std::endl;
    std::cout << "Took " << seconds << "s" << std::endl;

    drawCanvas(canvas);
    return 0;
}