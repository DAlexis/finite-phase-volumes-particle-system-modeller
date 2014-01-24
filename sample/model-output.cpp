#include "model-output.h"
#include <iostream>
/*
Fraction1ConcentrationVsCoordsOutput::Fraction1ConcentrationVsCoordsOutput() :
    OutputInstanceBase(FRACTION1_QUANTITIES_PARTICLES_COUNT,
        0.01,
        100,
        "Fraction1-concentration-x-slice-(t).txt")
{
}

Fraction1ConcentrationVsCoordsOutput::~Fraction1ConcentrationVsCoordsOutput()
{
}

void Fraction1ConcentrationVsCoordsOutput::printToFile(double time)
{
    Space& space = *(m_parent->m_space);
    double maxVal = space.gridDescription->axis[SPACE_COORDS_X_COORD].getMaxValue();
    double minVal = space.gridDescription->axis[SPACE_COORDS_X_COORD].getMinValue();
    
    double spacePoint[SPACE_COORDS_COUNT] = { 0 };
    
    for (unsigned int sapceIndex=0; sapceIndex<m_pointsCount; sapceIndex++)
    {
        spacePoint[SPACE_COORDS_X_COORD] = minVal + (maxVal-minVal) * sapceIndex / m_pointsCount;
        
        FractionsPool *spaceCell = space.accessElement_d(spacePoint);
        Fraction1Space *fractionSpace = static_cast<Fraction1Space*> (spaceCell->fractions[FRACTION_FRACTION1]);
        // Convolution in fraction1 space cell
        double result = 0;
        for (unsigned int particleIndex=0; particleIndex < fractionSpace->elementsCount; particleIndex++)
        {
            Fraction1Cell& cell = fractionSpace->elements[particleIndex];
            result += cell.quantities[FRACTION1_QUANTITIES_PARTICLES_COUNT] / cell.volume;
        }
        (*m_file) << time << " " << spacePoint[SPACE_COORDS_X_COORD] << " " << result << std::endl;
    }
}


/////////////////////////
// Fraction1ConcentrationVsVelocityOutput

Fraction1ConcentrationVsVelocityOutput::Fraction1ConcentrationVsVelocityOutput() :
    OutputInstanceBase(FRACTION1_QUANTITIES_PARTICLES_COUNT,
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
    
    Fraction1Space *fractionSpace = static_cast<Fraction1Space*> ( space.accessElement_d(spacePoint)->fractions[FRACTION_FRACTION1] );
    
    double maxVal = fractionSpace->gridDescription->axis[FRACTION1_COORDS_X_SPEED].getMaxValue();
    double minVal = fractionSpace->gridDescription->axis[FRACTION1_COORDS_X_SPEED].getMinValue();
    
    double fractionSubspacePoint[FRACTION1_COORDS_COUNT] = { 0 };
    
    for (unsigned int fractionSapceIndex=0; fractionSapceIndex<m_pointsCount; fractionSapceIndex++)
    {
        fractionSubspacePoint[FRACTION1_COORDS_X_SPEED] = minVal + (maxVal-minVal) * fractionSapceIndex / m_pointsCount;
        
        Fraction1Cell *fractionCell = static_cast<Fraction1Cell *>(fractionSpace->accessElement_d(fractionSubspacePoint));
        // Convolution in fraction1 space cell
        double result = fractionCell->quantities[FRACTION1_QUANTITIES_PARTICLES_COUNT];
        (*m_file) << time << " " << fractionSubspacePoint[FRACTION1_COORDS_X_SPEED] << " " << result << std::endl;
    }
}

*/  
