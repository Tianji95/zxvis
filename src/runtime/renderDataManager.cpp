
#include "renderDataManager.h"

/*
     1
1 -------- -1
    -1
*/
void RenderDataManager::MakeData()
{
    builder = new PrimitiveBuilder();
    builder->SetColor(vec3(1, 0, 0));
    builder->SetScale(vec3(1, 1, 1));
    builder->BuildRectangle(vec3(0.95f, 0.0f, 0.00f), 0.1f, 0.04f);
    builder->SetColor(vec3(0, 1, 0));
    builder->BuildRectangle(vec3(0.95f, 0.0f, 0.00f), 0.02f, 0.04f);

    //for (auto year : years) {
    //    for (auto block : data[year]) {
    //        for (auto subblock : data[year][block]) {
    //            builder->BuildRectangle(vec3(0.96f, 0.0f, 0.05f), 0.1f, 0.1f);

    //        }
    //    }
    //}
  


    vb = builder->GetVertexBuffer();
    ib = builder->GetIndexBuffer();
}


void RenderDataManager::FreeData()
{
    delete builder;
}

VertexBuffer* RenderDataManager::GetVertexBuffer()
{
    return &vb;
}

IndexBuffer* RenderDataManager::GetIndexBuffer()
{
    return &ib;
}