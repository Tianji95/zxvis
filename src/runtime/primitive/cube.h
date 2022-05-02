#pragma once
#ifndef _CUBE_H_
#define _CUBE_H_
#include <vector>
#include "vertex.h"
#include "primitive.h"

class CubeRGB : public Primitive {
public:
	CubeRGB(vec3& pos, float width, float height, float length, vec3 scale, vec3 color, int indexOffset)
	{
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

		v.pos = pos;
		v.pos.b += length;
		vertices.push_back(v); // bottom left;
		v.pos.r -= width * scale.r;
		vertices.push_back(v); // bottom right;
		v.pos.g -= height * scale.g;
		vertices.push_back(v); // top right;
		v.pos.r += width * scale.r;
		vertices.push_back(v); // top left;

		// font face
		indices.push_back(indexOffset);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 3);

		//top face
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 6);
		indices.push_back(indexOffset + 7);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 7);
		indices.push_back(indexOffset + 3);

		//left face
		indices.push_back(indexOffset + 4);
		indices.push_back(indexOffset + 0);
		indices.push_back(indexOffset + 3);
		indices.push_back(indexOffset + 4);
		indices.push_back(indexOffset + 3);
		indices.push_back(indexOffset + 7);

		//right face
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 5);
		indices.push_back(indexOffset + 6);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 6);
		indices.push_back(indexOffset + 2);

		//back face
		indices.push_back(indexOffset + 6);
		indices.push_back(indexOffset + 5);
		indices.push_back(indexOffset + 4);
		indices.push_back(indexOffset + 7);
		indices.push_back(indexOffset + 6);
		indices.push_back(indexOffset + 4);

		//bottom face
		indices.push_back(indexOffset + 4);
		indices.push_back(indexOffset + 5);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 4);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 0);
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

#endif // !_CUBE_H_
