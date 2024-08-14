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

static void APIENTRY glsk_gl_debug_msg_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    const char* default_str = "UNKNOWN";

    // TODO: to be implemented
    const char* source_str = default_str;
    switch (source)
    {
    case GL_DEBUG_SOURCE_API: { source_str = "API"; } break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: { source_str = "WINDOW_SYSTEM"; } break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: { source_str = "SHADER_COMPILER"; } break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: { source_str = "THIRD_PARTY"; } break;
    case GL_DEBUG_SOURCE_APPLICATION: { source_str = "APPLICATION"; } break;
    case GL_DEBUG_SOURCE_OTHER: { source_str = "OTHER"; } break;
    }

    const char* type_str = default_str;
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: { type_str = "ERROR"; } break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: { type_str = "DEPRECATED_BEHAVIOR"; } break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: { type_str = "UNDEFINED_BEHAVIOR"; } break;
    case GL_DEBUG_TYPE_PORTABILITY: { type_str = "PORTABILITY"; } break;
    case GL_DEBUG_TYPE_PERFORMANCE: { type_str = "PERFORMANCE"; } break;
    case GL_DEBUG_TYPE_MARKER: { type_str = "MARKER"; } break;
    case GL_DEBUG_TYPE_PUSH_GROUP: { type_str = "PUSH_GROUP"; } break;
    case GL_DEBUG_TYPE_POP_GROUP: { type_str = "POP_GROUP"; } break;
    case GL_DEBUG_TYPE_OTHER: { type_str = "OTHER"; } break;
    }

    const char* severity_str = default_str;
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: { severity_str = "HIGH"; } break;
    case GL_DEBUG_SEVERITY_MEDIUM: { severity_str = "MEDIUM"; } break;
    case GL_DEBUG_SEVERITY_LOW: { severity_str = "LOW"; } break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: { severity_str = "NOTIFICATION"; } break;
    }

    printf("[GL][SRC:%s][TYPE:%s][LEVEL:%s]: (%u) %s\n", source_str, type_str, severity_str, id, message);
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
    #if defined(GLSK_DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #endif
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

    #if defined(GLSK_DEBUG)
    // NOTE: check for opengl debug context
    {
        int flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glsk_gl_debug_msg_callback, NULL);
        }
        else
        {
            puts("[GLSK]: failed to create opengl debug context");
        }
    }
    #endif

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // NOTE: render
        {
            int window_w = 0;
            int window_h = 0;
            glfwGetWindowSize(window, &window_w, &window_h);
            glViewport(0, 0, window_w, window_h);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        glfwSwapBuffers(window);
    }

exit:
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}