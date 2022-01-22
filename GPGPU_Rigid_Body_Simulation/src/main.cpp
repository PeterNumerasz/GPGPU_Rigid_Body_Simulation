#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void) {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Rigid Body Simulation", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  // It is needed here, because we need a valid OpenGL rendering context to
  // initialize GLEW
  if (glewInit() != GLEW_OK) {
    std::cout << "Something went wrong !" << std::endl;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;
  {
    float positions[] = {
        100.0f, 100.0f, /*position*/ 0.0f, 0.0f, /*texture position*/  // 0 - bottom left
        200.0f, 100.0f, /*position*/ 1.0f, 0.0f, /*texture position*/  // 1 - bottom right
        200.0f, 200.0f, /*position*/ 1.0f, 1.0f, /*texture position*/  // 2 - top right
        100.0f, 200.0f, /*position*/ 0.0f, 1.0f, /*texture position*/  // 3 - top left
    };

    unsigned int indicies[] = {
        0, 1, 2, 
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indicies, 6);

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

    // Multiplication in reverse order because OpenGL is column-major with matrices
    glm::mat4 mvp = proj * view * model;

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);

    Texture texture("res/textures/tree.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    shader.SetUniformMat4f("u_MVP", mvp);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    /* Loop until the user closes the window */
    float g = 1.0f;
    float increment = -0.005f;
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      renderer.Clear();

      shader.Bind();
      shader.SetUniform4f("u_Color", 0.0f, g, 0.0f, 1.0f);

      renderer.Draw(va, ib, shader);

      if (g > 1.0f)
        increment = -0.005f;
      else if (g < 0.0f)
        increment = 0.005f;

      g += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }
  glfwTerminate();
  return 0;
}