#include <stdbool.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

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

size_t glsk_get_file_size(const char* path)
{
    size_t out = 0;

    FILE* f = fopen(path, "rb");
    if (!f) goto exit;

    bool failed = fseek(f, 0, SEEK_END);
    if (failed) goto exit;

    long size = ftell(f);
    if (size > 0) out = (size_t)(size);
    else goto exit;

exit:
    if (f) fclose(f);
    return out;
}

size_t glsk_load_file(const char* path, size_t buf_size, char* buf)
{
    size_t bytes_read = 0;

    FILE* f = fopen(path, "rb");
    if (!f) goto exit;
    bytes_read = fread(buf, 1, buf_size, f);
    fclose(f);

exit:
    return bytes_read;
}

#define GLSK_LOG_SIZE 1024
static char s_log[GLSK_LOG_SIZE] = { 0 };

GLuint glsk_compile_shader(const char* path, GLenum shader_type, GLsizei count, const GLchar* const* string, const GLint* length)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, count, string, length);
    glCompileShader(shader);

    bool success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, GLSK_LOG_SIZE, NULL, s_log);
        printf("[GLSK][%s]: %s\n", path, s_log);
    }

    return shader;
}

enum
{
    GLSK_SHADER_EXAMPLE,
    GLSK_SHADER_COUNT,
};

#define GLSK_VS_PATH(name) "glsl/" name ".vs"

static const char* s_vs_paths[] =
{
    [GLSK_SHADER_EXAMPLE] = GLSK_VS_PATH("example"),
};

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

    // NOTE: check for opengl debug context
    #if defined(GLSK_DEBUG)
    {
        int flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glsk_gl_debug_msg_callback, NULL);
            // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE); // NOTE: for filtering debug messages
        }
        else
        {
            puts("[GLSK]: failed to create opengl debug context");
        }
    }
    #endif

    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        +0.5f, -0.5f, 0.0f,
        +0.0f, +0.5f, 0.0f,
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // NOTE: compile shader programs
    for (int i = 0; i < GLSK_SHADER_COUNT; i++)
    {
        // NOTE: compile vertex shader
        GLuint vs = 0;
        if (s_vs_paths[i])
        {
            size_t file_size = glsk_get_file_size(s_vs_paths[i]);
            if (file_size > 0)
            {
                char* file_bytes = calloc(file_size, 1);
                if (file_bytes)
                {
                    glsk_load_file(s_vs_paths[i], file_size, file_bytes);
                    int length = (int)(file_size);
                    vs = glsk_compile_shader(s_vs_paths[i], GL_VERTEX_SHADER, 1, &file_bytes, &length);
                    free(file_bytes);
                }
            }
        }

        // TODO: compile fragment shader

        glDeleteShader(vs);
    }

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
    glDeleteBuffers(1, &vbo);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}