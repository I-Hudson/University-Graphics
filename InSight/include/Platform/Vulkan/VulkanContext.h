#pragma once

#include "InSight/Renderer/GraphicsContext.h"
#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace InSight
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> mGraphicsFamily;

		bool IsComplete()
		{
			return mGraphicsFamily.has_value();
		}
	};

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* aWindow);
		~VulkanContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

		void SetupDebugMessenger();
		void PickPhysicalDevice();
		void CreateLogicalDevice();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice aDevice);
		bool IsDeviceSuitable(VkPhysicalDevice aDevice);

	private:
		GLFWwindow* mWindowHandle;
		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mDevice;
		VkQueue mGrahicsQueue;

		bool mValidationLayersEnabled = true;
		std::vector<const char*> mValidationLayers;

	};
}