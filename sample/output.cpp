/*
 * This file is model-independent and should be used as is in any generated model
 */

#include "output.h"

#include <iomanip>
#include <limits>
#include <string.h>

OutputInstance::OutputInstance() :
    m_parent(NULL),
    m_space(NULL),
    isFirstTime(true),
    lastOutputTime(0),
    m_currentTime(0)
{
    
}

OutputInstance::~OutputInstance()
{
    if (m_file) {
        m_file->close();
        delete m_file;
    }
}

OutputInstance* OutputInstance::addOutputAxis(
    OutputAxisType type,
    unsigned int pointsCount,
    unsigned int axisIndex)
{
    axis.push_back(OutputAxisDescription());
    axis.back().type = type;
    axis.back().pointsCount = pointsCount;
    axis.back().axisIndex = axisIndex;
    return this;
}

OutputInstance* OutputInstance::useConvolutionByFractionAxis(unsigned int axisIndex)
{
    convolutionAxis.push_back(axisIndex);
    return this;
}

OutputInstance* OutputInstance::useAllFractionSpaceConvolution(unsigned int fractionSpaceDimension)
{
    for (uint i=0; i<fractionSpaceDimension; i++)
        useConvolutionByFractionAxis(i);
    return this;
}

void OutputInstance::setParent(OutputMaker* parent)
{
    m_parent = parent;
    m_space = m_parent->m_space;
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

void OutputInstance::recursiveIterate(uint axisIndex, std::string fileName)
{
    if (axisIndex == axis.size())
    {
        FractionsPool *spaceCell = m_space->accessElement_d(spacePoint);
        IFractionSpace *fractionSpace = spaceCell->fractions[m_fractionId];
        IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
        double result = fractionCell->getQuantitiesDensityConvolution(m_quantityId, convolutionAxis) / spaceCell->volume;
        for (uint i=axisIndex-2; i<axisIndex; i++)
        {
            if (axis[i].type == OAT_SPACE_COORDINATE)
                (*m_file) << spacePoint[axis[i].axisIndex] << " ";
            else
                (*m_file) <<  fractionPoint[axis[i].axisIndex] << " ";
        }
        (*m_file) << result << std::endl;
        return;
    }
    
    if (axisIndex == axis.size()-2)
    {
        // It is time to open file
        m_file = new std::ofstream(fileName + "-t=" + std::to_string(m_currentTime) +".txt");
        (*m_file) << std::setprecision (std::numeric_limits<double>::digits10 + 1);
    }
    else if (axisIndex < axis.size()-2)
    {
        // May be little optimized
        if (axis[axisIndex].type == OAT_SPACE_COORDINATE)
            fileName +=
                "-" + m_space->gridDescription->axis[axis[axisIndex].axisIndex].getId()
                + "=" + std::to_string(spacePoint[axis[axisIndex].axisIndex]);
        else
            fileName += 
                "-" + m_space->accessElement_d(spacePoint)->fractions[m_fractionId]->getAxisDescription(axis[axisIndex].axisIndex)->getId()
                + "=" + std::to_string(spacePoint[axis[axisIndex].axisIndex]);
    }
    
    // Iterating here
    switch(axis[axisIndex].type)
    {
        case OAT_SPACE_COORDINATE: {
            double maxVal = m_space->gridDescription->axis[axis[axisIndex].axisIndex].getMaxValue();
            double minVal = m_space->gridDescription->axis[axis[axisIndex].axisIndex].getMinValue();
            for (uint i=0; i<axis[axisIndex].pointsCount; i++)
            {
                spacePoint[axis[axisIndex].axisIndex] = minVal + (maxVal-minVal) * i / (axis[axisIndex].pointsCount-1);
                recursiveIterate(axisIndex+1, fileName);
            }
        } break;
        case OAT_FRACTION_COORDINATE: {
            FractionsPool *spaceCell = m_space->accessElement_d(spacePoint);
            IFractionSpace *fractionSpace = spaceCell->fractions[m_fractionId];
            double maxVal = fractionSpace->getAxisDescription(axis[axisIndex].axisIndex)->getMaxValue();
            double minVal = fractionSpace->getAxisDescription(axis[axisIndex].axisIndex)->getMinValue();
            for (uint i=0; i<axis[axisIndex].pointsCount; i++)
            {
                fractionPoint[axis[axisIndex].axisIndex] = minVal + (maxVal-minVal) * i / (axis[axisIndex].pointsCount-1);
                recursiveIterate(axisIndex+1, fileName);
            }
        } break;
        default: break;
    }
    
    if (axisIndex == axis.size()-1)
        (*m_file) << std::endl;
    
    else if (axisIndex == axis.size()-2)
    {
        // It is time to close file
        m_file->close();
        delete m_file;
        m_file = NULL;
    }
}

void OutputInstance::output(double time)
{
    if ((time < lastOutputTime+m_period) && !isFirstTime)
        return;
    m_currentTime = time;
    
    // When one axis is time
    if (axis.size() == 1)
    {
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
                    spacePoint[axis[0].axisIndex] = minVal + (maxVal-minVal) * pointNumber / (axis[0].pointsCount-1);
                    FractionsPool *spaceCell = space.accessElement_d(spacePoint);
                    IFractionSpace *fractionSpace = spaceCell->fractions[m_fractionId];
                    IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
                    double result = fractionCell->getQuantitiesDensityConvolution(m_quantityId, convolutionAxis) / spaceCell->volume;
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
                    fractionPoint[axis[0].axisIndex] = minVal + (maxVal-minVal) * pointNumber / (axis[0].pointsCount-1);
                    IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
                    double result = fractionCell->getQuantitiesDensityConvolution(m_quantityId, convolutionAxis) / spaceCell->volume;
                    (*m_file) << time << " " << fractionPoint[axis[0].axisIndex] << " "<< result << std::endl;
                }
            } break;
            default: break;
        }
    } else
        recursiveIterate(0, m_filenamePrefix);
    
    isFirstTime = false;
    lastOutputTime = time;
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
