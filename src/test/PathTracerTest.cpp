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

#include "PathTracer.hpp"
#include "Common.hpp"
#include "Utils.hpp"
#include "Vector3D.hpp"
#include "Objects.hpp"
#include "Camera.hpp"

#include <chrono>
#include <cstdio>

#include "debug.hpp"

static const char* TAG = "PathTracerTest";

bool isPowerOfTwo(unsigned x) {
    return (x && !(x & (x - 1)));
}

Sphere* createSphere(struct Material material, Vec3D& center, Real radius) {
    return new Sphere(material, center, radius);
}

Plane* createPlane(struct Material material, Vec3D& center, Vec3D& normal) {
    return new Plane(material, center, normal);
}

Triangle* createTriangle(struct Material material, Vec3D& A, Vec3D& B, Vec3D& C) {
    return new Triangle(material, A, B, C);
}


void buildScene(struct Scene* scene) {
    Color color;
    Vec3D v1, v2, v3;
    Real sRad = 30;

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, 0);
    v2.set(0, 1, 0);
    scene->objects.push_back(createPlane({color}, v1, v2));

    Real ceiling = 6*sRad;
    color.set(0.75, 0.75, 0.75);
    v1.set(0, ceiling, 0);
    v2.set(0, -1, 0);
    Plane* lightCeil = createPlane({color}, v1, v2);
    //lightCeil->material.emission = 0.05;
    scene->objects.push_back(lightCeil);

    color.set(0.75, 0.2, 0.2);
    v1.set(-4*sRad, 0, 0);
    v2.set(1, 0, 0);
    scene->objects.push_back(createPlane({color}, v1, v2));

    color.set(0.75, 0.2, 0.2);
    v1.set(4*sRad, 0, 0);
    v2.set(-1, 0, 0);
    scene->objects.push_back(createPlane({color}, v1, v2));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, -200);
    v2.set(0, 0, 1);
    scene->objects.push_back(createPlane({color}, v1, v2));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, 0);
    v2.set(0, 0, -1);
    scene->objects.push_back(createPlane({color}, v1, v2));

    color.set(0.707, 0, 0.707);
    v1.set(-2.5f*sRad, sRad, -200+sRad);
    scene->objects.push_back(createSphere({color}, v1, sRad));

    color.set(0.707, 0.707, 0);
    v1.set(0, 2*sRad, -200 + sRad+sRad);
    Sphere* greenSphere = createSphere({color}, v1, sRad);
    scene->objects.push_back(greenSphere);

    color.set(0, 0.707, 0.707);
    v1.set(2.5f*sRad, 2*sRad, -190+sRad);
    scene->objects.push_back(createSphere({color}, v1, sRad));

    Real lRad = 10;
    color.set(1.0, 1.0, 1.0);
    v1.set(-50, ceiling-lRad, -100);
    Sphere* lightBall1 = createSphere({color}, v1, lRad);
    lightBall1->material().emission = 20*64.0;
    scene->objects.push_back(lightBall1);

    v1.set(50, ceiling-lRad, -100);
    Sphere* lightBall2 = createSphere({color}, v1, lRad);
    lightBall2->material().emission = 20*64.0;
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
        Debug::Log::e(TAG, "ERROR: Specify resolution, FOV, SPP, depth");
        return -1;
    } else {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        fov = atof(argv[3]);
        spp = atoi(argv[4]);
        depth = atoi(argv[5]);
        if (width <= 0 || height <= 0) {
            Debug::Log::e(TAG, "ERROR: Surface cannot have null size");
            return -1;
        } else if (width*height > 1920 * 1080) {
            Debug::Log::e(TAG, "ERROR: You specified a surface of size %dx%d. Please, use a lower resolution.", width, height);
            return -1;
        } else if (fov <= 0 | fov >= 180) {
            Debug::Log::e(TAG, "ERROR: Suported FOVs range from 0º to 180º");
            return -1;
        } else if (spp <= 0) {
            Debug::Log::e(TAG, "Number or samples per pixer must be positive");
            return -1;
        }
    }

    struct Scene scene;

    IRenderer* renderer = new PathTracer(spp, depth);
    Vec3D camPos(0, 80, -0);
    Vec3D camFacing(0, -0.1, -1);
    Camera camera(width, height, fov, camPos, camFacing);
    camera.setGammaCorrectionEnabled(true);
    Surface& surface = camera.getSurface();
    buildScene(&scene);

    renderer->renderScene(scene, camera);
    char filename[100];
    sprintf(filename, "pic/conv_test_final_%d.ppm", spp);
    surface.toPPM(filename);
    return 0;
}
