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

#include "visualizer/Visualizer.hpp"

#include <SDL2/SDL.h>

#include "debug.hpp"

#include "IRenderer.hpp"
#include "PathTracer.hpp"

#include "Camera.hpp"
#include "Surface.hpp"
#include "SceneParser.hpp"

#define DEFAULT_FILENAME "scene.xml"

const char* TAG = "VisualizerWindow";

VisualizerWindow::VisualizerWindow(Camera& camera)
:   mCamera(camera),
    mWidth(camera.getWidth()),
    mHeight(camera.getHeight())
{
    Debug::Log::d(TAG, "VisualizerWindow()");
    init();
    update();
}

VisualizerWindow::~VisualizerWindow() {
    Debug::Log::d(TAG, "~VisualizerWindow()");
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void VisualizerWindow::init() {
    Debug::Log::d(TAG, "%s()", __func__);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Debug::Log::e(TAG, "%s:%d] SDL could not initialize! SDL Error: %s\n",
            __func__, __LINE__, SDL_GetError());
		return;
	}
	else
	{
        Uint32 windowFlag = SDL_WINDOW_OPENGL;

        mWindow = SDL_CreateWindow (
            mTitle,                     // title
            SDL_WINDOWPOS_UNDEFINED,    // initial x
            SDL_WINDOWPOS_UNDEFINED,    // initial y
            mWidth,                     // width
            mHeight,                    // height
            windowFlag
        );

        if (mWindow == NULL) {
            return;
        }

        SDL_SetWindowResizable(mWindow, SDL_FALSE);

		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			Debug::Log::e(TAG, "%s:%d] Warning: Linear texture filtering not enabled!",
                __func__, __LINE__);
		}

		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
		if (mRenderer == NULL) {
			Debug::Log::e(TAG, "%s:%d] Renderer could not be created! SDL Error: %s\n",
                __func__, __LINE__, SDL_GetError());
			return;
		}
	}
}

void VisualizerWindow::update() {
    const unsigned int camWidth = mCamera.getWidth();
    const unsigned int camHeight = mCamera.getHeight();
    if (mWidth != camWidth || mHeight != camHeight) {
        mWidth = camWidth;
        mHeight = camHeight;
        SDL_SetWindowSize(mWindow, mWidth, mHeight);
    }

    SDL_RenderClear(mRenderer);
    render();
}

void VisualizerWindow::render() {
    Surface& surface = mCamera.getSurface();
    for (int i = 0; i < mWidth; i++) {
        for (int j = 0; j < mHeight; j++) {
            Color c = surface[i][j];
            SDL_SetRenderDrawColor(mRenderer,
                toColorInt(c.x),
                toColorInt(c.y),
                toColorInt(c.z),
                0xFF);
            SDL_RenderDrawPoint(mRenderer, i, j);
        }
    }
    SDL_RenderPresent(mRenderer);
}

void waitForQuit() {
    bool quit = false;
    SDL_Event event;
    do {
        SDL_WaitEvent(&event);
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym != SDLK_ESCAPE)
                break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    } while (!quit);
}

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
    } else if (fov <= 0 | fov >= 180) {
        Debug::Log::e(TAG, "ERROR: Suported FOVs range from 0º to 180º");
        return -1;
    } else if (spp <= 0) {
        Debug::Log::e(TAG, "Number or samples per pixer must be positive");
        return -1;
    }

    struct Scene scene;
    ParserError ret = parseSceneFromXml(filename, scene);

    Vec3D camPos(0, 80, -0);
    Vec3D camFacing(0, -0.1, -1);
    Camera camera(width, height, fov, camPos, camFacing);
    camera.setGammaCorrectionEnabled(true);

    // This goes in a thread
    IRenderer* renderer = new PathTracer(spp, depth);
    renderer->renderScene(scene, camera);

    VisualizerWindow window(camera);
    waitForQuit();
    camera.getSurface().toPPM("visualizer.ppm");

    delete renderer;
    return 0;
}
