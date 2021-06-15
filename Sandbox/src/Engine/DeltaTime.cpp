#include "xpch.h"
#include "DeltaTime.h"
#include "GLFW/glfw3.h"

//returns the time in seconds since the start of the program
double DeltaTime::getGLFWseconds()
{
    return glfwGetTime();
}
