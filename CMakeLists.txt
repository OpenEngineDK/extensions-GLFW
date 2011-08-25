
# Create the extension library
ADD_LIBRARY(Extensions_GLFW
  Display/GLFWEnvironment.h
  Display/GLFWFrame.h
  Display/GLFWFrame.cpp
  Devices/GLFWInput.h
  Devices/GLFWInput.cpp
)

TARGET_LINK_LIBRARIES(Extensions_GLFW
  glfw
)
