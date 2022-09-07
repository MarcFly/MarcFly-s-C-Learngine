#ifndef MFLY_GPU
#define MFLY_GPU

#include <stdint.h>
#include <vulkan/vulkan.hpp>

// UNDERSTAND MACRO TO DISABLE OPTIMIZATIONS IN VMKGL
#ifdef MEM_OPTIMIZE
#undef MEMOPT;
#define MEMOPT(a) ;
#define MEMOPT_ALT(a) a;
#else
#define MEMOPT(a) a;
#undef MEMOPT_ALT;
#define MEMOPT_ALT(a) ; 
#endif

namespace mfly
{
    namespace gpu
    {
        // Module Functions
        uint16_t DefaultInit();

        // Function to gather Vulkan Instance possible Extensions
        struct VkPhysDeviceInfoWrap {
            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures features;
        };

        struct VkInstanceInfoWrap{
            VkApplicationInfo app_info = {};
            VkInstanceCreateInfo create_info = {};
            std::vector<const char*> exts;
            std::vector<const char*> layers;
            
            std::vector<VkExtensionProperties> available_exts;
        };
        uint16_t InitVkInstance(VkInstanceInfoWrap& instance_info); // Startup Vulkan
        
        struct VkLDeviceInfoWrap {
            std::vector<VkDeviceQueueCreateInfo> queues_info;
            std::vector<std::vector<float>> prios;
            VkDeviceCreateInfo create_info = {};
            std::vector<const char*> exts;
            std::vector<void*> exts_info;
        };

        uint16_t InitQueues(VkLDeviceInfoWrap& info, uint16_t phys_dvc_handle = 0);

        uint16_t CreateSwapchain(VkSwapchainCreateInfoKHR info, uint16_t surface_handle, uint16_t logical_dvc_handle);
        
        struct VkShaderInfoWrap{
            uint32_t* bytecode;
            uint64_t code_size;
            uint16_t existing_shader = UINT16_MAX;
        };
        
        struct VkShaderStageInfoWrap {
            uint32_t start, end; // Uses 2 values for start and end when used in a bulk
            // Recreating a pipeline should be asking the previously created pipeline again...
            
            uint16_t logical_dvc;
            uint32_t stage;
            const char* name;
        };

        struct VkPipelineStateInfoWrap {

        };

        struct VkShaderBulk {
            std::vector<VkShaderInfoWrap> shader_infos;
            std::vector<VkShaderStageInfoWrap> declares;
        };

        uint16_t AddShader(VkShaderInfoWrap shader_info, uint16_t logical_dvc, uint32_t stage);
        uint16_t AddShaders(const VkShaderBulk& shader_bulk);
        
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
        struct VkDMEMHandles {
            uint16_t logical_dvc_handle = 0;
            uint16_t mem_handle = -1;
            uint32_t mem_info_handle = -1;
        };
        struct VkBufferInfoWrap {
            VkBufferCreateInfo buffer = {};
            VkDMEMHandles handles;
            std::vector<VkBufferViewCreateInfo> views;
        };
        uint32_t CreateBuffer(VkBufferInfoWrap info);
        
        struct VkImageInfoWrap {
            VkImageCreateInfo img = {};
            VkDMEMHandles handles;
            std::vector<VkImageViewCreateInfo> views = {};
        };
        uint32_t CreateImage(VkImageInfoWrap info);

        struct VkCmdPoolWrap {
            VkCommandPoolCreateInfo info = {};
            VkCommandPool pool;
            VkCommandBufferAllocateInfo alloc_info = {};
            //VkComandBuffer buf;
            VkCommandBufferBeginInfo begin_info = {};
        };

    };
};

#endif // MFLY_GPU