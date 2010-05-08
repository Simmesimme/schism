
#ifndef SCM_GL_CORE_FRAME_BUFFER_H_INCLUDED
#define SCM_GL_CORE_FRAME_BUFFER_H_INCLUDED

#include <vector>

#include <scm/core/math.h>

#include <scm/gl_core/constants.h>
#include <scm/gl_core/frame_buffer_objects/frame_buffer_objects_fwd.h>
#include <scm/gl_core/render_device/render_device_fwd.h>
#include <scm/gl_core/render_device/device_child.h>

#include <scm/core/platform/platform.h>
#include <scm/core/utilities/platform_warning_disable.h>

namespace scm {
namespace gl {

class __scm_export(gl_core) frame_buffer : public render_device_child
{
protected:
    struct attachment {
        attachment(const render_target_ptr& in_target = render_target_ptr(), unsigned in_level = 0, int in_layer = -1);
        bool operator==(const attachment& rhs) const;
        bool operator!=(const attachment& rhs) const;

        render_target_ptr   _target;
        unsigned            _level;
        int                 _layer;
    }; // struct attachment
    typedef std::vector<attachment> attachment_array;
    typedef std::vector<unsigned>   draw_buffer_array;

public:
    virtual ~frame_buffer();

    void                            attach_color_buffer(unsigned in_color_attachment, const render_target_ptr& in_target,
                                                        unsigned in_level = 0, unsigned in_layer = 0);
    void                            attach_depth_stencil_buffer(const render_target_ptr& in_target,
                                                                unsigned in_level = 0, unsigned in_layer = 0);

    void                            clear_attachments();

    const math::vec2ui&             drawable_region() const;

protected:
    frame_buffer(render_device& in_device);

    unsigned                        buffer_id() const;

    void                            bind(const render_context& in_context, frame_buffer_binding in_binding = FRAMEBUFFER_DRAW) const;
    void                            unbind(const render_context& in_context) const;

    void                            clear_color_buffer(const render_context&  in_context,
                                                       const unsigned         in_buffer,
                                                       const math::vec4f&     in_clear_color   = math::vec4f(0.0f));
    void                            clear_color_buffers(const render_context&  in_context,
                                                        const math::vec4f&     in_clear_color   = math::vec4f(0.0f));
    void                            clear_depth_stencil_buffer(const render_context&  in_context,
                                                               const float            in_clear_depth = 1.0f,
                                                               const int              in_clear_stencil = 0);
    
    void                            apply_attachments(const render_context& in_context);
    bool                            check_completeness(const render_context& in_context);

    void                            apply_attachment(const render_context& in_context, unsigned in_attach_point, const attachment& in_attachment);
    void                            clear_attachment(const render_context& in_context, unsigned in_attach_point);

protected:
    unsigned                        _gl_buffer_id;

    attachment_array                _selected_color_attachments;
    attachment                      _selected_depth_stencil_attachment;

    attachment_array                _current_color_attachments;
    attachment                      _current_depth_stencil_attachment;

    draw_buffer_array               _draw_buffers;
    math::vec2ui                    _drawable_region;

    mutable unsigned                _current_gl_binding;
    bool                            _attachments_dirty;

private:

    friend class render_device;
    friend class render_context;
}; // class frame_buffer

} // namespace gl
} // namespace scm

#include <scm/core/utilities/platform_warning_enable.h>

#endif // SCM_GL_CORE_FRAME_BUFFER_H_INCLUDED