#include "renderer/vulkan/vulkan_backend.h"

#include "vulkan_types.inl"
#include "vulkan_platform.h"
#include "vulkan_device.h"
#include "vulkan_swapchain.h"
#include "vulkan_renderpass.h"
#include "vulkan_command_buffer.h"
#include "vulkan_framebuffer.h"
#include "vulkan_fence.h"

#include "core/logger.h"
#include "core/kstring.h"
#include "core/kmemory.h"
#include "core/application.h"

#include "containers/darray.h"

#include "platform/platform.h"

static vulkan_context context;
static u32 cached_framebuffer_width = 0;
static u32 cached_framebuffer_height = 0;

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_types,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
  void* user_data);

i32 find_memory_index(u32 type_filter, u32 property_flag);

void create_command_buffers(renderer_backend* backend);
void regenerate_framebuffers(renderer_backend* backend, vulkan_swapchain* swapchain, vulkan_renderpass* renderpass);

b8 vulkan_renderer_backend_initialise(renderer_backend* backend, const char* application_name, struct platform_state* plat_state) {
  
  context.find_memory_index = find_memory_index;

  context.allocator = 0;

  application_get_framebuffer_size(&cached_framebuffer_width, &cached_framebuffer_height);
  context.framebuffer_width = (cached_framebuffer_width != 0) ? cached_framebuffer_width : 800;
  context.framebuffer_height = (cached_framebuffer_height != 0) ? cached_framebuffer_height : 600;
  cached_framebuffer_width = 0;
  cached_framebuffer_height = 0;
  
  VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  app_info.apiVersion = VK_VERSION_1_3;
  app_info.pApplicationName = application_name;
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "Thunder Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

  VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };

  const char** required_extensions = darray_create(const char*);
  darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);
  platform_get_required_extension_names(&required_extensions);
  #if defined(_DEBUG)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    KDEBUG("Required extensions:");
    u32 length = darray_length(required_extensions);
    for (u32 i = 0; i < length; ++i) {
      KDEBUG(required_extensions[i]);
    }
  #endif

  create_info.pApplicationInfo = &app_info;
  create_info.enabledExtensionCount = darray_length(required_extensions);
  create_info.ppEnabledExtensionNames = required_extensions;

  const char** required_validation_layer_names = 0;
  u32 required_validation_layer_count = 0;

  #if defined(_DEBUG)
  KINFO("Validation layers enabled. Enumerating...");

  required_validation_layer_names = darray_create(const char*);
  darray_push(required_validation_layer_names, &"VK_LAYER_KHRONOS_validation");
  required_validation_layer_count = darray_length(required_validation_layer_names);

  u32 available_layer_count = 0;
  VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
  VkLayerProperties* available_layers = darray_reserve(VkLayerProperties, available_layer_count);
  VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));

  for (u32 i = 0; i < required_validation_layer_count; ++i) {
    KINFO("Searching for layer: %s...", required_validation_layer_names[i]);
    b8 found = FALSE;
    for (u32 j = 0; j < available_layer_count; ++j) {
      if (strings_equal(required_validation_layer_names[i], available_layers[j].layerName)) {
        found = TRUE;
        KINFO("Found.");
        break;
      }
    }
    if (!found) {
      KFATAL("Required validation layer is missing: %s", required_validation_layer_names[i]);
      return FALSE;
    }

    darray_destroy(available_layers);

    KINFO("All required validation layers are present.");
  }
  #endif

  create_info.enabledLayerCount = required_validation_layer_count;
  create_info.ppEnabledLayerNames = required_validation_layer_names;

  VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance));

  darray_destroy(required_extensions);

  KDEBUG("Vulkan instance created");

  darray_destroy(required_validation_layer_names);

  #if defined(_DEBUG)
  KDEBUG("Creating Vulkan debugger...");
  u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
  debug_create_info.messageSeverity = log_severity;
  debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
  debug_create_info.pfnUserCallback = vk_debug_callback;

  PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance,"vkCreateDebugUtilsMessengerEXT");
  KASSERT_MSG(func, "Failed to create debug messenger!");
  VK_CHECK(func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger));
  KDEBUG("Vulkan debugger created");
  #endif

    KDEBUG("Creating vulkan surface...");
    if (!platform_create_vulkan_surface(plat_state, &context)) {
      KERROR("Failed to create platform surface");
      return FALSE;
    }
    KDEBUG("Vulkan surface created");

  if (!vulkan_device_create(&context)) {
    KERROR("Failed to create device!");
    return FALSE;
  }

  vulkan_swapchain_create(&context, context.framebuffer_width, context.framebuffer_height, &context.swapchain);

  vulkan_renderpass_create(&context, &context.main_renderpass, 0, 0, context.framebuffer_width, context.framebuffer_height, 0.0f, 0.0f, 2.0f, 1.0f, 1.0f, 0);

  context.swapchain.framebuffers = darray_reserve(vulkan_framebuffer, context.swapchain.image_count);
  regenerate_framebuffers(backend, &context.swapchain, &context.main_renderpass);

  create_command_buffers(backend);

  context.image_available_semaphores = darray_reserve(VkSemaphore, context.swapchain.max_frames_in_flight);
  context.queue_complete_semaphores = darray_reserve(VkSemaphore, context.swapchain.max_frames_in_flight);
  context.in_flight_fences = darray_reserve(vulkan_fence, context.swapchain.max_frames_in_flight);

  for (u8 i = 0; i < context.swapchain.max_frames_in_flight; ++i) {
    VkSemaphoreCreateInfo semaphore_create_info = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    vkCreateSemaphore(context.device.logical_device, &semaphore_create_info, context.allocator, &context.image_available_semaphores[i]);
    vkCreateSemaphore(context.device.logical_device, &semaphore_create_info, context.allocator, &context.queue_complete_semaphores[i]);

    vulkan_fence_create(&context, TRUE, &context.in_flight_fences[i]);
  }

  context.images_in_flight = darray_reserve(vulkan_fence, context.swapchain.image_count);
  for (u32 i = 0; i < context.swapchain.image_count; ++i) {
    context.images_in_flight[i] = 0;
  }

  KINFO("Vulkan renderer initialised successfully");
  return TRUE;
}

