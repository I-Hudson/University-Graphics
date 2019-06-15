#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "InSight/Renderer/GraphicsContext.h"
#include <vector>
#include <optional>

namespace InSight
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;

		bool IsComplete()
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* aWindow);
		~VulkanContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		void ShutDown();

		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateGraphicsPipeline();
		void CreateFramebuffers();
		void CreateCommandPool();
		void CreateCommandBuffers();
		void CreateSemaphores();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice aDevice);

		bool IsDeviceSuitable(VkPhysicalDevice aDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice aDevice);

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice aDevice);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& aAvailableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& aAvailablePresents);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& aCapabilities);

		//HELPER FUNCTIONS MUST BE MOVED
		static std::vector<char> ReadFile(const std::string& aFileName);
		VkShaderModule CreateShaderModule(const std::vector<char>& aCode);

	private:
		GLFWwindow* mWindowHandle;

		VkInstance mInstance;
		VkSurfaceKHR mSurface;
		VkDebugUtilsMessengerEXT mDebugMessenger;

		VkPhysicalDevice mPhysicalDevice;
		VkDevice mDevice;

		VkQueue mGrahicsQueue;
		VkQueue mPresentQueue;

		VkSwapchainKHR mSwapChain;
		std::vector<VkImage> mSwapChainImages;
		VkFormat mSwapChainImageFormat;
		VkExtent2D mSwapChainExtent;

		std::vector<VkImageView> mSwapChainImageViews;

		VkRenderPass mRenderPass;
		VkPipelineLayout mPipelineLayout;

		VkPipeline mGraphicsPipeline;

		std::vector<VkFramebuffer> mSwapChainFramebuffers;

		VkCommandPool mCommandPool;
		std::vector<VkCommandBuffer> mCommandBuffers;

		VkSemaphore mImageAvailableSemaphore;
		VkSemaphore mRenderFinishedSemaphore;

		bool mValidationLayersEnabled = true;
		std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		std::vector<const char*> mDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};
}