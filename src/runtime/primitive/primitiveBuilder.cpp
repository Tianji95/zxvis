#include "primitiveBuilder.h"

void PrimitiveBuilder::BuildRectangle(vec3& pos, float width, float height)
{
	Primitive* prim = new RectangleRGB(pos, width, height, scale, color, indexOffset);
	prims.push_back(prim);
	indexOffset += 4;
	if (vb.data != nullptr) {
		ResetVertexBuffer();
	}
	if (ib.data != nullptr) {
		ResetIndexBuffer();
	}
}

void PrimitiveBuilder::BuildCircle(vec3& pos, float r)
{

}

void PrimitiveBuilder::SetColor(vec3& c)
{
	color = c;
}

void PrimitiveBuilder::SetScale(vec3& s)
{
	scale = s;
}

VertexBuffer PrimitiveBuilder::GetVertexBuffer()
{
	if (vb.data != nullptr) {
		return vb;
	}
	unsigned int vbsize = 0;
	for (auto prim : prims) {
		VertexBuffer tmpvb = prim->GetVerticesBuffer();
		vbsize += tmpvb.bufferSize;
		vb.stride = tmpvb.stride;
	}
	vb.bufferSize = vbsize;
	vb.data = (uint8_t*)malloc(vbsize);
	vbsize = 0;
	for (auto prim : prims) {
		VertexBuffer tmpvb = prim->GetVerticesBuffer();
		memcpy_s(vb.data + vbsize, tmpvb.bufferSize, tmpvb.data, tmpvb.bufferSize);
		vbsize += tmpvb.bufferSize;
	}
	return vb;
}

IndexBuffer PrimitiveBuilder::GetIndexBuffer()
{
	if (ib.data != nullptr) {
		return ib;
	}
	unsigned int ibsize = 0;
	for (auto prim : prims) {
		IndexBuffer tmpib = prim->GetIndicesBuffer();
		ibsize += tmpib.bufferSize;
		ib.typeSize = tmpib.typeSize;
	}
	ib.bufferSize = ibsize;
	ib.data = (uint8_t*)malloc(ibsize);
	ibsize = 0;
	for (auto prim : prims) {
		IndexBuffer tmpib = prim->GetIndicesBuffer();
		memcpy_s(ib.data + ibsize, tmpib.bufferSize, tmpib.data, tmpib.bufferSize);
		ibsize += tmpib.bufferSize;
	}
	return ib;
}
void PrimitiveBuilder::ResetIndexBuffer()
{
	ib.bufferSize = 0;
	if (ib.data != nullptr) {
		free(ib.data);
		ib.data = nullptr;
	}
	ib.typeSize = 0;
}

void PrimitiveBuilder::ResetVertexBuffer()
{
	vb.bufferSize = 0;
	if (vb.data != nullptr) {
		free(vb.data);
		vb.data = nullptr;
	}
	vb.stride = 0;
}