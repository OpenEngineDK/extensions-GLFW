// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "GLFWInput.h"
#include <Logging/Logger.h>
#include <GL/glfw.h>

namespace OpenEngine {
namespace Devices {

using Core::Event;
using Core::InitializeEventArg;
using Core::ProcessEventArg;
using Core::DeinitializeEventArg;

static GLFWInput* input;

// Key specialToKey(unsigned int s) {
//     switch (s) {
//     case GLFW_KEY_UP: return KEY_UP;
//     case GLFW_KEY_DOWN: return KEY_DOWN;
//     case GLFW_KEY_LEFT: return KEY_LEFT;
//     case GLFW_KEY_RIGHT: return KEY_RIGHT;
        
//     }
//     return KEY_LAST;
// }

// void specialKeyDown(int key, int x, int y) {
//     // TODO, this call is repeated.
    
//     if (input) {
//         KeyboardEventArg arg;
//         arg.type = EVENT_PRESS;
//         arg.sym = specialToKey(key);
        
//         input->KeyEvent().Notify(arg);
//     }
// }

// void specialKeyUp(int key, int x, int y) {
//     if (input) {
//         KeyboardEventArg arg;
//         arg.type = EVENT_RELEASE;
//         arg.sym = specialToKey(key);

//         input->KeyEvent().Notify(arg);
//     }
// }

    
Key specialToKey(int k) {
    switch (k) {
        case GLFW_KEY_ESC: return KEY_ESCAPE;
    }
    return KEY_UNKNOWN;
}
    
void keyCallback(int key, int state) {
    if (input) {
        
        KeyboardEventArg arg;
        arg.type = (state == GLFW_PRESS)?EVENT_PRESS:EVENT_RELEASE;
        
        if (key >= GLFW_KEY_SPECIAL) {
            arg.sym = specialToKey(key);
        } else {
            if (key >= 'A' && key <= 'Z') {
                key = 'a' + (key - 'A'); // glfw uses uppercase, we expect lower
            }
            arg.sym = (Key)key;
        }
        input->KeyEvent().Notify(arg);
        
    }
}
    
void mouseCallback(int x, int y) {
    if (input) {
        MouseMovedEventArg arg;
        
        arg.x = x;
        arg.y = y;
        
        arg.dx = x - input->state.x;
        arg.dy = y - input->state.y;
        
        
        input->state.x = x;
        input->state.y = y;
        
        arg.buttons = input->state.buttons;

        
        input->MouseMovedEvent().Notify(arg);
    }    
}
    
void mouseButtonCallback(int btn, int state) {    
    if (input) {
        MouseButtonEventArg arg;
        arg.type = (state == GLFW_PRESS)?EVENT_PRESS:EVENT_RELEASE;
        
        MouseButton button;
        
        switch (btn) {
            case GLFW_MOUSE_BUTTON_LEFT: button = BUTTON_LEFT; break;
            case GLFW_MOUSE_BUTTON_MIDDLE: button = BUTTON_MIDDLE; break;
            case GLFW_MOUSE_BUTTON_RIGHT: button = BUTTON_RIGHT; break;
        }
        
        arg.button = button;
        
        if (state == GLFW_PRESS)
            input->state.buttons = MouseButton(input->state.buttons | arg.button); // Set
        else 
            input->state.buttons = MouseButton(input->state.buttons & ~arg.button); // Unset
        
        
        input->MouseButtonEvent().Notify(arg);
    }
    
}
    
void mouseWheelCallback(int w) {
    
    if (input) {
        
        MouseButtonEventArg arg;
        
        if (w < input->wheelPos) {
            arg.button = BUTTON_WHEEL_DOWN;
        } else if (w > input->wheelPos) {
            arg.button = BUTTON_WHEEL_UP;
        }
        
        input->wheelPos = w;
        logger.info << w << logger.end;
        
        input->MouseButtonEvent().Notify(arg);
    }
    
    
}

    

GLFWInput::GLFWInput()
    : IKeyboard()
    , IMouse()
    , IJoystick() {


}

GLFWInput::~GLFWInput() {}

void GLFWInput::Reset() {
    glfwSetKeyCallback(keyCallback);    
    glfwSetMousePosCallback(mouseCallback);
    glfwSetMouseButtonCallback(mouseButtonCallback);
    glfwSetMouseWheelCallback(mouseWheelCallback);
}
    
void GLFWInput::Handle(InitializeEventArg arg) {
    // Move to initialize?
    input = this;
    
    Reset();
    


    // glutMouseFunc(mouseDown);
    // glutPassiveMotionFunc(mouseMove);
    // glutMotionFunc(mouseMove);

}
void GLFWInput::Handle(ProcessEventArg arg) {
    keyEvent.Release();
    mouseMovedEvent.Release();
    mouseButtonEvent.Release();

}
void GLFWInput::Handle(DeinitializeEventArg arg) {

}


// Events

IEvent<KeyboardEventArg>& GLFWInput::KeyEvent() {
    return this->keyEvent;
}

IEvent<MouseMovedEventArg>& GLFWInput::MouseMovedEvent() {
    return this->mouseMovedEvent;
}

IEvent<MouseButtonEventArg>& GLFWInput::MouseButtonEvent() {
    return this->mouseButtonEvent;
}

IEvent<JoystickButtonEventArg>& GLFWInput::JoystickButtonEvent() {
    return this->joystickButtonEvent;
}

IEvent<JoystickAxisEventArg>& GLFWInput::JoystickAxisEvent() {
    return this->joystickAxisEvent;
}


// IMouse
void GLFWInput::HideCursor() {
    throw "hide";
}

void GLFWInput::ShowCursor() {
    throw "show";
}

void GLFWInput::SetCursor(int x, int y) {
    throw "set";
}


MouseState GLFWInput::GetState() {
    return state;
}



} // NS Devices
} // NS OpenEngine

