#pragma once
#include <vector>
#include "primitiveBuilder.h"

class RenderDataManager {
public:
    static RenderDataManager& Instance()
    {
        static RenderDataManager renderDataMgr;
        return renderDataMgr;
    }

    VertexBuffer* GetVertexBuffer();
    IndexBuffer* GetIndexBuffer();
private:

    RenderDataManager()
    {
        MakeData();
    }
    ~RenderDataManager()
    {
        FreeData();
    }
    void MakeData();
    void FreeData();
    RenderDataManager(const RenderDataManager&);
    RenderDataManager& operator=(const RenderDataManager&);
private:
    PrimitiveBuilder* builder = nullptr;
    VertexBuffer vb;
    IndexBuffer ib;
};