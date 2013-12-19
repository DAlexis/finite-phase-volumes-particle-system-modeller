#ifndef MODEL_OUTPUT_H_INCLUDED
#define MODEL_OUTPUT_H_INCLUDED

#include "output.h"

class Fraction1ConcentrationOutput : public OutputInstanceBase
{
public:
    Fraction1ConcentrationOutput();
    virtual ~Fraction1ConcentrationOutput();
    
protected:
    virtual void printToFile(double time);
};

#endif // MODEL_OUTPUT_H_INCLUDED
