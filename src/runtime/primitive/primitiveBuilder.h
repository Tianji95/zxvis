#pragma once
#ifndef _PRIMITIVE_BUILDER_H_
#define _PRIMITIVE_BUILDER_H_
#include "primitive.h"
#include "rectangle.h"
#include "vertex.h"

class PrimitiveBuilder {
public:
	PrimitiveBuilder() = default;
	~PrimitiveBuilder() = default;
	virtual void BuildRectangle(vec3& pos, float width, float height);
	virtual void BuildCircle(vec3& pos, float r);
	virtual void SetColor(vec3& c);
	virtual void SetScale(vec3& s);

	virtual VertexBuffer GetVertexBuffer();
	virtual IndexBuffer GetIndexBuffer();
private:
	virtual void ResetIndexBuffer();
	virtual void ResetVertexBuffer();
	vec3 color;
	vec3 scale;
	std::vector<Primitive*> prims;
	VertexBuffer vb;
	IndexBuffer ib;
	int indexOffset = 0;
};

#endif // _PRIMITIVE_BUILDER_H_