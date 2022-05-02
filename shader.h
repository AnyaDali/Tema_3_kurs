#ifndef SHADER_H
#define SHADER_H

#include "file_include.h"

std::string vertexShader =
    "#version 330 core\n"
    "layout (location = 0) in vec2 position;\n"
    "void main() {\n"
    "gl_Position = vec4(position.x, position.y, 0.0, 1.0);\n"
    "}\n";

std::string fragmentShader =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "color = vec4(1.0f, 0.7f, 0.5f, 1.0f);\n"
    "}\n";

#endif