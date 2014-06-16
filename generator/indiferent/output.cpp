/*
 * This file is model-independent and should be used as is in any generated model
 */

#include "output.h"

#include <iomanip>
#include <limits>
#include <string.h>
#include <sys/stat.h>

OutputInstance::OutputInstance() :
    m_parent(NULL),
    m_space(NULL),
    isFirstTime(true),
    lastOutputTime(0),
    m_currentTime(0)
{
    for (uint i=0; i<MAX_FRACTION_DIMENSION; i++)
        fractionPoint[i] = 0;
    for (uint i=0; i<MAX_SPACE_DIMENSION; i++)
        spacePoint[i] = 0;
}

OutputInstance::~OutputInstance()
{
    if (m_dataFile) {
        m_dataFile->close();
        delete m_dataFile;
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

OutputInstance* OutputInstance::setFractionAndQuantity(uint fractionId, uint quantityId, OutputQuantityType quantityType)
{
    m_fractionId = fractionId;
    m_quantityId = quantityId;
    m_quantityType = quantityType;
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
        IFractionsPool *spaceCell = m_space->getCell_d(spacePoint);
        IFractionSpace *fractionSpace = spaceCell->getFraction(m_fractionId);
        IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
        double result = 0;
        if (m_quantityType == OCT_EXTENSIVE_QUANTITY)
            result = fractionCell->getQuantitiesDensityConvolution(m_quantityId, convolutionAxis) / spaceCell->getVolume();
        else if (m_quantityType == OCT_INTENSIVE_QUANTITY)
            result = fractionCell->getIntensiveQuantity(m_quantityId);
        
        for (uint i=axisIndex-2; i<axisIndex; i++)
        {
            if (axis[i].type == OAT_SPACE_COORDINATE)
                (*m_dataFile) << spacePoint[axis[i].axisIndex] << " ";
            else
                (*m_dataFile) << fractionPoint[axis[i].axisIndex] << " ";
        }
        (*m_dataFile) << result << std::endl;
        return;
    }
    
    if (axisIndex == axis.size()-2)
    {
        // It is time to open file
        std::string fullFilenameWithoutExt = fileName + "-t=" + std::to_string(m_currentTime);
        m_dataFile = new std::ofstream(fullFilenameWithoutExt +".txt");
        (*m_dataFile) << std::setprecision (std::numeric_limits<double>::digits10 + 1);
        createGnuplotFile(fullFilenameWithoutExt);
    }
    else if (axisIndex < axis.size()-2)
    {
        // May be little optimized
        if (axis[axisIndex].type == OAT_SPACE_COORDINATE)
            fileName +=
                "-" + m_space->getAxisDescription(axis[axisIndex].axisIndex)->getId()
                + "=" + std::to_string(spacePoint[axis[axisIndex].axisIndex]);
        else
            fileName += 
                "-" + m_space->getCell_d(spacePoint)->getFraction(m_fractionId)->getAxisDescription(axis[axisIndex].axisIndex)->getId()
                + "=" + std::to_string(spacePoint[axis[axisIndex].axisIndex]);
    }
    
    // Iterating here
    switch(axis[axisIndex].type)
    {
        case OAT_SPACE_COORDINATE: {
            double maxVal = m_space->getAxisDescription(axis[axisIndex].axisIndex)->getMaxValue();
            double minVal = m_space->getAxisDescription(axis[axisIndex].axisIndex)->getMinValue();
            for (uint i=0; i<axis[axisIndex].pointsCount; i++)
            {
                spacePoint[axis[axisIndex].axisIndex] = minVal + (maxVal-minVal) * i / (axis[axisIndex].pointsCount-1);
                recursiveIterate(axisIndex+1, fileName);
            }
        } break;
        case OAT_FRACTION_COORDINATE: {
            IFractionsPool *spaceCell = m_space->getCell_d(spacePoint);
            IFractionSpace *fractionSpace = spaceCell->getFraction(m_fractionId);
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
        (*m_dataFile) << std::endl;
    
    else if (axisIndex == axis.size()-2)
    {
        // It is time to close file
        m_dataFile->close();
        delete m_dataFile;
        m_dataFile = NULL;
    }
}

void OutputInstance::quantityVsAxisAndTimeIterate()
{
    if (isFirstTime) {
        m_dataFile = new std::ofstream(m_filenamePrefix+".txt");
        (*m_dataFile) << std::setprecision (std::numeric_limits<double>::digits10 + 1);
        createGnuplotFile(m_filenamePrefix);
        
    }
    if (not isFirstTime)
        (*m_dataFile) << std::endl;
    
    switch (axis[0].type)
    {
        case OAT_SPACE_COORDINATE: {
            ISpace *space = m_parent->m_space;
            double maxVal = m_space->getAxisDescription(axis[0].axisIndex)->getMaxValue();
            double minVal = m_space->getAxisDescription(axis[0].axisIndex)->getMinValue();
            for (unsigned int pointNumber=0; pointNumber<axis[0].pointsCount; pointNumber++)
            {
                spacePoint[axis[0].axisIndex] = minVal + (maxVal-minVal) * pointNumber / (axis[0].pointsCount-1);
                IFractionsPool *spaceCell = space->getCell_d(spacePoint);
                IFractionSpace *fractionSpace = spaceCell->getFraction(m_fractionId);
                IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
                double result = 0;
                if (m_quantityType == OCT_EXTENSIVE_QUANTITY)
                    result = fractionCell->getQuantitiesDensityConvolution(m_quantityId, convolutionAxis) / spaceCell->getVolume();
                else if (m_quantityType == OCT_INTENSIVE_QUANTITY)
                    result = fractionCell->getIntensiveQuantity(m_quantityId);
                (*m_dataFile) << m_currentTime << " " << spacePoint[axis[0].axisIndex] << " "<< result << std::endl;
            }
        } break;
        case OAT_FRACTION_COORDINATE: {
            ISpace *space = m_parent->m_space;
            IFractionsPool *spaceCell = space->getCell_d(spacePoint);
            IFractionSpace *fractionSpace = spaceCell->getFraction(m_fractionId);
            double maxVal = fractionSpace->getAxisDescription(axis[0].axisIndex)->getMaxValue();
            double minVal = fractionSpace->getAxisDescription(axis[0].axisIndex)->getMinValue();
            
            for (unsigned int pointNumber=0; pointNumber<axis[0].pointsCount; pointNumber++)
            {
                fractionPoint[axis[0].axisIndex] = minVal + (maxVal-minVal) * pointNumber / (axis[0].pointsCount-1);
                IFractionCell *fractionCell = fractionSpace->getCell(fractionPoint);
                double result = 0;
                if (m_quantityType == OCT_EXTENSIVE_QUANTITY)
                    result = fractionCell->getQuantitiesDensityConvolution(m_quantityId, convolutionAxis) / spaceCell->getVolume();
                else if (m_quantityType == OCT_INTENSIVE_QUANTITY)
                    result = fractionCell->getIntensiveQuantity(m_quantityId);
                (*m_dataFile) << m_currentTime << " " << fractionPoint[axis[0].axisIndex] << " "<< result << std::endl;
            }
        } break;
        default: break;
    }
}

void OutputInstance::output(double time)
{
    if ((time < lastOutputTime+m_period) && !isFirstTime)
        return;
    m_currentTime = time;
        
    if (axis.size() == 1)
        /* If axis count is 1, we should plot 3d graph (axis, time, quantity)
         * in other cases time will be in a file name
         */
        quantityVsAxisAndTimeIterate();
    else
        // Output axis count is more than 1
        recursiveIterate(0, m_filenamePrefix);
    
    isFirstTime = false;
    lastOutputTime = time;
}

void OutputInstance::finalise()
{
    
}

void OutputInstance::createGnuplotFile(const std::string& filenamePrefix)
{
    std::string filename = filenamePrefix + ".plt";
    std::ofstream gnuplotFile(filename);
    chmod(filename.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
    gnuplotFile << "#!/usr/bin/gnuplot" << std::endl << std::endl;
    gnuplotFile << "set terminal qt" << std::endl;
    gnuplotFile << "set title \"" << m_space->getCellByIndex(0)->getFraction(m_fractionId)->getFractionDescription()->name << "\"" << std::endl;
    if (axis.size() == 1) {
        gnuplotFile << "set xlabel \"Time, sec\"" << std::endl;
        gnuplotFile << "set ylabel \"" << m_space->getAxisDescription(axis[0].axisIndex)->getName() << "\"" << std::endl;
    } else {
        gnuplotFile << "set xlabel \"" << m_space->getAxisDescription(axis[0].axisIndex)->getName() << "\"" << std::endl;
        gnuplotFile << "set ylabel \"" << m_space->getAxisDescription(axis[1].axisIndex)->getName() << "\"" << std::endl;
    }
    if (m_quantityType == OCT_EXTENSIVE_QUANTITY)
        gnuplotFile << "set zlabel \"" << m_space->getCellByIndex(0)->getFraction(m_fractionId)->getFractionDescription()->extensiveQuantitiesNames[m_quantityId] << "\"";
    else if (m_quantityType == OCT_INTENSIVE_QUANTITY)
        gnuplotFile << "set zlabel \"" << m_space->getCellByIndex(0)->getFraction(m_fractionId)->getFractionDescription()->intensiveQuantitiesNames[m_quantityId] << "\"";
    gnuplotFile << " rotate by 90 offset -2,0" << std::endl;
    gnuplotFile << "splot \"" << filenamePrefix + ".txt" << "\" with pm3d" << std::endl;
    gnuplotFile << "pause -1" << std::endl;
    gnuplotFile.close();
}

//////////////////////
// OutputMaker
OutputMaker::OutputMaker(ISpace* space) :
    m_space(space)
{
}

OutputMaker::~OutputMaker()
{
    for(auto it = m_instances.begin(); it != m_instances.end(); it++)
    {
        (*it)->finalise();
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

void OutputMaker::finalise()
{
    for(auto it = m_instances.begin(); it != m_instances.end(); it++)
        (*it)->finalise();
}
