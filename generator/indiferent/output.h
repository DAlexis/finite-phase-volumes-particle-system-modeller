/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include "space-base.h"
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
    OCT_EXTENSIVE_QUANTITY,
    OCT_INTENSIVE_QUANTITY,
};

#define MAX_FRACTION_DIMENSION  15
#define MAX_SPACE_DIMENSION     15

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
    void finalise();
    
private:
    void createGnuplotFile(const std::string& filenamePrefix);
    void quantityVsAxisAndTimeIterate();
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
    ISpace* m_space;
    
    double m_period;
    std::string m_filenamePrefix;
    
    bool isFirstTime;
    double lastOutputTime;
    
    std::ofstream* m_dataFile;
    
    uint m_fractionId, m_quantityId;
    OutputQuantityType m_quantityType;
    
    double fractionPoint[MAX_FRACTION_DIMENSION];
    double spacePoint[MAX_SPACE_DIMENSION];
    
    double m_currentTime;
};

class OutputMaker
{
public:
    OutputMaker(ISpace* space);
    ~OutputMaker();
    
    void addInstance(OutputInstance* instance);
    
    void output(double time);
    void finalise();
    
    ISpace* m_space;
    
private:
    std::vector<OutputInstance*> m_instances;
};

#endif //OUTPUT_H_INCLUDED
