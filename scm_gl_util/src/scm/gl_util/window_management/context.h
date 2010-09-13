
#ifndef SCM_GL_UTIL_WM_CONTEXT_H_INCLUDED
#define SCM_GL_UTIL_WM_CONTEXT_H_INCLUDED

#include <scm/core/pointer_types.h>

#include <scm/gl_util/window_management/wm_fwd.h>
#include <scm/gl_util/window_management/surface.h>

#include <scm/core/platform/platform.h>
#include <scm/core/utilities/platform_warning_disable.h>

namespace scm {
namespace gl {
namespace wm {

class __scm_export(gl_util) context
{
public:
    struct __scm_export(gl_util) attribute_desc {
        int             _version_major;
        int             _version_minor;
        bool            _compatibility_profile;
        bool            _debug;
        bool            _forward_compatible;

        attribute_desc(int version_major, int version_minor,
                       bool compatibility = false, bool debug = false, bool forward = false);
    }; // struct attribute_desc
public:
    context(const surface_ptr&      in_surface,
            const attribute_desc&   in_attributes,
            const context_ptr&      in_share_ctx = context_ptr());
    virtual ~context();

    bool                            make_current(const surface_ptr& in_surface, bool current = true);

    const display_ptr&              associated_display() const;
    const surface::format_desc&     surface_format() const;
    const attribute_desc&           context_attributes() const;

    static const attribute_desc&    default_attributes();

protected:
    const display_ptr&              _associated_display;
    surface::format_desc            _surface_format;
    attribute_desc                  _attributes;

    weak_ptr<surface>               _current_surface;

private:
    struct context_impl;
    shared_ptr<context_impl>        _impl;

private:
    // non_copyable
    context(const context&);
    context& operator=(const context&);

}; // class context

} // namespace wm
} // namepspace gl
} // namepspace scm

#include <scm/core/utilities/platform_warning_enable.h>

#endif // SCM_GL_UTIL_WM_CONTEXT_H_INCLUDED