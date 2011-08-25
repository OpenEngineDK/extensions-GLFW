// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Meta/OpenGL.h>

#include "GLFWFrame.h"
#include <Display/ViewingVolume.h>
#include <Display/StereoCamera.h>
#include <Logging/Logger.h>
#include <sstream>
#include <map>
#include <GL/glfw.h>


namespace OpenEngine {
namespace Display {

using std::map;

// // evil callback hack
// static bool glutIsInitialized = false;
// static map<int, GLFWFrame*> windows;

// void GLFWFrame::display() {
//     // logger.info << "window: " << glutGetWindow() << logger.end;
//     // glutSwapBuffers();
// }

// void GLFWFrame::reshape(int w, int h) {
//     GLFWFrame* f = windows[glutGetWindow()];
//     f->width = w;
//     f->height = h;
//     ((IListener<Display::ResizeEventArg>*)f->canvas)->Handle(Display::ResizeEventArg(f->fc));
//     //    logger.info << "reshape: " << glutGetWindow() << logger.end;
// }

GLFWFrame::GLFWFrame(int w,int h,int d,FrameOption opts)
    : canvas(NULL)
    , fc(FrameCanvas(*this))
    , width(w)
    , height(h)
    , depth(d)
    , options(FrameOption(opts|FRAME_OPENGL)) 
    , init(false)
{
}

GLFWFrame::~GLFWFrame() {
}

bool GLFWFrame::IsFocused() const {
    throw "focus";
    return true;
}

string GLFWFrame::GetName() const {
    return name;
}

unsigned int GLFWFrame::GetWidth() const {
    return width;
}

unsigned int GLFWFrame::GetHeight() const {
    return height;
}

unsigned int GLFWFrame::GetDepth() const {
    return depth;
}

FrameOption GLFWFrame::GetOptions() const {
    return options;
}

bool GLFWFrame::GetOption(const FrameOption option) const {
    return (option & GetOptions()) == option;
}

void GLFWFrame::SetName(const string name) {
    this->name = name;
    //if (init) glutSetWindowTitle(name.c_str());
}

void GLFWFrame::SetWidth(const unsigned int width) {
    this->width = width;
    //if (init) glutReshapeWindow(width, height);
}

void GLFWFrame::SetHeight(const unsigned int height) {
    this->height = height;
    //if (init) glutReshapeWindow(width, height);
}

void GLFWFrame::SetDepth(const unsigned int depth) {
    this->depth = depth;
}

void GLFWFrame::SetOptions(const FrameOption options) {    
    this->options = options;
    glfwCloseWindow();
    InitFrame();
}

void GLFWFrame::ToggleOption(const FrameOption option) {
    FrameOption opt = FrameOption(options ^ option);
    SetOptions(opt);
}

void GLFWFrame::Handle(Core::InitializeEventArg arg) {
    if (init) return;
    // Initialize the video frame
    InitFrame();
    init = true;

    if (canvas) 
        ((IListener<Display::InitializeEventArg>*)canvas)->Handle(Display::InitializeEventArg(fc));

    if (renderModule)
        ((IListener<Core::InitializeEventArg>*)renderModule)->Handle(arg);


}

void GLFWFrame::InitFrame() {
    
    int r = glfwOpenWindow(width, height, 
                           0, 0, 0, 0,
                           0, 0, 
                           (options & FRAME_FULLSCREEN)?GLFW_FULLSCREEN:
                           GLFW_WINDOW);
    if (input)
        input->Reset();
    if (r == GL_FALSE) {
        throw "Failed to open window";
    }
    
    // int argc = 1;
    // char *argv[] = {(char*)"hest"};
    // logger.info << "Initialize GLFW" << logger.end;
    // glutInit(&argc,argv);
    // unsigned int mode = GLUT1_DOUBLE | GLUT_RGB | GLUT_DEPTH; 
    // if (IsOptionSet(FRAME_STEREO))
    //     mode |= GLUT_STEREO;
    // glutInitDisplayMode(mode);
    // glutInitWindowSize (width, height);
    // if (IsOptionSet(FRAME_FULLSCREEN)) {
    //     std::ostringstream os;
    //     os << width << "x" << height << ":" << depth;
    //     glutGameModeString( os.str().c_str() );
    //     if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE) == 0) 
    //         logger.info << "GLUT: Game mode not possible." << logger.end;
    //     logger.info << "GLUT: Enter game mode: " << os.str() << logger.end;
    //     glutEnterGameMode();
    //     window = glutGetWindow();
    // }
    // else {
    //     window = glutCreateWindow(name.c_str());
    //     windows[window] = this;
    // }
    // glutReshapeFunc(GLUTFrame::reshape);
 }

void GLFWFrame::Handle(Core::ProcessEventArg arg) {
    #ifdef OE_SAFE
    if (!init) throw new Exception("GLFWFrame not initialized");
    #endif

    // if (IsOptionSet(FRAME_STEREO)) {
    //     IViewingVolume* vol = vv;
    //     vv = stereo->GetLeft();
    //     glDrawBuffer(GL_BACK_LEFT);
    //     glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //     redrawEvent.Notify(RedrawEventArg(*this, arg.start, arg.approx));
    //     vv = stereo->GetRight();
    //     glDrawBuffer(GL_BACK_RIGHT);
    //     glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //     redrawEvent.Notify(RedrawEventArg(*this, arg.start, arg.approx));
    //     vv = vol;
    // }
    // else {
        // glDrawBuffer(GL_BACK);
        // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    if (canvas) 
        ((IListener<Display::ProcessEventArg>*)canvas)->Handle(ProcessEventArg(fc, arg.start, arg.approx));
    if (renderModule)
        ((IListener<Core::ProcessEventArg>*)renderModule)->Handle(arg);

    // }
    glfwSwapBuffers();
}

void GLFWFrame::Handle(Core::DeinitializeEventArg arg) {
    if (IsOptionSet(FRAME_FULLSCREEN)) {       
        //glutLeaveGameMode();
    }
    if (canvas)
        ((IListener<Display::DeinitializeEventArg>*)canvas)->Handle(DeinitializeEventArg(fc));
    if (renderModule)
        ((IListener<Core::DeinitializeEventArg>*)renderModule)->Handle(arg);

    //windows.erase(window);
    //glutDestroyWindow(window);
    init = false;
}

void GLFWFrame::SetRenderModule(IModule* renderModule) {
    this->renderModule = renderModule;
}

    
} // NS Display
} // NS OpenEngine


