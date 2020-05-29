#pragma once

#include <vector>

namespace gol { namespace sim {

class GOLSimulator final
{
private:
    std::vector< std::vector<int> > m_cellStates;
    bool m_isRunning;

public:
    GOLSimulator(size_t width, size_t height);
    void Update();
    void SetAlive(size_t x, size_t y);
    
    inline void SetRunning(bool isRunning) { m_isRunning = isRunning; };

    inline const std::vector<std::vector<int>>& GetState() const { return m_cellStates; }
};

}}