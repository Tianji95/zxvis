
#include "renderDataManager.h"
#include "hnmfExcelLoader.h"
/*
     1
1 -------- -1
    -1
*/
void RenderDataManager::MakeData()
{
    builder = new PrimitiveBuilder();
    //builder->SetColor(vec3(1, 0, 0));
    builder->SetScale(vec3(1, 1, 1));
    //builder->BuildRectangle(vec3(0.95f, 0.0f, 0.00f), 0.1f, 0.04f);
    //builder->SetColor(vec3(0, 1, 0));
    //builder->BuildRectangle(vec3(0.95f, 0.0f, 0.00f), 0.02f, 0.04f);

    HNMFLoader* loader = new HNMFExcelLoader();
    HnmfData data = loader->LoadData(std::string("F:/GitHub/zxvis/resources/test_data/hnmftest.xlsx"));

    float widthGap = 1.0f / data.size();
    float blockWidth = 0.8f / data.size();
    float blockHeight = 0.05f;
    float heightGap = 0.1f;
    float startx = 0.90f;
    float starty = 0.0f;

    for (auto& yearIter : data) {
        float blockx = startx;
        unsigned int blockCnt = yearIter.second.size();
        float blocky = starty;
        if (blockCnt % 2 == 0) {
            blocky -= (heightGap + blockHeight) / 2;
        }
        blocky += (blockCnt / 2) * (heightGap + blockHeight);
        for (auto& block : yearIter.second) {
            float subblockx = blockx;
            float subblocky = blocky;
            for (auto subblock : block.data) {
                float subblockWidth = subblock.percentage * blockWidth;
                builder->SetColor(vec3(subblock.color.x, subblock.color.y, subblock.color.z));
                builder->BuildCube(vec3(subblockx, subblocky, 0.0f), subblockWidth, blockHeight, 0.15f);
                subblockx -= subblockWidth;
            }
            blocky -= blockHeight + heightGap;
        }
        startx -= widthGap + blockWidth;
    }
  


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