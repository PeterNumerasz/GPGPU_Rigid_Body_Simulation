#pragma once

#include <GL\glew.h>

// Macros for basic debugging of OpenGL function calls
// Has some flaws, but does what it needs to do

// This ASSERT(x) breaks on the line of code that causes the error basically
#define ASSERT(x) \
  if (!(x)) __debugbreak();

// This GLCall(x) will call the ASSERT(x) macro on the GLLogCall() function
// and will give back
//  - the function name
//  - the full path to the file
//  - the line of code that has the error
#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// For debugging OpenGL
// Clears all the previous error messages
void GLClearError();

// For debugging OpenGL
// Prints out the current error
bool GLLogCall(const char* function, const char* file, int line);