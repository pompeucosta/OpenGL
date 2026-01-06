#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include <chrono>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_opengl3.h"
#include "external/imgui/imgui_impl_glfw.h"

#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "texture.hpp"

static void APIENTRY GLDebugMsgCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message, const void* userParam) {
    //adapted from https://gist.github.com/Challanger524/cdf90cf11809749363fb638646225773
    const char *source_, *type_, *severity_;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             source_ = "API";             break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_ = "WINDOW_SYSTEM";   break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: source_ = "SHADER_COMPILER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     source_ = "THIRD_PARTY";     break;
	case GL_DEBUG_SOURCE_APPLICATION:     source_ = "APPLICATION";     break;
	case GL_DEBUG_SOURCE_OTHER:           source_ = "OTHER";           break;
	default:                              source_ = "UNKNOWN";        break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               type_ = "ERROR";               break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_ = "DEPRECATED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_ = "UDEFINED_BEHAVIOR";   break;
	case GL_DEBUG_TYPE_PORTABILITY:         type_ = "PORTABILITY";         break;
	case GL_DEBUG_TYPE_PERFORMANCE:         type_ = "PERFORMANCE";         break;
	case GL_DEBUG_TYPE_OTHER:               type_ = "OTHER";               break;
	case GL_DEBUG_TYPE_MARKER:              type_ = "MARKER";              break;
	default:                                type_ = "UNKNOWN";              break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         severity_ = "HIGH";         break;
	case GL_DEBUG_SEVERITY_MEDIUM:       severity_ = "MEDIUM";       break;
	case GL_DEBUG_SEVERITY_LOW:          severity_ = "LOW";          break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: severity_ = "NOTIFICATION"; break;
	default:                             severity_ = "UNKNOWN";   break;
	}

    std::cout << std::format("[OpenGL {}({})][{}] {}\n",type_,severity_,source_,message);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    // glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum err = glewInit();
    // if(err != GLEW_OK) {
    //     std::cout << glewGetErrorString(err) << std::endl;
    //     return -1;
    // }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugMsgCallback,nullptr);

    float positions[] = {
        100.0f,100.0f, 0.0f, 0.0f,
        400.0f,100.0f, 1.0f, 0.0f,
        400.0f,400.0f, 1.0f, 1.0f,
        100.0f,400.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    VertexArray va;
    VertexBuffer vb(positions,16 * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.addBuffer(vb,layout);

    IndexBuffer ib(indices,6);

    glm::mat4 proj = glm::ortho(0.0f,960.0f,0.0f,540.0f,-1.0f,1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(-100,0,0));

    Shader shader("../res/shaders/Basic.shader");
    shader.bind();

    Texture texture("../res/textures/omega.jpg");
    texture.bind();
    shader.setUniform1i("u_Texture",0);

    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();
    Renderer renderer;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    glm::vec3 translation(200,200,0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 model = glm::translate(glm::mat4(1.0f),translation);
        glm::mat4 mvp = proj * view * model;

        shader.bind();
        shader.setUniformMat4f("u_MVP",mvp);

        renderer.draw(va,ib,shader);

        {
            ImGui::SliderFloat3("float", &translation.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}