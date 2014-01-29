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
    /// May be I should use recursive algorythm here
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
                        double result = fractionSpace->getQuantitiesSum(m_quantityId) / spaceCell->volume;
                        (*m_file) << time << " " << spacePoint[axis[0].axisIndex] << " "<< result << std::endl;
                    }
                } break;
                case OAT_FRACTION_COORDINATE: {
                    Space& space = *(m_parent->m_space);
                    FractionsPool *spaceCell = space.accessElement_d(spacePoint);
                    IFractionSpace *fractionSpace = spaceCell->fractions[m_fractionId];
                    double maxVal = fractionSpace->getAxisDescription(axis[0].axisIndex)->getMaxValue();
                    double minVal = fractionSpace->getAxisDescription(axis[0].axisIndex)->getMinValue();
                    
                    for (unsigned int pointNumber=0; pointNumber<axis[0].pointsCount; pointNumber++)
                    {
                        fractionPoint[axis[0].axisIndex] = minVal + (maxVal-minVal) * pointNumber / axis[0].pointsCount;
                        IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
                        double result = fractionCell->getQuantitiesDensity(m_quantityId) / spaceCell->volume;
                        (*m_file) << time << " " << fractionPoint[axis[0].axisIndex] << " "<< result << std::endl;
                    }
                } break;
                default: break;
            }
            
        } break;
        case 2: {
            m_file = new std::ofstream(m_filenamePrefix + "-t=" + std::to_string(time) +".txt");
            (*m_file) << std::setprecision (std::numeric_limits<double>::digits10 + 1);
            // If both are coordinate
            if (axis[0].type == OAT_SPACE_COORDINATE && axis[1].type == OAT_SPACE_COORDINATE)
            {
                Space& space = *(m_parent->m_space);
                double maxValX = space.gridDescription->axis[axis[0].axisIndex].getMaxValue();
                double minValX = space.gridDescription->axis[axis[0].axisIndex].getMinValue();
                
                double maxValY = space.gridDescription->axis[axis[1].axisIndex].getMaxValue();
                double minValY = space.gridDescription->axis[axis[1].axisIndex].getMinValue();
                
                for (unsigned int pointNumberX=0; pointNumberX<axis[0].pointsCount; pointNumberX++)
                {
                    for (unsigned int pointNumberY=0; pointNumberY<axis[1].pointsCount; pointNumberY++)
                    {
                        spacePoint[axis[0].axisIndex] = minValX + (maxValX-minValX) * pointNumberX / axis[0].pointsCount;
                        spacePoint[axis[1].axisIndex] = minValY + (maxValY-minValY) * pointNumberY / axis[1].pointsCount;
                        FractionsPool *spaceCell = space.accessElement_d(spacePoint);
                        IFractionSpace *fractionSpace = spaceCell->fractions[m_fractionId];
                        double result = fractionSpace->getQuantitiesSum(m_quantityId) / spaceCell->volume;
                        (*m_file) << spacePoint[axis[0].axisIndex] << " "
                                  << spacePoint[axis[1].axisIndex] << " "
                                  << result << std::endl;
                    }
                    (*m_file) << std::endl;
                }
            }
            
            if (axis[0].type == OAT_FRACTION_COORDINATE && axis[1].type == OAT_SPACE_COORDINATE)
                std::swap(axis[0], axis[1]);
            
            if (axis[0].type == OAT_SPACE_COORDINATE && axis[1].type == OAT_FRACTION_COORDINATE)
            {
                Space& space = *(m_parent->m_space);
                double maxValX = space.gridDescription->axis[axis[0].axisIndex].getMaxValue();
                double minValX = space.gridDescription->axis[axis[0].axisIndex].getMinValue();
                
                for (unsigned int pointNumberX=0; pointNumberX<axis[0].pointsCount; pointNumberX++)
                {
                    spacePoint[axis[0].axisIndex] = minValX + (maxValX-minValX) * pointNumberX / axis[0].pointsCount;
                    FractionsPool *spaceCell = space.accessElement_d(spacePoint);
                    IFractionSpace *fractionSpace = spaceCell->fractions[m_fractionId];
                    
                    double maxValY = fractionSpace->getAxisDescription(axis[1].axisIndex)->getMaxValue();
                    double minValY = fractionSpace->getAxisDescription(axis[1].axisIndex)->getMinValue();
                
                    for (unsigned int pointNumberY=0; pointNumberY<axis[1].pointsCount; pointNumberY++)
                    {
                        fractionPoint[axis[1].axisIndex] = minValY + (maxValY-minValY) * pointNumberY / axis[1].pointsCount;
                        IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
                        double result = fractionCell->getQuantitiesDensity(m_quantityId) / spaceCell->volume;
                        (*m_file) << spacePoint[axis[0].axisIndex] << " "
                                  << fractionPoint[axis[1].axisIndex] << " "
                                  << result << std::endl;
                    }
                    (*m_file) << std::endl;
                }
            }
            
            if (axis[0].type == OAT_FRACTION_COORDINATE && axis[1].type == OAT_FRACTION_COORDINATE)
            {
                Space& space = *(m_parent->m_space);
                FractionsPool *spaceCell = space.accessElement_d(spacePoint);
                IFractionSpace *fractionSpace = spaceCell->fractions[m_fractionId];
                double maxValX = fractionSpace->getAxisDescription(axis[0].axisIndex)->getMaxValue();
                double minValX = fractionSpace->getAxisDescription(axis[0].axisIndex)->getMinValue();
                
                double maxValY = fractionSpace->getAxisDescription(axis[1].axisIndex)->getMaxValue();
                double minValY = fractionSpace->getAxisDescription(axis[1].axisIndex)->getMinValue();
                
                for (unsigned int pointNumberX=0; pointNumberX<axis[0].pointsCount; pointNumberX++)
                {
                    for (unsigned int pointNumberY=0; pointNumberY<axis[1].pointsCount; pointNumberY++)
                    {
                        fractionPoint[axis[0].axisIndex] = minValX + (maxValX-minValX) * pointNumberX / axis[0].pointsCount;
                        fractionPoint[axis[1].axisIndex] = minValY + (maxValY-minValY) * pointNumberY / axis[1].pointsCount;
                        
                        IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
                        double result = fractionCell->getQuantitiesDensity(m_quantityId) / spaceCell->volume;
                        (*m_file) << fractionPoint[axis[0].axisIndex] << " "
                                  << fractionPoint[axis[1].axisIndex] << " "
                                  << result << std::endl;
                    }
                    (*m_file) << std::endl;
                }
            }
            
            m_file->close();
            delete m_file;
            m_file = NULL;
        } break;
    }
    lastOutputTime = time;
    isFirstTime = false;
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
