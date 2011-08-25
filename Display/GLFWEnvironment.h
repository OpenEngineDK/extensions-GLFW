// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_G_L_F_W_ENVIRONMENT_H_
#define _OE_G_L_F_W_ENVIRONMENT_H_


#include <Display/IEnvironment.h>
#include <Display/GLFWFrame.h>
#include <Devices/GLFWInput.h>
#include <Meta/OpenGL.h>
#include <GL/glfw.h>
#include <list>
#include <sys/param.h>
#include <unistd.h>


namespace OpenEngine {
namespace Display {

using namespace std;

/**
 * Short description.
 *
 * @class GLFWEnvironment GLFWEnvironment.h ons/GLFW/Display/GLFWEnvironment.h
 */
class GLFWEnvironment : public IEnvironment {
private:
    int w, h, d;
    FrameOption o;
    
    Devices::GLFWInput* input;

    list<GLFWFrame*> frames;

public:
    GLFWEnvironment(int w, int h, int d=32, FrameOption options = FrameOption()) 
    : w(w), h(h), d(d), o(options) {
            
        // I'm gonna cheat glfw by resetting the cwd!
        char buf[MAXPATHLEN];
        getwd(buf);
            
        glfwInit();
        
        chdir(buf);
        
        input = new Devices::GLFWInput();
    }
    

    void Handle(Core::InitializeEventArg arg) {
        list<GLFWFrame*>::iterator i = frames.begin();
        for (; i != frames.end(); ++i) {
            (*i)->Handle(arg);
        }
        input->Handle(arg);
    }
    void Handle(Core::ProcessEventArg arg) {
        list<GLFWFrame*>::iterator i = frames.begin();
        for (; i != frames.end(); ++i)
            (*i)->Handle(arg);
        input->Handle(arg);
    }
    void Handle(Core::DeinitializeEventArg arg) {
        list<GLFWFrame*>::iterator i = frames.begin();
        for (; i != frames.end(); ++i)
            (*i)->Handle(arg);
        input->Handle(arg);
    }

    IFrame& CreateFrame() { 
        GLFWFrame* f = new GLFWFrame(w, h, d, o);
        f->input = input;
        frames.push_back(f);
        return *f;
    }

    Devices::IMouse*    GetMouse()    { return input; }
    Devices::IKeyboard* GetKeyboard() { return input; }
    Devices::IJoystick* GetJoystick() { return input; }

};

} // NS Display
} // NS OpenEngine

#endif // _OE_G_L_F_W_ENVIRONMENT_H_
