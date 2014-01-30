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

#define MAX_FRACTION_DIMENSION  15

class OutputInstance
{
public:
    OutputInstance();
    ~OutputInstance();
    /// Configure axis in output
    OutputInstance* configAxis(OutputAxisType type, unsigned int pointsCount, unsigned int axisIndex);
    
    OutputInstance* setFilenamePrefix(const std::string& filenamePrefix);
    OutputInstance* setFractionAndQuantity(uint fractionId, uint quantityId);
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
    
    enum OutputMode
    {
        OM_NOT_DEFINED = 0,
        OM_CONTINIOUS,
        OM_REGULAR
    };
    
    bool hasParticleAxis;
    bool hasSpaceAxis;
    bool hasTimeAxis;
    
    int enabledAxisCount;
    
    std::vector<OutputAxisDescription> axis;
    OutputMaker* m_parent;
    Space* m_space;
    
    double m_period;
    std::string m_filenamePrefix;
    bool isFirstTime;
    double lastOutputTime;
    std::ofstream* m_file;
    
    uint m_fractionId, m_quantityId;
    
    double fractionPoint[MAX_FRACTION_DIMENSION];
    double spacePoint[SPACE_COORDS_COUNT];
    
    double m_currentTime;
    OutputMode mode;
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
