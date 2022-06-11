#pragma once
#include "vec.h"

struct VertexBuffer {
	unsigned int bufferSize = 0;
	unsigned int stride = 0;
	uint8_t* data = nullptr;
};

struct IndexBuffer {
	unsigned int bufferSize = 0;
	unsigned int typeSize = 0;
	uint8_t* data = nullptr;
};

class Primitive {
public:
	Primitive(){ }
	virtual ~Primitive(){ }
	virtual VertexBuffer GetVerticesBuffer() = 0;
	virtual IndexBuffer GetIndicesBuffer() = 0;
	virtual size_t GetVertexCount() = 0;
};
