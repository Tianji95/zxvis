#include "renderDataManager.h"
#include "vertex.h"

void RenderDataManager::MakeData()
{
    static const std::vector<VertexRGB> vertices = {
        {{-0.05f, -0.05f, 0.05f}, {1.0f, 0.0f, 0.0f}},// 0
        {{0.05f, -0.05f, 0.05f}, {1.0f, 0.0f, 0.0f}}, // 1
        {{0.05f, 0.05f, 0.05f}, {1.0f, 0.0f, 0.0f}},  // 2
        {{-0.05f, 0.05f, 0.05f}, {1.0f, 0.0f, 0.0f}}, // 3

        {{-0.05f, -0.05f, -0.05f}, {1.0f, 0.0f, 0.0f}},// 4
        {{0.05f, -0.05f, -0.05f}, {1.0f, 0.0f, 0.0f}}, // 5
        {{0.05f, 0.05f, -0.05f}, {1.0f, 0.0f, 0.0f}},  // 6
        {{-0.05f, 0.05f, -0.05f}, {1.0f, 0.0f, 0.0f}}, // 7
    };
    static const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0, // ÕýÃæ
         1,5,6,6,2,1,       //right
         2,6,7,7,3,2,      //top
         3,7,4,4,0,3,      //left
         4,5,1,1,0,4,     //bottom
         5,4,7,7,6,5     //back
    };
    vb.bufferSize = sizeof(VertexRGB) * vertices.size();
    vb.data = vertices.data();
    vb.stride = sizeof(VertexRGB);
    ib.bufferSize = sizeof(uint16_t) * indices.size();
    ib.data = indices.data();
    ib.typeSize = sizeof(uint16_t);
}

VertexBuffer* RenderDataManager::GetVertexBuffer()
{
    return &vb;
}

IndexBuffer* RenderDataManager::GetIndexBuffer()
{
    return &ib;
}