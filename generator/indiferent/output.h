/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include "space.h"
#include <vector>
#include <string>
#include <fstream>

class OutputMaker;

enum OutputAxisType
{
    OAT_NOT_DEFINED = 0,
    OAT_SPACE_COORDINATE,
    OAT_FRACTION_COORDINATE
};

enum OutputQuantityType
{
    OCT_NOT_DEFINED = 0,
    OCT_PRIMARY_QUANTITY,
    OCT_SECONDARY_QUANTITY
};

#define MAX_FRACTION_DIMENSION  15

class OutputInstance
{
public:
    OutputInstance();
    ~OutputInstance();
    /// Configure axis in output
    OutputInstance* addOutputAxis(OutputAxisType type, unsigned int pointsCount, unsigned int axisIndex);
    OutputInstance* useConvolutionByFractionAxis(unsigned int axisIndex);
    OutputInstance* useAllFractionSpaceConvolution(unsigned int fractionSpaceDimension);
    
    OutputInstance* setFilenamePrefix(const std::string& filenamePrefix);
    OutputInstance* setFractionAndQuantity(uint fractionId, uint quantityId, OutputQuantityType quantityType);
    OutputInstance* setPeriod(double period);
    
    
    double* getFractionPoint() { return fractionPoint; }
    double* getSpacePoint() { return spacePoint; }
    
    void setParent(OutputMaker* parent);
    void output(double time);
    
private:
    void recursiveIterate(uint axisIndex, std::string fileName);
    
    struct OutputAxisDescription
    {
        OutputAxisDescription():
            pointsCount(10),
            type(OAT_NOT_DEFINED),
            axisIndex(0)
        {}
        unsigned int pointsCount;
        OutputAxisType type;
        unsigned int axisIndex;
    };
    
    std::vector<OutputAxisDescription> axis;
    std::vector<uint> convolutionAxis;
    OutputMaker* m_parent;
    Space* m_space;
    
    double m_period;
    std::string m_filenamePrefix;
    
    bool isFirstTime;
    double lastOutputTime;
    
    std::ofstream* m_file;
    
    uint m_fractionId, m_quantityId;
    OutputQuantityType m_quantityType;
    
    double fractionPoint[MAX_FRACTION_DIMENSION];
    double spacePoint[SPACE_COORDS_COUNT];
    
    double m_currentTime;
};

class OutputMaker
{
public:
    OutputMaker(Space* space);
    ~OutputMaker();
    
    void addInstance(OutputInstance* instance);
    
    void output(double time);
    
    Space* m_space;
    
private:
    std::vector<OutputInstance*> m_instances;
};

#endif //OUTPUT_H_INCLUDED
