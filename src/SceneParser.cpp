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

#include "SceneParser.hpp"
#include "debug.hpp"

#include "Objects.hpp"

#ifndef SCENE_PARSER_TEMPORARY
static const char* LOG_TAG = "SceneParser";

#include <cstring>

#include <libxml/parser.h>
#include <libxml/tree.h>

#define ROOT_NODE "scene"
#define ATTR_BACKGROUND "background"
#define NODE_SPHERE "sphere"
#define NODE_TRIANGLE "triangle"
#define NODE_PLANE "plane"
#endif

#ifdef SCENE_PARSER_TEMPORARY
Sphere* createSphere(struct Material material, Vec3D& center, Real radius) {
    return new Sphere(material, center, radius);
}

Plane* createPlane(struct Material material, Vec3D& center, Vec3D& normal) {
    return new Plane(material, center, normal);
}

Triangle* createTriangle(struct Material material, Vec3D& A, Vec3D& B, Vec3D& C) {
    return new Triangle(material, A, B, C);
}


void buildScene(struct Scene& scene) {
    Color color;
    Vec3D v1, v2, v3;
    Real sRad = 30;

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, 0);
    v2.set(0, 1, 0);
    scene.objects.push_back(createPlane({color}, v1, v2));

    Real ceiling = 8*sRad;
    v1.set(0, ceiling, 0);
    v2.set(0, -1, 0);
    scene.objects.push_back(createPlane({color, 10.0f*Color(1.0, 1.0, 1.0)}, v1, v2));

    color.set(0.75, 0.2, 0.2);
    v1.set(-5*sRad, 0, 0);
    v2.set(1, 0, 0);
    scene.objects.push_back(createPlane({color}, v1, v2));

    color.set(0.75, 0.2, 0.2);
    v1.set(5*sRad, 0, 0);
    v2.set(-1, 0, 0);
    scene.objects.push_back(createPlane({color}, v1, v2));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, -200);
    v2.set(0, 0, 1);
    scene.objects.push_back(createPlane({color}, v1, v2));

    color.set(0.75, 0.75, 0.75);
    v1.set(0, 0, 0);
    v2.set(0, 0, -1);
    scene.objects.push_back(createPlane({color}, v1, v2));

    color.set(0.707, 0, 0.707);
    v1.set(-2.5f*sRad, sRad, -200+sRad);
    scene.objects.push_back(createSphere({color}, v1, sRad));

    color.set(0.707, 0.707, 0);
    v1.set(0, 2*sRad, -200 + sRad+sRad);
    Sphere* greenSphere = createSphere({color}, v1, sRad);
    scene.objects.push_back(greenSphere);

    color.set(0, 0.707, 0.707);
    v1.set(2.5f*sRad, 2*sRad, -190+sRad);
    scene.objects.push_back(createSphere({color}, v1, sRad));
}
#endif

#ifndef SCENE_PARSER_TEMPORARY
/// \todo
IObject3D* parseObject(xmlNode* node) {
    /// \todo Return object
    return nullptr;

    const char* name = (char*)(node->name);
    if (!strcmp(name, NODE_SPHERE)) {
    } else if (!strcmp(name, NODE_TRIANGLE)) {
    } else if (!strcmp(name, NODE_PLANE)) {
    } else {
        return nullptr;
    }
}

Color parseColor(const char* hexstr) {
    const unsigned long value = std::strtoul(hexstr, 0, 16);
    const Real r = (value & 0xFF0000)/255.0f;
    const Real g = (value & 0x00FF00)/255.0f;
    const Real b = (value & 0x0000FF)/255.0f;
    return Color(r, g, b);
}

ParserError parse(const char* filename, struct Scene& scene) {
    /// \todo Remove PARSER_ERROR_UNFINISHED when finished
    return PARSER_ERROR_UNFINISHED;

    xmlDoc *doc = NULL;
    xmlNode *root = NULL;

    doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        Debug::Log::e(LOG_TAG, "Could not parse file %s", filename);
        return PARSER_ERROR_NO_FILE;
    }

    root = xmlDocGetRootElement(doc);
    if (strcmp((char*)root->name, ROOT_NODE) != 0) {
        return PARSER_ERROR_INVALID_FORMAT;
    }

    /* Attributes of the scene */
    xmlAttr* attribute = root->properties;
    while(attribute && attribute->name && attribute->children) {
        xmlChar* value = xmlNodeListGetString(root->doc, attribute->children, 1);

        // BACKGROUND
        if (!strcmp((char*)attribute->name, ATTR_BACKGROUND)) {
            Color background = parseColor((char*)value);
            Debug::Log::i(LOG_TAG, "Found background %d", (char*)value);
            /// \todo Parse colour and add it to the scene
        }

        xmlFree(value);
        attribute = attribute->next;
    }

    /* Scene contents */
    for (xmlNode* cur_node = root->children; cur_node; cur_node = cur_node->next) {
        IObject3D* object = parseObject(cur_node);
        if (object != nullptr) {
            scene.objects.push_back(object);
        }
    }

    return PARSER_OK;
}
#endif

/// \todo Empty scene
ParserError parseSceneFromXml(const char* filename, struct Scene& scene) {
#ifdef SCENE_PARSER_TEMPORARY
    (void) filename;
    buildScene(scene);
    return PARSER_OK;
#else
    return parse(filename, scene);
#endif
}
