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

#include <iostream>

#include "PathTracer.hpp"
#include "Common.hpp"
#include "Geometry.hpp"
#include "Objects.hpp"
#include "Camera.hpp"

#include <chrono>
#include <iostream>

Sphere* createSphere(Color color, Vec3D& center, Real radius) {
    return new Sphere(color, center, radius);
}

Plane* createPlane(Color color, Vec3D& center, Vec3D& normal) {
    return new Plane(color, center, normal);
}

Triangle* createTriangle(Color color, Vec3D& A, Vec3D& B, Vec3D& C) {
    return new Triangle(color, A, B, C);
}

LightSource* createLight(Vec3D& pos, Color color) {
    return new LightSource(pos, color);
}



void buildScene(struct Scene* scene) {
    Color color;
    Vec3D v1, v2, v3;
    Real r;

    r = 30;
    color.set(1, 0, 0);
    v1.set(-3*r, 0, -200);
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0, 1, 0);
    v1.set(0, 0, -200);
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0, 0, 1);
    v1.set(3*r, 0, -200);
    scene->objects.push_back(createSphere(color, v1, r));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, -r, 0);
    v2.set(0, 1, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.75, 0, 0.6);
    v1.set(-4*r, 0, 0);
    v2.set(1, 0, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.75, 0, 0.6);
    v1.set(4*r, 0, 0);
    v2.set(-1, 0, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.75, 0.5, 0.6);
    v1.set(0, 0, -400);
    v2.set(0, 0, 1);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(1.0, 0.6, 0.6);
    v1.set(-20, -30, -100);
    v2.set(20, -30, -100);
    v3.set(0, 5, -90);
    scene->objects.push_back(createTriangle(color, v1, v2, v3));

    r = 5;
    color.set(0, 0.6, 1);
    v1.set(-25, -30+r, -75);
    scene->objects.push_back(createSphere(color, v1, r));

    r = 10;
    color.set(0, 1, 1);
    v1.set(40, -30+r, -90);
    scene->objects.push_back(createSphere(color, v1, r));

    v1.set(50, 2*r, -200);
    color.set(1.0, 1.0, 1.0);
    scene->lights.push_back(createLight(v1, color));

    v1.set(50, 2*r, -100);
    color.set(1.0, 1.0, 1.0);
    scene->lights.push_back(createLight(v1, color));
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

    PathTracer tracer(10);
    Vec3D camPos(0, 100, -300);
    Vec3D camFacing(0, -1, 1);
    Camera camera(width, height, fov, camPos, camFacing);
    buildScene(&scene);

    std::cout << "Rendering scene..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    canvas = tracer.renderScene(scene, camera);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    float seconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0;
    std::cout << "Finished rendering scene." << std::endl;
    std::cout << "Took " << seconds << "s" << std::endl;

    canvas->toPPM("pathTracerTest.ppm");
    return 0;
}