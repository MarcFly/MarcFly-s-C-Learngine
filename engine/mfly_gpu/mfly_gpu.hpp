#ifndef MFLY_GPU
#define MFLY_GPU

#include <stdint.h>
#include <vulkan/vulkan.hpp>

namespace mfly
{
    namespace gpu
    {
        // Module Functions
        uint16_t DefaultInit();

        // Function to gather Vulkan Instance possible Extensions
        struct VkInstanceWrap{
            VkApplicationInfo app_info = {};
            VkInstanceCreateInfo create_info = {};
            std::vector<const char*> exts;
            std::vector<const char*> layers;
        };
        uint16_t InitVkInstance(VkInstanceWrap instance_info); // Startup Vulkan
        

        // Function to Gather Physical Device possible Extensions
        struct VkLDeviceWrap {
            std::vector<VkDeviceQueueCreateInfo> queues_info;
            std::vector<std::vector<float>> prios;
            VkDeviceCreateInfo create_info = {};
            std::vector<const char*> exts;
        };
        uint16_t DeclareQueue(VkDeviceQueueCreateInfo info, float* prios); // Declare a new queue for vulkan
        uint16_t InitQueues(VkLDeviceWrap info, uint32_t phys_device_handle = 0); // Gather non initialized queues and create a logical device for them
        
        uint16_t CreateSwapchain(VkSwapchainCreateInfoKHR info, uint16_t surface_handle, uint16_t logical_dvc_handle);
        uint16_t Close();
        uint16_t PreUpdate();
        uint16_t DoUpdate();
        uint16_t PostUpdate();
        uint16_t AsyncDispatch();
        uint16_t AsyncGather();

        //uint16_t event_code_start = 0;

        enum ERRORCODE
        {
            GOOD = 0,
            BAD = 1,
        };
#if defined(DEBUG_STRINGS)
        const char*[] debug_strings
        {
            "Good, move on.",
            "Something went wrong, message not set",
        }
        const char* DebugMessage(uint16_t errorcode) {
            if(errorcode > (sizeof(debug_string)/sizeof(const char*)))
                return debug_string[1];
            else
                return debug_string[errorcode];
        }
#endif

        // Module Specifics
        /// void*: VkInstance the library provides and the window api most likely requires
        /// uint16_t: Window number that we want a surface from (usually the main window - 0)
        /// expected result: VkSurfaceKHR
        typedef void*(*GetSurfaceFun)(void*, uint16_t);
        void ProvideSurfaceFun(GetSurfaceFun fun);


        struct VkMemInfoWrap {
            VkMemoryRequirements mem = {};
            VkMemoryAllocateInfo mem_info = {};
        };
        // Memory Allocation?
        struct VkBufferInfoWrap {
            VkBufferCreateInfo buffer = {};
            uint16_t logical_dvc_handle = 0;
            uint16_t mem_handle = -1;
            
            std::vector<VkBufferViewCreateInfo> views;
        };
        uint16_t CreateBuffer(VkBufferInfoWrap info);
        
    };
};

#endif // MFLY_GPU