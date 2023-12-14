#include "renderer_frontend.h"

#include "renderer_backend.h"

#include "core/kmemory.h"
#include "core/logger.h"

static renderer_backend *backend = 0;

b8 renderer_initialise(const char *application_name, struct platform_state *plat_state) {
    backend = kallocate(sizeof(renderer_backend), MEMORY_TAG_RENDERER);

    renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend);
    backend->frame_number = 0;

    if (!backend->initialise(backend, application_name, plat_state)) {
        KFATAL("Renderer backend failed to initialise. Shutting down");
        return false;
    }

    return true;
}

void renderer_shutdown() {
    backend->shutdown(backend);
    kfree(backend, sizeof(renderer_backend), MEMORY_TAG_RENDERER);
}

b8 renderer_begin_frame(f32 delta_time) {
    return backend->begin_frame(backend, delta_time);
}

b8 renderer_end_frame(f32 delta_time) {
    b8 result = backend->end_frame(backend, delta_time);
    backend->frame_number++;
    return result;
}

void renderer_on_resized(u16 width, u16 height) {
    if (backend) {
        backend->resized(backend, width, height);
    } else {
        KWARN("renderer backend does not exist to accept resize: %i %i", width, height);
    }
}

b8 renderer_draw_frame(render_packet *packet) {
    if (renderer_begin_frame(packet->delta_time)) {
        b8 result = renderer_end_frame(packet->delta_time);
        if (!result) {
            KERROR("renderer_end_frame failed. Shutting down");
            return false;
        }
    }
    return true;
}