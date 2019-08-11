#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "InSight/Renderer/GraphicsContext.h"
#include <vector>
#include <array>
#include <optional>
#include <glm/glm.hpp>

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
		void CreateSyncObjects();
		void CleanSwapChain();
		void RecreateSwapChain();

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

		void SetFramebufferResize(bool aState);

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

		std::vector<VkSemaphore> mImageAvailableSemaphores;
		std::vector<VkSemaphore> mRenderFinishedSemaphores;
		std::vector<VkFence> mInFlightFences;
		int mCurrentFrame = 0;

		bool mFramebufferResized = false;

		const int MAX_FRAMES_IN_FLIGHT = 2;

		bool mValidationLayersEnabled = true;
		std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		std::vector<const char*> mDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	
		struct Vertex
		{
			glm::vec2 Pos;
			glm::vec3 Color;

			static VkVertexInputBindingDescription  GetBindingDescription()
			{
				VkVertexInputBindingDescription  bindingDesc = {};

				bindingDesc.binding = 0;
				bindingDesc.stride = sizeof(Vertex);
				bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDesc;
			}

			static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDesc()
			{
				std::array<VkVertexInputAttributeDescription, 2>attributeDesc = {};
				
				attributeDesc[0].binding = 0;
				attributeDesc[0].location = 0;
				attributeDesc[0].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDesc[0].offset = offsetof(Vertex, Pos);

				attributeDesc[1].binding = 0;
				attributeDesc[1].location = 1;
				attributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDesc[1].offset = offsetof(Vertex, Color);

				return attributeDesc;
			}
		};
		std::vector<Vertex> Vertices = 
		{
			{ {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} },
			{ {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} },
			{ {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} }
		};
	};
}