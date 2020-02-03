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

#ifndef _INCLUDE_PATHTRACER_SCENE_PARSER_H_
#define _INCLUDE_PATHTRACER_SCENE_PARSER_H_

#include "Objects.hpp"

enum ParserError : int {
    PARSER_OK = 0,
    PARSER_ERROR_NO_FILE,
    PARSER_ERROR_INVALID_FORMAT,
    /// \todo Remove when finished
    PARSER_ERROR_UNFINISHED,
};

ParserError parseSceneFromXml(const char* filename, struct Scene& scene);

#endif // _INCLUDE_PATHTRACER_SCENE_PARSER_H_
