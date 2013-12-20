/*
 * This file is model-independent and should be used as is in any generated model
 */

#include "output.h"

#include <iomanip>
#include <limits>
#include <string.h>

//////////////////////
// OutputInstanceBase
OutputInstanceBase::OutputInstanceBase(int quantity, double period, unsigned int pointsCount, const std::string& fileName) :
    m_quantity(quantity),
    m_period(period),
    m_pointsCount(pointsCount),
    m_fileName(fileName),
    m_lastOutput(0),
    m_isFirst(true),
    m_file(0)
{
    
}

OutputInstanceBase::~OutputInstanceBase()
{
    if (m_file) {
        m_file->close();
        delete m_file;
    }
}

void OutputInstanceBase::output(double time)
{
    if (m_isFirst) {
        m_file = new std::ofstream(m_fileName);
        (*m_file) << std::setprecision (std::numeric_limits<double>::digits10 + 1);
    }
    
    if ((time >= m_lastOutput+m_period) || m_isFirst)
    {
        if (not m_isFirst)
            (*m_file) << std::endl;
        
        printToFile(time);
        m_lastOutput = time;
    }
    
    if (m_isFirst)
        m_isFirst = false;
}

void OutputInstanceBase::setParent(OutputMaker* parent)
{
    m_parent = parent;
}

//////////////////////
// OutputMaker
OutputMaker::OutputMaker(Space* space) :
    m_space(space)
{
}

OutputMaker::~OutputMaker()
{
    for(auto it = m_instances.begin(); it != m_instances.end(); it++)
    {
        delete (*it);
    }
}

void OutputMaker::addInstance(OutputInstanceBase* instance)
{
    m_instances.push_back(instance);
    m_instances.back()->setParent(this);
}

void OutputMaker::output(double time)
{
    for(auto it = m_instances.begin(); it != m_instances.end(); it++)
    {
        (*it)->output(time);
    }
}
