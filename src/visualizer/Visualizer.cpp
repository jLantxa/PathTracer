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

#include <thread>

#include "debug.hpp"

#include "IRenderer.hpp"
#include "PathTracer.hpp"

#include "Camera.hpp"
#include "Surface.hpp"
#include "SceneParser.hpp"

#define DEFAULT_FILENAME "scene.xml"

const char* TAG = "Visualizer";

int main(int argc, char* argv[]) {
    if (argc < 6 || argc > 7) {
        Debug::Log::e(TAG, "Usage: Visualizer [filename] width height fov spp depth");
        return -1;
    }

    const char* filename = (argc == 7)? argv[1] : DEFAULT_FILENAME;
    const int o = (argc == 7)? 1 : 0;
    unsigned int width = atoi(argv[1+o]);
    unsigned int height = atoi(argv[2+o]);
    float fov = atof(argv[3+o]);
    unsigned int spp = atoi(argv[4+o]);
    unsigned int depth = atoi(argv[5+o]);

    if (width <= 0 || height <= 0) {
        Debug::Log::e(TAG, "ERROR: Surface cannot have null size");
        return -1;
    } else if (width*height > 1920 * 1080) {
        Debug::Log::e(TAG, "ERROR: You specified a surface of size %dx%d. Please, use a lower resolution.", width, height);
        return -1;
    } else if ((fov <= 0) | (fov >= 180)) {
        Debug::Log::e(TAG, "ERROR: Suported FOVs range from 0º to 180º");
        return -1;
    } else if (spp <= 0) {
        Debug::Log::e(TAG, "Number or samples per pixer must be positive");
        return -1;
    }

    struct Scene scene;
    ParserError ret = parseSceneFromXml(filename, scene);
    if (ret != PARSER_OK) {
        Debug::Log::e(TAG, "Error %d while parsing scene xml", ret);
        return ret;
    }


    Vec3D camPos(0, 80, -0);
    Vec3D camFacing(0, -0.1, -1);
    Camera camera(width, height, fov, camPos, camFacing);
    camera.setGammaCorrectionEnabled(true);

    PathTracer renderer(spp, depth);
    renderer.renderScene(scene, camera);
    camera.getSurface().toPPM("visualizer.ppm");

    return 0;
}
