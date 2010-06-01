
#ifndef SCM_GL_GUI_FRAME_H_INCLUDED
#define SCM_GL_GUI_FRAME_H_INCLUDED

#include <scm/core/gui/frame.h>

#include <scm/core/platform/platform.h>
#include <scm/core/utilities/platform_warning_disable.h>

namespace scm {
namespace gl_classic {
namespace gui {

class __scm_export(gl_core) frame : public scm::gui::frame
{
public:
    frame();
    virtual ~frame();

    virtual void                        draw();

protected:

private:

}; // class frame

} // namespace gui
} // namespace gl_classic
} // namespace scm

#include <scm/core/utilities/platform_warning_enable.h>

#endif // SCM_GL_GUI_FRAME_H_INCLUDED