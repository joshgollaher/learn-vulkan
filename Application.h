#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <vector>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	static QueueFamilyIndices find(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		QueueFamilyIndices qf{};

		uint32_t familyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
		std::vector<VkQueueFamilyProperties> families(familyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

		int i = 0;
		for(const auto& family : families)
		{
			if(qf.isSuitable())
			{
				break;
			}

			if(family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				qf.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (presentSupport)
			{
				qf.presentFamily = i;
			}

			i++;
		}

		return qf;
	}

	bool isSuitable() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;

	static SwapChainSupportDetails find(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails details{};

		//Get capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		//Get formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if(formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		//Get present modes
		uint32_t modeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr);
		if (modeCount != 0)
		{
			details.presentModes.resize(modeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, details.presentModes.data());
		}


		return details;
	}

	bool isSuitable() const
	{
		return !presentModes.empty() && !formats.empty();
	}
};

class Application
{
private:

	VkDebugUtilsMessengerEXT debugMessenger;

	GLFWwindow* window;

	VkInstance instance;

	QueueFamilyIndices queueIndices;

	VkPhysicalDevice physicalDevice;
	VkDevice device;

	VkSurfaceKHR surface;

	VkQueue presentQueue;
	VkQueue graphicsQueue;

	SwapChainSupportDetails swapChainDetails;

	VkFormat swapChainFormat;

	VkExtent2D swapChainExtent;

	VkSwapchainKHR swapchain;

	std::vector<VkImage> swapChainImages;

	std::vector<VkImageView> swapChainImageViews;

	std::vector<VkFramebuffer> swapChainFrameBuffers;

	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

	VkPipeline graphicsPipeline;

	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

	//Why, Vulkan, why?
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& modes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void initializeVulkan();
	void mainLoop();
	void drawFrame();
	void dispose();

public:
	void run();
};

