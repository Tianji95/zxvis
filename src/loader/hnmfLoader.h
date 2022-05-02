#pragma once
#ifndef _HNMF_LOADER_H_
#define _HNMF_LOADER_H_
#include <string>
#include <map>
#include <vector>
#include "vec.h"

struct SubBlock {
	std::string name;
	vec4 color;
	float percentage;
};
struct Block {
	std::vector<SubBlock> data;
	std::string name;
};
using YearData = std::vector<Block>;
using HnmfData = std::map<int, YearData>;

class HNMFLoader {
public:
	HNMFLoader() = default;
	virtual ~HNMFLoader() = default;
	virtual HnmfData LoadData(std::string& path) = 0;
private:

};
#endif // !_LOADER_H_
