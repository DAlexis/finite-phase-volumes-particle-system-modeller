#include "model-output.h"
#include <iostream>

Fraction1ConcentrationOutput::Fraction1ConcentrationOutput() :
    OutputInstanceBase(FRACTION1_QUANTITY_CONCENTRATION,
        0.01,
        100,
        "Fraction1-concentration-x-slice-(t).txt")
{
}

Fraction1ConcentrationOutput::~Fraction1ConcentrationOutput()
{
}

void Fraction1ConcentrationOutput::printToFile(double time)
{
    (*m_file) << std::endl;
    
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
