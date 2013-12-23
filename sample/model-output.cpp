#include "model-output.h"
#include <iostream>

Fraction1ConcentrationVsCoordsOutput::Fraction1ConcentrationVsCoordsOutput() :
    OutputInstanceBase(FRACTION1_QUANTITY_CONCENTRATION,
        0.01,
        50,
        "Fraction1-concentration-x-slice-(t).txt")
{
}

Fraction1ConcentrationVsCoordsOutput::~Fraction1ConcentrationVsCoordsOutput()
{
}

void Fraction1ConcentrationVsCoordsOutput::printToFile(double time)
{
    Space& space = *(m_parent->m_space);
    double maxVal = space.gridDescription->axis[SPACE_COORDS_X].getMaxValue();
    double minVal = space.gridDescription->axis[SPACE_COORDS_X].getMinValue();
    
    double spacePoint[SPACE_COORDS_COUNT] = { 0 };
    
    for (unsigned int sapceIndex=0; sapceIndex<m_pointsCount; sapceIndex++)
    {
        spacePoint[SPACE_COORDS_X] = minVal + (maxVal-minVal) * sapceIndex / m_pointsCount;
        
        FractionsPool &spaceCell = space.accessElement_d(spacePoint)->data;
        
        // Convolution in fraction1 space cell
        double result = 0;
        for (unsigned int particleIndex=0; particleIndex<spaceCell.fraction1.elementsCount; particleIndex++)
        {
            result += spaceCell.fraction1.elements[particleIndex].data.quantities[FRACTION1_QUANTITY_CONCENTRATION];
        }
        (*m_file) << time << " " << spacePoint[SPACE_COORDS_X] << " " << result << std::endl;
    }
}


/////////////////////////
// Fraction1ConcentrationVsVelocityOutput

Fraction1ConcentrationVsVelocityOutput::Fraction1ConcentrationVsVelocityOutput() :
    OutputInstanceBase(FRACTION1_QUANTITY_CONCENTRATION,
        0.01,
        50,
        "Fraction1-concentration-vx-slice-(t).txt")
{
}

Fraction1ConcentrationVsVelocityOutput::~Fraction1ConcentrationVsVelocityOutput()
{
}

void Fraction1ConcentrationVsVelocityOutput::printToFile(double time)
{
    Space& space = *(m_parent->m_space);
    
    // Here we will get a distribution
    double spacePoint[SPACE_COORDS_COUNT] = { 0 };
    
    Fraction1Space &fractionSpace = space.accessElement_d(spacePoint)->data.fraction1;
    
    double maxVal = fractionSpace.gridDescription->axis[FRACTION1_COORDS_VX].getMaxValue();
    double minVal = fractionSpace.gridDescription->axis[FRACTION1_COORDS_VX].getMinValue();
    
    double fractionSubspacePoint[FRACTION1_COORDS_COUNT] = { 0 };
    
    for (unsigned int fractionSapceIndex=0; fractionSapceIndex<m_pointsCount; fractionSapceIndex++)
    {
        fractionSubspacePoint[FRACTION1_COORDS_VX] = minVal + (maxVal-minVal) * fractionSapceIndex / m_pointsCount;
        
        Fraction1Cell &fractionCell = fractionSpace.accessElement_d(fractionSubspacePoint)->data;
        // Convolution in fraction1 space cell
        double result = fractionCell.quantities[FRACTION1_QUANTITY_CONCENTRATION];
        (*m_file) << time << " " << fractionSubspacePoint[FRACTION1_COORDS_VX] << " " << result << std::endl;
    }
}
