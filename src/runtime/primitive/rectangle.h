#pragma once
#include <vector>
#include "vertex.h"
#include "primitive.h"

class RectangleRGB : public PrimitiveRGB {
public:
	virtual void* GetVertices() override;
	virtual void* GetIndices() override;
private:
	std::vector<VertexRGB> vertices;
	std::vector<unsigned int> indices;
};