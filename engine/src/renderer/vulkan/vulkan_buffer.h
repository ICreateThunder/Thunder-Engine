#pragma once

#include "vulkan_types.inl"

b8 vulkan_buffer_create(
    vulkan_context* context, 
    u64 size, 
    VkBufferUsageFlagBits usage,
    u32 memory_property_flags,
    b8 bind_on_create,
    vulkan_buffer* out_buffer);

void vulkan_buffer_destroy(vulkan_context* context, vulkan_buffer* buffer);

b8 vulkan_buffer_resize(
    vulkan_context* context,
    u64 new_size,
    vulkan_buffer* buffer,
    VkQueue queue,
    VkCommandPool pool);

void vulkan_buffer_bind(vulkan_context* context, vulkan_buffer* buffer, u64 offset);

void* vulkan_buffer_lock_memory(vulkan_context* context, vulkan_buffer* buffer, u64 offset, u64 size, u32 flags);
void vulkan_buffer_unlock_memory(vulkan_context* context, vulkan_buffer* buffer);

void vulkan_buffer_load_data(vulkan_context* context, vulkan_buffer* buffer, u64 offset, u64 size, u32 flags, const void* data);

void vulkan_buffer_copy_to(
    vulkan_context* context,
    VkCommandPool pool,
    VkFence fence,
    VkQueue queue,
    VkBuffer src,
    u64 src_offset,
    VkBuffer dst,
    u64 dst_offset,
    u64 size);