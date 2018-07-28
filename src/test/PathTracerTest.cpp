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

#include "PathTracer.hpp"
#include "Common.hpp"
#include "Utils.hpp"
#include "Vector3D.hpp"
#include "Objects.hpp"
#include "Camera.hpp"

#include <chrono>
#include <iostream>
#include <cstdio>

Sphere* createSphere(Vec3D color, Vec3D& center, Real radius) {
    return new Sphere(color, center, radius);
}

Plane* createPlane(Vec3D color, Vec3D& center, Vec3D& normal) {
    return new Plane(color, center, normal);
}

Triangle* createTriangle(Vec3D color, Vec3D& A, Vec3D& B, Vec3D& C) {
    return new Triangle(color, A, B, C);
}

LightSource* createLight(Vec3D& pos, Vec3D color) {
    return new LightSource(pos, color);
}

void savePartialResultCallback(Canvas* canvas) {
    unsigned spp = canvas->spp;
    // If spp power of 2
    if (spp && !(spp & (spp - 1))) {
        char filename[100];
        sprintf(filename, "pic/conv_test_partial_%d.ppm", spp);
        canvas->toPPM(filename);
    }
}


void buildScene(struct Scene* scene) {
    Vec3D color;
    Vec3D v1, v2, v3;
    Real sRad = 30;

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, 0);
    v2.set(0, 1, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    Real ceiling = 6*sRad;
    color.set(0.75, 0.75, 0.75);
    v1.set(0, ceiling, 0);
    v2.set(0, -1, 0);
    Plane* lightCeil = createPlane(color, v1, v2);
    //lightCeil->material.emission = 0.05;
    scene->objects.push_back(lightCeil);

    color.set(0.75, 0.75, 0.2);
    v1.set(-4*sRad, 0, 0);
    v2.set(1, 0, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.75, 0.75, 0.2);
    v1.set(4*sRad, 0, 0);
    v2.set(-1, 0, 0);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, -200);
    v2.set(0, 0, 1);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, 0);
    v2.set(0, 0, -1);
    scene->objects.push_back(createPlane(color, v1, v2));

    color.set(1, 0, 0);
    v1.set(-2.5f*sRad, sRad, -200+sRad);
    scene->objects.push_back(createSphere(color, v1, sRad));

    color.set(0, 1, 0);
    v1.set(0, 2*sRad, -200 + sRad+sRad);
    Sphere* greenSphere = createSphere(color, v1, sRad);
    scene->objects.push_back(greenSphere);

    color.set(0, 0, 1);
    v1.set(2.5f*sRad, 2*sRad, -200+sRad);
    scene->objects.push_back(createSphere(color, v1, sRad));

    Real lRad = 10;
    color.set(1.0, 1.0, 1.0);
    v1.set(-50, ceiling-lRad, -100);
    Sphere* lightBall1 = createSphere(color, v1, lRad);
    lightBall1->material.emission = 20*64.0;
    scene->objects.push_back(lightBall1);

    v1.set(50, ceiling-lRad, -100);
    Sphere* lightBall2 = createSphere(color, v1, lRad);
    lightBall2->material.emission = 20*64.0;
    scene->objects.push_back(lightBall2);
}

void testSample(uint16_t* Xi) {
    Vec3D normal(0, 1, 0);
    Vec3D sample = sampleHemisphere(normal, Xi);
}

int main (int argc, char* argv[]) {
    unsigned width, height, spp, depth, aa;
    float fov;

    if (argc != 6) {
        std::cout << "ERROR: Specify resolution, FOV, SPP, depth" << std::endl;
        return -1;
    } else {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        fov = atof(argv[3]);
        spp = atoi(argv[4]);
        depth = atoi(argv[5]);
        //aa = atoi(argv[;6]);
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
        } else if(spp <= 0) {
            std::cout << "Number or samples per pixer must be positive" << std::endl;
            return -1;
        }/* else if(aa < 1) {
            std::cout << "Antialias factor must be at least 1" << std::endl;
            return -1;
        }*/
    }

    Canvas* canvas;
    struct Scene scene;

    PathTracer tracer(depth);
    tracer.setCallback(savePartialResultCallback);
    Vec3D camPos(0, 80, -0);
    Vec3D camFacing(0, -0.1, -1);
    Camera camera(width, height, fov, camPos, camFacing);
    buildScene(&scene);

    canvas = tracer.renderScene(spp, scene, camera);
    char filename[100];
    sprintf(filename, "pic/conv_test_final_%d.ppm", spp);
    canvas->toPPM(filename);
    return 0;
}
