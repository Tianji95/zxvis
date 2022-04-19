#pragma once
#ifndef VERTICS_DATA_H
#define VERTICS_DATA_H

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
};


//const std::vector<Vertex> vertices = {
//    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},//1
//    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},//5
//    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}//6
//
//};
//const std::vector<uint16_t> indices = {
//    0,1,2      //right
//};


//注意，这个地方图省事，只弄了8个顶点，所以UV显示是不对的，正常来说应该要有12个顶点才行（xyz相同uv不同）
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},//0
    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},//1
    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},//2
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},//3

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},//4
    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},//5
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},//6
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},//7

};
const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0, // 正面
    1,5,6,6,2,1,       //right
    2,6,7,7,3,2,      //top
    3,7,4,4,0,3,      //left
    4,5,1,1,0,4,     //bottom
    5,4,7,7,6,5     //back
};



struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

#endif // !VERTICS_DATA_H
