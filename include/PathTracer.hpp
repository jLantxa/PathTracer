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

#ifndef _INCLUDE_PATHTRACER_PATHTRACER_H_
#define _INCLUDE_PATHTRACER_PATHTRACER_H_

#include "IRenderer.hpp"

#include "Common.hpp"
#include "Surface.hpp"
#include "Objects.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Surface.hpp"

#include <cstdint>
#include <vector>

/**
 * IResultsListener is an interface to be notified of rendering results,
 * either partial or complete. Thus, listeners can act upon notification of
 * a rendering result.
*/
class IResultsListener {
public:
    virtual ~IResultsListener() = default;

    /** The renderer has finished rendering a part of the scene*/
    virtual void onPartialResult(struct Scene& scene, Camera& camera) = 0;

    /** The renderer has finished rendering the scene */
    virtual void onRenderFinished(struct Scene& scene, Camera& camera) = 0;
};

class PathTracer : public IRenderer {
public:
    PathTracer(unsigned spp, unsigned depth);
    virtual ~PathTracer();

    /** Register a results listener */
    void addCallback(IResultsListener* callback);

    /** Set the block size of the renderer */
    void setBlockSize(unsigned int width, unsigned int height);

private:
    unsigned mMaxDepth;
    unsigned mSPP;
    std::vector<IResultsListener*> mPartialResultListeners;

    virtual void render(struct Scene& scene, Camera& camera);
    Color traceRay(unsigned depth, Ray& ray, struct Scene& scene);

    void notifyPartialResult(struct Scene& scene, Camera& camera);
    void notifyRenderFinished(struct Scene& scene, Camera& camera);

    static constexpr unsigned int DEFAULT_BLOCK_WIDTH = 64;
    static constexpr unsigned int DEFAULT_BLOCK_HEIGHT = 64;
    unsigned int mBlockWidth = DEFAULT_BLOCK_WIDTH;
    unsigned int mBlockHeight = DEFAULT_BLOCK_HEIGHT;

    struct Block {
        unsigned int left, up, right, down;
    };

    void calculateBlocks(std::vector<Block>& blocks, unsigned int width, unsigned int height);
    void reorderBlocks(std::vector<Block>& blocks, unsigned int width, unsigned int height);
};

#endif // _INCLUDE_PATHTRACER_PATHTRACER_H_
