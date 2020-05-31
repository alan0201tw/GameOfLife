/**
 *  A Conway's Game of Life program for fun.
 *  Target features :
 *      * Cell inserting by mouse clicking
 *      * Status initializeing by reading textures
 * 
 */


#include <iostream>

#include "render.hpp"
#include "simulation.hpp"

int main(int argc, char* argv[])
{
    using namespace gol::gfx;
    using namespace gol::sim;

    const size_t simulationSpaceX = 64u;
    const size_t simulationSpaceY = 64u;

    GOLSimulator simulator(simulationSpaceX, simulationSpaceY);

    GFXUtility::Init();
    // insert per-frame update method here
    GFXUtility::onRender = [&](){
        GFXUtility::RenderSimulator(simulator);
    };
    int frameCounter = 0;
    GFXUtility::onUpdate = [&](){
        ++frameCounter;
        // update every multiple frames
        // this is a quick-hack for lowering frame-rat
        // a more formal implementation will do time comparison between
        // every two frames and wait for an interval of time base on
        // how much time is used during the rendering of last frame
        if(frameCounter >= 60)
        {
            simulator.Update();
            frameCounter = 0;
        }
    };

    float lastPosX = 0.0f, lastPosY = 0.0f;
    bool isMouseJustReleased = true;

    GFXUtility::onMouseDrag = [&](float x, float y){
        // std::cout << "cursor Pos x = " << x << ", y = " << y << "\n";
        simulator.SetRunning(false);
        // interpolate cursor position

        // transform x, y from screen space to simulation space
        x *= (float)simulationSpaceX / GFXUtility::GetScreenWidth();
        y *= (float)simulationSpaceY / GFXUtility::GetScreenHeight();

        if(isMouseJustReleased == true)
        {
            lastPosX = x;
            lastPosY = y;
            isMouseJustReleased = false;
        }
        
        {
            // this way the point might become [x0, y1], which is incorrect
            // int x0 = std::min(lastPosX, x);
            // int x1 = std::max(lastPosX, x);
            // int y0 = std::min(lastPosY, y);
            // int y1 = std::max(lastPosY, y);
            int x0 = lastPosX;
            int x1 = x;
            int y0 = lastPosY;
            int y1 = y;

            // Bresenham Algorithm
            // Ref : https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

            int dx = std::abs(x1 - x0);
            int sx = x0 < x1 ? 1 : -1;
            int dy = -1 * std::abs(y1 - y0);
            int sy = y0 < y1 ? 1 : -1;
            int err = dx + dy;  /* error value e_xy */
            while(true)
            {
                simulator.SetAlive(x0, y0);
                if(x0 == x1 && y0 == y1) break;
                int e2 = 2 * err;
                if(e2 >= dy)
                {
                    err += dy;
                    x0 += sx;
                }
                if(e2 <= dx)
                {
                    err += dx;
                    y0 += sy;
                }
            }
        }

        lastPosX = x;
        lastPosY = y;
    };

    GFXUtility::onMouseRelease = [&](){
        simulator.SetRunning(true);
        isMouseJustReleased = true;
    };

    GFXUtility::MainLoop();
    GFXUtility::CleanUp();
}