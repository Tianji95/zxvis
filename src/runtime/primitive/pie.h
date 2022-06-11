#pragma once
#ifndef _PIE_H_
#define _PIE_H_
#include <vector>
#include "vertex.h"
#include "primitive.h"

class PieRGB : public Primitive {
public:
	PieRGB(vec3& pos, float radius, float startAngle, float endAngle, vec3& color, int indexOffset)
	{
		VertexRGB v;
		int indexCnt = indexOffset;

		v.rgb = color;
		v.pos = pos;
		vertices.push_back(v); // pie center
		float angle = startAngle;
		v.pos.x = pos.x + radius * cos(angle / 180.0f * 3.1415926535);
		v.pos.y = pos.y + radius * sin(angle / 180.0f * 3.1415926535);
		vertices.push_back(v); // start line
		indexCnt += 2;
		do {
			angle += 0.1f;
			v.pos.x = pos.x + radius * cos(angle / 180.0f * 3.1415926535);
			v.pos.y = pos.y + radius * sin(angle / 180.0f * 3.1415926535);
			vertices.push_back(v);
			indices.push_back(indexOffset); // pie center
			indices.push_back(indexCnt - 1); // start opint
			indices.push_back(indexCnt); // end opint
			indexCnt++;

		} while (endAngle - angle > 0.00001f);
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
	virtual size_t GetVertexCount() override
	{
		return vertices.size();
	}

private:
	std::vector<VertexRGB> vertices;
	std::vector<uint16_t> indices;
};

#endif // !_PIE_H_
