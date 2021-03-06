#pragma once

// opengl : glad and glfw
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <functional>

#include "simulation.hpp"

namespace gol { namespace gfx {

class GFXUtility final
{
private:
    static constexpr const int m_screenWidth = 640;
    static constexpr const int m_screenHeight = 640;
    static constexpr const float m_screenAspectRatio = 
        (float)m_screenWidth / m_screenHeight;

    static GLFWwindow* m_window;

    static void ErrorCallback(int error, const char* description);

public:
    static void Init();
    static void MainLoop();
    static void CleanUp();

    static void RenderSimulator(const gol::sim::GOLSimulator& sim);

    static inline constexpr const int GetScreenWidth() { return m_screenWidth; }
    static inline constexpr const int GetScreenHeight() { return m_screenHeight; }

    static std::function<void()> onRender;
    static std::function<void()> onUpdate;
    static std::function<void(float, float)> onMouseDrag;
    static std::function<void()> onMouseRelease;
};


}}