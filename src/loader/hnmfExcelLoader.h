#pragma once
#ifndef _HNMF_EXCEL_LOADER_H_
#define _HNMF_EXCEL_LOADER_H_
#include "hnmfLoader.h"

class HNMFExcelLoader : public HNMFLoader {
public:
	HNMFExcelLoader() = default;
	virtual ~HNMFExcelLoader() = default;
	virtual HnmfData LoadData(std::string& path) override;
private:

};
#endif // !_HNMF_EXCEL_LOADER_H_
