#pragma once
#include <vector>
#include "vertex.h"
#include "primitive.h"

class RectangleRGB : public Primitive {
public:
	RectangleRGB(vec3& pos, float width, float height, vec3 scale, vec3 color, int indexOffset){
		VertexRGB v;
		v.pos = pos;
		v.rgb = color;
		vertices.push_back(v); // bottom left;
		v.pos.r -= width * scale.r;
		vertices.push_back(v); // bottom right;
		v.pos.g -= height * scale.g;
		vertices.push_back(v); // top right;
		v.pos.r += width * scale.r;
		vertices.push_back(v); // top left;

		indices.push_back(indexOffset);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 3);
		indices.push_back(indexOffset);
	}
	virtual VertexBuffer GetVerticesBuffer() override
	{
		VertexBuffer vb;
		vb.bufferSize = vertices.size() * sizeof(VertexRGB);
		vb.data = (uint8_t*)vertices.data();
		vb.stride = sizeof(VertexRGB);
		return vb;
	}
	virtual IndexBuffer GetIndicesBuffer() override
	{
		IndexBuffer ib;
		ib.bufferSize = indices.size() * sizeof(uint16_t);
		ib.data = (uint8_t*)indices.data();
		ib.typeSize = sizeof(uint16_t);
		return ib;
	}
private:
	std::vector<VertexRGB> vertices;
	std::vector<uint16_t> indices;
};