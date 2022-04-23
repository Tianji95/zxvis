#pragma once
#include <vector>
#include "primitive.h"

struct VertexBuffer {
    unsigned int bufferSize;
    unsigned int stride;
    const void* data;
};

struct IndexBuffer {
    unsigned int bufferSize;
    unsigned int typeSize;
    const void* data;
};

class RenderDataManager {
public:
    static RenderDataManager& Instance()
    {
        static RenderDataManager renderDataMgr;
        return renderDataMgr;
    }

    void MakeData();
    VertexBuffer* GetVertexBuffer();
    IndexBuffer* GetIndexBuffer();
private:
    RenderDataManager()
    {
        MakeData();
    }
    ~RenderDataManager() = default;
    RenderDataManager(const RenderDataManager&);
    RenderDataManager& operator=(const RenderDataManager&);
private:
    VertexBuffer vb;
    IndexBuffer ib;
};