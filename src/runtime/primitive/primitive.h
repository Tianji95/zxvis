#pragma once
#include "vec.h"

class Primitive {
public:
	Primitive(){ }
	virtual ~Primitive(){ }
	virtual void SetCenter(vec2& c)
	{
		center = c;
	}
	virtual void SetParams(vec2& p);
	virtual void* GetVertices();
	virtual void* GetIndices();
protected:
	vec2 center;
};

class PrimitiveRGB : public Primitive{
public:
	PrimitiveRGB()
	{
	}
	virtual ~PrimitiveRGB()
	{
	}
	virtual void SetColor(vec3& c)
	{
		color = c;
	}

protected:
	vec3 color;
};