void vulkan_renderer_backend_shutdown(renderer_backend* backend) {
  vkDeviceWaitIdle(context.device.logical_device);

  for (u8 i = 0; i < context.swapchain.max_frames_in_flight; ++i) {
    if (context.image_available_semaphores[i]) {
      vkDestroySemaphore(context.device.logical_device, context.image_available_semaphores[i], context.allocator);
      context.image_available_semaphores[i] = 0;
    }
    if (context.queue_complete_semaphores[i]) {
      vkDestroySemaphore(context.device.logical_device, context.queue_complete_semaphores[i], context.allocator);
      context.queue_complete_semaphores[i] = 0;
    }
    vulkan_fence_destroy(&context, &context.in_flight_fences[i]);
  }

  darray_destroy(context.image_available_semaphores);
  context.image_available_semaphores = 0;

  darray_destroy(context.queue_complete_semaphores);
  context.queue_complete_semaphores = 0;

  darray_destroy(context.in_flight_fences);
  context.in_flight_fences = 0;

  darray_destroy(context.images_in_flight);
  context.images_in_flight = 0;

  for (u32 i = 0; i < context.swapchain.image_count; ++i) {
    if (context.graphics_command_buffers[i].handle) {
      vulkan_command_buffer_free(&context, context.device.graphics_command_pool, &context.graphics_command_buffers[i]);
      context.graphics_command_buffers[i].handle = 0;
    }
  }
  darray_destroy(context.graphics_command_buffers);
  context.graphics_command_buffers = 0;

  for (u32 i = 0; i < context.swapchain.image_count; ++i) {
    vulkan_framebuffer_destroy(&context, &context.swapchain.framebuffers[i]);
  }

  KDEBUG("Destroying renderpass");
  vulkan_renderpass_destroy(&context, &context.main_renderpass);

  KDEBUG("Destroying swapchain");
  vulkan_swapchain_destroy(&context, &context.swapchain);

  KDEBUG("Destroying Vulkan device...");
  vulkan_device_destroy(&context);

  KDEBUG("Destroying Vulkan surface...")
  if (context.surface) {
    vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
    context.surface = 0;
  }

  KDEBUG("Destroying Vulkan debugger...");
  if (context.debug_messenger) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
    func(context.instance, context.debug_messenger, context.allocator);
  }

  KDEBUG("Destroying Vulkan instance");
  vkDestroyInstance(context.instance, context.allocator);

}

void vulkan_renderer_backend_on_resized(renderer_backend* backend, u16 width, u16 height) {

}

b8 vulkan_renderer_backend_begin_frame(renderer_backend* backend, f32 delta_time) {
  return TRUE;
}

b8 vulkan_renderer_backend_end_frame(renderer_backend* backend, f32 delta_time) {
  return TRUE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_types,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
  void* user_data) {
  switch(message_severity) {
    default:
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      KERROR(callback_data->pMessage);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      KWARN(callback_data->pMessage);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      KINFO(callback_data->pMessage);
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      KTRACE(callback_data->pMessage);
      break;
  }
  return VK_FALSE;
}

i32 find_memory_index(u32 type_filter, u32 property_flags) {
  VkPhysicalDeviceMemoryProperties memory_properties;
  vkGetPhysicalDeviceMemoryProperties(context.device.physical_device, &memory_properties);

  for (u32 i = 0; i < memory_properties.memoryTypeCount; ++i) {
    if (type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
      return i;
    }
  }

  KWARN("Unable to find suitable memory type!");
  return -1;
}

void create_command_buffers(renderer_backend* backend) {
  if (!context.graphics_command_buffers) {
    context.graphics_command_buffers = darray_reserve(vulkan_command_buffer, context.swapchain.image_count);
    for (u32 i = 0; i < context.swapchain.image_count; ++i) {
      kzero_memory(&context.graphics_command_buffers[i], sizeof(vulkan_command_buffer));
    }
  }
  
  for (u32 i = 0; i < context.swapchain.image_count; ++i) {
    if (context.graphics_command_buffers[i].handle) {
      vulkan_command_buffer_free(&context, context.device.graphics_command_pool, &context.graphics_command_buffers[i]);
    }
    kzero_memory(&context.graphics_command_buffers[i], sizeof(vulkan_command_buffer));
    vulkan_command_buffer_allocate(&context, context.device.graphics_command_pool, TRUE, &context.graphics_command_buffers[i]);
  }

  KINFO("Vulkan command buffers created");
}

void regenerate_framebuffers(renderer_backend* backend, vulkan_swapchain* swapchain, vulkan_renderpass* renderpass) {
  for (u32 i = 0; i < swapchain->image_count; ++i) {
    u32 attachment_count = 2;
    VkImageView attachments[] = { swapchain->views[i], swapchain->depth_attachment.view };

    vulkan_framebuffer_create(&context, renderpass, context.framebuffer_width, context.framebuffer_height, attachment_count, attachments, &context.swapchain.framebuffers[i]);
  }
}