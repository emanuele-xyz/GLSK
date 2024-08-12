#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_W 1280
#define WINDOW_H 720
#define WINDOW_TITLE "Open GL Starter Kit"

static void glsk_glfw_error_callback(int error_code, const char* description)
{
    printf("[GLFW]: (%d) %s\n", error_code, description);
}

static void glsk_glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwSetErrorCallback(glsk_glfw_error_callback);

    bool did_glfwInit_succeed = glfwInit();
    if (!did_glfwInit_succeed)
    {
        puts("[GLSK]: failed to initialize GLFW");
        goto exit;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_W, WINDOW_H, WINDOW_TITLE, NULL, NULL);
    if (!window) 
    {
        puts("[GLSK]: failed to create GLFW window");
        goto exit;
    }

    glfwMakeContextCurrent(window);

    bool did_gladLoadGLLoader_succeed = gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));
    if (!did_gladLoadGLLoader_succeed)
    {
        puts("[GLSK]: failed to initialize GLAD");
        goto exit;
    }

    glViewport(0, 0, WINDOW_W, WINDOW_H);
    glfwSetFramebufferSizeCallback(window, glsk_glfw_framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

exit:
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}