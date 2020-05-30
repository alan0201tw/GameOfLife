#include "render.hpp"

#include <iostream>

namespace gol { namespace gfx {

GLFWwindow* GFXUtility::m_window;
std::function<void()> GFXUtility::onRender;
std::function<void()> GFXUtility::onUpdate;
std::function<void(int, int)> GFXUtility::onMouseDrag;
std::function<void()> GFXUtility::onMouseRelease;

static bool s_isMouseButtonDown = false;

void GFXUtility::ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void RedirectionMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    // this will re-arrange the input arguments and redirect to cleaner callback functions
    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(GLFW_PRESS == action)
            s_isMouseButtonDown = true;
        else if(GLFW_RELEASE == action)
        {
            s_isMouseButtonDown = false;
            if(GFXUtility::onMouseRelease)
                GFXUtility::onMouseRelease();
        }
    }
}

void GFXUtility::Init()
{
    glfwSetErrorCallback(GFXUtility::ErrorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(
        m_screenWidth, m_screenHeight, "ASCIIRenderer", NULL, NULL);
    if (m_window == nullptr)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetMouseButtonCallback(m_window, RedirectionMouseCallback);

    glfwMakeContextCurrent(m_window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
}

void GFXUtility::MainLoop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();

        float ortho_size_x = (float)m_screenWidth;
        float ortho_size_y = (float)m_screenHeight;
    
        // gluOrtho2D sets up a two-dimensional orthographic viewing region.
        // This is equivalent to calling glOrtho with near = -1 and far = 1 .
        glOrtho(
            0, ortho_size_x,
            0, ortho_size_y,
            -1, 1);

        // deal with input mouse dragging
        if(s_isMouseButtonDown == true)
        {
            if(GFXUtility::onMouseDrag)
            {
                double xPos, yPos;
                glfwGetCursorPos(m_window, &xPos, &yPos);

                if(xPos >= m_screenWidth || xPos < 0 || yPos >= m_screenHeight || yPos < 0)
                {
                    std::cout << "Mouse drag out of window!\n";
                }
                else
                {
                    GFXUtility::onMouseDrag((float)xPos, (float)m_screenHeight - (float)yPos);
                }
            }
        }
    
        // callback function
        if(onRender) // use the bool operator of std::function to determine if it is callable
        {
                onRender();
        }
        if(onUpdate)
        {
            if(glfwGetKey(m_window, GLFW_KEY_SPACE) != GLFW_PRESS)
                onUpdate();
        }

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void GFXUtility::CleanUp()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GFXUtility::RenderSimulator(const gol::sim::GOLSimulator& sim)
{
    auto states = sim.GetState();

    // glBegin(GL_POINTS);
    glBegin(GL_QUADS);
    {
        glColor3f(1,1,1);
        
        for(size_t i = 0; i < states.size(); ++i)
        {
            for(size_t j = 0; j < states[i].size(); ++j)
            {
                if(states[i][j] > 0)
                {
                    // transform from simulation space to screen space

                    float x = (float)i * m_screenWidth / states.size();
                    float x1 = (float)(i+1) * m_screenWidth / states.size();
                    float y = (float)j * m_screenHeight / states[i].size();
                    float y1 = (float)(j+1) * m_screenHeight / states[i].size();

                    glVertex2f(x, y);
                    glVertex2f(x, y1);
                    glVertex2f(x1, y1);
                    glVertex2f(x1, y);
                }
            }
        }
    }
    glEnd();
}

}}