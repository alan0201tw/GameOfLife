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

    GOLSimulator simulator(640u, 640u);

    GFXUtility::Init();
    // insert per-frame update method here
    GFXUtility::onRender = [&](){
        GFXUtility::RenderSimulator(simulator);
        simulator.Update();
    };

    float lastPosX, lastPosY;
    bool isMouseJustReleased = true;

    GFXUtility::onMouseDrag = [&](float x, float y){
        // std::cout << "cursor Pos x = " << x << ", y = " << y << "\n";
        simulator.SetRunning(false);
        // interpolate cursor position

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
            // simulator.SetAlive((size_t)tx, (size_t)ty);
        }

        // simulator.SetAlive((size_t)x, (size_t)y);

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