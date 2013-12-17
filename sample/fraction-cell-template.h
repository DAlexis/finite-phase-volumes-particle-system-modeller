#ifndef FRACTION_CELL_TEMPLATE
#define FRACTION_CELL_TEMPLATE

template <int SpaceDimension, int FractionSpaceDimension, int QuantitiesCount>
class FractionCell
{
public:
    /// Space coordinates derivatives
    double spaceCoordsDerivatives[SpaceDimension];
    
    /// Fraction coordinates derivatives
    double fractionCoordsDerivatives[FractionSpaceDimension];
    
    FractionCell()
    {
        for (unsigned int i=0; i<QuantitiesCount; i++) {
            quantitiesBuffer0[i] = 0;
            quantitiesBuffer1[i] = 0;
        }
        quantities = quantitiesBuffer0;
        nextStepQuantities = quantitiesBuffer1;
    }
    
    void swapBuffers()
    {
        if (quantities == quantitiesBuffer0)
        {
            quantities = quantitiesBuffer1;
            nextStepQuantities = quantitiesBuffer0;
        } else {
            quantities = quantitiesBuffer0;
            nextStepQuantities = quantitiesBuffer1;
        }
    }
    
    double* quantities;
    double* nextStepQuantities;
    
private:
    double quantitiesBuffer0[QuantitiesCount];
    double quantitiesBuffer1[QuantitiesCount];
};

#endif // FRACTION_CELL_TEMPLATE
