#pragma once
#ifndef _PIPELINE_INFO_H_
#define _PIPELINE_INFO_H_
#include <vulkan/vulkan.h>

class PipelineInfo {
public:
	PipelineInfo(VkDevice& device, VkRenderPass& rp, VkDescriptorSetLayout& dslayout, VkExtent2D& swapchainextent):
		device(&device), renderPass(&rp), descriptorSetLayout(&dslayout), swapchainExtent(&swapchainextent)
	{
	}
	virtual void Create() = 0;
	virtual void Destroy() = 0;
	virtual VkPipeline GetvkPipeline()
	{
		return pipeline;
	}
	virtual VkPipelineLayout GetvkPipelineLayout()
	{
		return pipelineLayout;
	}
protected:
	VkRenderPass*      renderPass;
	VkDevice*         device;
	VkDescriptorSetLayout* descriptorSetLayout;
	VkExtent2D* swapchainExtent;
//
	VkShaderModule    vertexShaderModule;
	VkShaderModule    fragmentShaderModule;
	VkPipelineLayout  pipelineLayout;
	VkPipeline        pipeline;
};
#endif // !_PIPELINE_INFO_H_
