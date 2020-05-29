#include "simulation.hpp"

#include <iostream>

#include <exception>

namespace gol { namespace sim {

GOLSimulator::GOLSimulator(size_t width, size_t height)
    : m_isRunning(true)
{
    m_cellStates.resize(height);
    for(size_t i = 0; i < m_cellStates.size(); ++i)
    {
        m_cellStates[i].resize(width);
    }

    // TEST
    for(size_t i = 0; i < 53; ++i)
    {
        m_cellStates[i][50] = 1;
    }

    for(size_t i = 0; i < 53; ++i)
    {
        m_cellStates[i][60] = 1;
    }

    m_cellStates[100][102] = 1;
    m_cellStates[101][100] = 1;
    m_cellStates[101][102] = 1;
    m_cellStates[102][101] = 1;
    m_cellStates[102][102] = 1;
}

void GOLSimulator::Update()
{
    if(m_isRunning == false)
        return;

    auto nextState = m_cellStates;

    for(size_t i = 0; i < m_cellStates.size(); ++i)
    {
        for(size_t j = 0; j < m_cellStates[i].size(); ++j)
        {
            // count the number of living cells near this cell
            int nearLivingCellCount = 0;

            for(size_t i1 = i-1; i1 < m_cellStates.size() && i1 <= i+1; ++i1)
            {
                for(size_t j1 = j-1; j1 < m_cellStates[i1].size() && j1 <= j+1; ++j1)
                {
                    // if is alive and not the center cell
                    if((i1 != i || j1 != j) && m_cellStates[i1][j1] > 0)
                    {
                        ++nearLivingCellCount;
                    }
                }
            }

            // if is alive
            if(m_cellStates[i][j] > 0)
            {
                if(nearLivingCellCount < 2 || nearLivingCellCount > 3)
                    nextState[i][j] = 0;
            }
            else
            {
                if(nearLivingCellCount == 3)
                    nextState[i][j] = 1;
            }
        }
    }

    m_cellStates = std::move(nextState);
}

void GOLSimulator::SetAlive(size_t x, size_t y)
{
    if(m_cellStates.size() < x || 0 > x)
        throw new std::runtime_error("GOLSimulator::SetAlive : x range error");
    if(m_cellStates[x].size() < y || 0 > y)
        throw new std::runtime_error("GOLSimulator::SetAlive : y range error");

    m_cellStates[x][y] = 1;
}

}}