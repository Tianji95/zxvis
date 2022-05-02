#include "hnmfExcelLoader.h"
#include <OpenXLSX.hpp>

static std::vector<vec4> colors = {
	{1, 0.855, 0.7255,1}, // PeachPuff 255 218 185
	{0.553, 0.9333, 0.9333,1}, // DarkSlateGray2 141 238 238
	{0.902,0.902,0.98,1}, //	lavender   230 230 250
	{0.3294,1,0.62353,1}, // SeaGreen1    84 255 159
	{0.51765,0.439,1,1}, // LightSlateBlue   132 112 255

	{1,0.9255,0.545,1}, // LightGoldenrod1   255 236 139
	{1,0.4157,0.4157,1}, // IndianRed1  255 106 106
	{0.804,0.788,0.788,1}, // 	Snow3  205 201 201
};

HnmfData HNMFExcelLoader::LoadData(std::string& path)
{
	HnmfData out;
	OpenXLSX::XLDocument doc;
	doc.open(path);
	auto names = doc.workbook().worksheetNames();

	for (auto& year : names) {
		auto sheet  = doc.workbook().worksheet(year);
		int intyear = std::stoi(year);
		int rowIdx  = 0;

		std::vector<std::string> titles;
		YearData yearData;
		for (auto& row : sheet.rows()) {
			std::vector<OpenXLSX::XLCellValue> readValues = row.values();
			// get titles;
			if (rowIdx == 0) {
				for (int colIdx = 0; colIdx < readValues.size(); colIdx++) {
					titles.push_back(readValues[colIdx]);
				}
				rowIdx++;
				continue;
			}
			Block block;
			block.name = readValues[0].get<std::string>();
			for (int colIdx = 1; colIdx < readValues.size(); colIdx++) {
				SubBlock subBlock;
				subBlock.name = titles[colIdx];
				if (readValues[colIdx].type() == OpenXLSX::XLValueType::Integer) {
					subBlock.percentage = readValues[colIdx].get<int>();
				}
				else if (readValues[colIdx].type() == OpenXLSX::XLValueType::Float) {
					subBlock.percentage = readValues[colIdx].get<float>();
				}
				subBlock.color = colors[colIdx % colors.size()];
				block.data.push_back(subBlock);
			}
			yearData.push_back(block);
			rowIdx++;
		}
		out[intyear] = yearData;
	}
	return out;
}
