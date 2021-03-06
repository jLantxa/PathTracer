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

#include <cstdint>

#include "Common.hpp"
#include "Light.hpp"
#include "Vector3D.hpp"


Ray::Ray(Vec3D origin_m, Vec3D dir_v) :
    mOrigin_v(origin_m),
    mDirection_v(dir_v.normalize()) { }

Ray::~Ray() {}

Vec3D Ray::getOrigin() {
    return mOrigin_v;
}

Vec3D Ray::getDirection() {
    return mDirection_v;
}

Vec3D Ray::point(Real t) {
    return mOrigin_v + t*mDirection_v;
}
