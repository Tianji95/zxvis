#pragma once
#ifndef _SIMPLE_COLOR_PIPE_H_
#define _SIMPLE_COLOR_PIPE_H_
#include "pipelineInfo.h"

class SimpleColorPipe :public PipelineInfo {
public:
	SimpleColorPipe(VkDevice& device, VkRenderPass& rp, VkDescriptorSetLayout& dslayout, VkExtent2D& swapchainextent):
		PipelineInfo(device, rp, dslayout, swapchainextent)
	{
	}
	virtual ~SimpleColorPipe() = default;
	virtual void Create() override;
	virtual void Destroy() override;

protected:

private:

};
#endif // !_SIMPLE_COLOR_PIPE_H_
