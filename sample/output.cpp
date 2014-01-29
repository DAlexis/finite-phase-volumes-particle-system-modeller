/*
 * This file is model-independent and should be used as is in any generated model
 */

#include "output.h"

#include <iomanip>
#include <limits>
#include <string.h>

OutputInstance::OutputInstance() :
    hasParticleAxis(false),
    hasSpaceAxis(false),
    hasTimeAxis(false),
    enabledAxisCount(0),
    m_parent(NULL),
    isFirstTime(true),
    lastOutputTime(0),
    mode(OM_NOT_DEFINED)
{
    
}

OutputInstance::~OutputInstance()
{
    if (m_file) {
        m_file->close();
        delete m_file;
    }
}

OutputInstance* OutputInstance::configAxis(int axisNumber,
    OutputAxisType type,
    unsigned int pointsCount,
    unsigned int axisIndex)
{
    axis[axisNumber].type = type;
    axis[axisNumber].pointsCount = pointsCount;
    axis[axisNumber].axisIndex = axisIndex;
    switch (type)
    {
        case OAT_SPACE_COORDINATE:
            hasSpaceAxis = true;
        break;
        case OAT_FRACTION_COORDINATE:
            hasParticleAxis = true;
        break;
        default: break;
    }
    enabledAxisCount = std::max(enabledAxisCount, axisNumber+1);
    return this;
}

void OutputInstance::setParent(OutputMaker* parent)
{
    m_parent = parent;
}

OutputInstance* OutputInstance::setPeriod(double period)
{
    m_period = period;
    return this;
}

OutputInstance* OutputInstance::setFractionAndQuantity(uint fractionId, uint quantityId)
{
    m_fractionId = fractionId;
    m_quantityId = quantityId;
    return this;
}

OutputInstance* OutputInstance::setFilenamePrefix(const std::string& filenamePrefix)
{
    m_filenamePrefix = filenamePrefix;
    return this;
}

void OutputInstance::output(double time)
{
    if ((time < lastOutputTime+m_period) && !isFirstTime)
        return;
    switch(enabledAxisCount)
    {
        case 1: {
            // X=axis, Y=time, Z=quantity
            if (isFirstTime) {
                m_file = new std::ofstream(m_filenamePrefix+".txt");
                (*m_file) << std::setprecision (std::numeric_limits<double>::digits10 + 1);
            }
            if (not isFirstTime)
                (*m_file) << std::endl;
            
            switch (axis[0].type)
            {
                case OAT_SPACE_COORDINATE: {
                    Space& space = *(m_parent->m_space);
                    double maxVal = space.gridDescription->axis[axis[0].axisIndex].getMaxValue();
                    double minVal = space.gridDescription->axis[axis[0].axisIndex].getMinValue();
                    for (unsigned int pointNumber=0; pointNumber<axis[0].pointsCount; pointNumber++)
                    {
                        spacePoint[axis[0].axisIndex] = minVal + (maxVal-minVal) * pointNumber / axis[0].pointsCount;
                        FractionsPool *spaceCell = space.accessElement_d(spacePoint);
                        IFractionSpace *fractionSpace = spaceCell->fractions[m_fractionId];
                        double result = fractionSpace->getQuantitiesSum(m_quantityId);
                        result /= spaceCell->volume;
                        (*m_file) << time << " " << spacePoint[axis[0].axisIndex] << " "<< result << std::endl;
                    }
                } break;
                case OAT_FRACTION_COORDINATE: {
                    Space& space = *(m_parent->m_space);
                    IFractionSpace *fractionSpace = space.accessElement_d(spacePoint)->fractions[m_fractionId];
                    double maxVal = fractionSpace->getAxisDescription(axis[0].axisIndex)->getMaxValue();
                    double minVal = fractionSpace->getAxisDescription(axis[0].axisIndex)->getMinValue();
                    
                    for (unsigned int pointNumber=0; pointNumber<axis[0].pointsCount; pointNumber++)
                    {
                        fractionPoint[axis[0].axisIndex] = minVal + (maxVal-minVal) * pointNumber / axis[0].pointsCount;
                        IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
                        double result = fractionCell->getQuantitiesDensity(m_quantityId);
                        (*m_file) << time << " " << fractionPoint[axis[0].axisIndex] << " "<< result << std::endl;
                    }
                } break;
                default: break;
            }
            lastOutputTime = time;
            
            if (isFirstTime) isFirstTime = false;
        } break;
        case 2: {
            
        } break;
    }
    /*
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
        m_isFirst = false;*/
}

void continiousOutput(double time)
{
}

void regularOutput(double time)
{
}
/*
//////////////////////
// OutputInstanceBase
OutputInstanceBase::OutputInstanceBase() :
    m_isFirstTime(true),
    m_lastOutputTime(0),
    m_timePeriod(1e100)
{
}

void OutputInstanceBase::setParent(OutputMaker* parent)
{
    m_parent = parent;
}

void OutputInstanceBase::setPeriod(double timePeriod)
{
    m_timePeriod = timePeriod;
}

//////////////////////
// RegularOutputInstance
RegularOutputInstance::RegularOutputInstance(const std::string& fileNamePrefix) :
    m_fileNamePrefix(fileNamePrefix)
{
    for (unsigned int i=0; i<SPACE_COORDS_COUNT; i++)
        m_point[i] = 0;
}

void RegularOutputInstance::configure(int fractionIndex, RegularOutputDimension mode)
{
    m_mode = mode;
    m_fractionIndex = fractionIndex;
}

void RegularOutputInstance::setAxis(int first, int second)
{
    m_firstAxis = first;
    m_secondAxis = second;
}

double* RegularOutputInstance::point()
{
    return m_point;
}

void RegularOutputInstance::output(double time)
{
    switch(m_mode)
    {
        case ROI_2D:
            
        break;
        case ROI_3D:
            
        break;
    }
}
*/
/*
//////////////////////
// RegularOutputInstance
RegularOutputInstance::RegularOutputInstance(int quantity, double period, unsigned int pointsCount, const std::string& fileName) :
    OutputInstanceBase(fileName),
    m_period(period),
    m_pointsCount(pointsCount),
    m_lastOutput(0),
    m_isFirst(true)
{
    
}

RegularOutputInstance::~RegularOutputInstance()
{
    if (m_file) {
        m_file->close();
        delete m_file;
    }
}

void RegularOutputInstance::output(double time)
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
*/
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

void OutputMaker::addInstance(OutputInstance* instance)
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
