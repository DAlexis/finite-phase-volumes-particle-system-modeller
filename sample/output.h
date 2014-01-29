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
    OutputInstance(OutputMaker* parent);
    ~OutputInstance();
    /// Configure axis in output
    void configAxis(int axisNumber,
        OutputAxisType type,
        unsigned int pointsCount,
        unsigned int axisIndex);
    
    void setParent(OutputMaker* parent);
    
    void setFilenamePrefix(const std::string& filenamePrefix);
    
    double* getFractionPoint() { return fractionPoint; }
    double* getSpacePoint() { return spacePoint; }
    
    void setFractionAndQuantity(uint fractionId, uint quantityId) { m_fractionId = fractionId; m_quantityId = quantityId; }
    void setPeriod(double period) { m_period = period; }
    
    void output(double time);
    
private:
    void continiousOutput(double time);
    void regularOutput(double time);
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
    
    OutputAxisDescription axis[3];
    OutputMaker* m_parent;
    
    double m_period;
    std::string m_filenamePrefix;
    bool isFirstTime;
    double lastOutputTime;
    std::ofstream* m_file;
    
    uint m_fractionId, m_quantityId;
    
    double fractionPoint[MAX_FRACTION_DIMENSION];
    double spacePoint[SPACE_COORDS_COUNT];
    
    OutputMode mode;
};

/*
enum RegularOutputDimension
{
    ROI_2D,
    ROI_3D
};

class OutputInstanceBase
{
public:
    OutputInstanceBase();
    void setParent(OutputMaker* parent);
    void setPeriod(double timePeriod);
    
    void output(double time) = 0;
    virtual ~OutputInstanceBase() {}
protected:
    virtual doRealOutput(double time)
    OutputMaker* m_parent;
    double m_timePeriod;
    double m_lastOutputTime;
    bool m_isFirstTime;
};

class RegularOutputInstance : public OutputInstanceBase
{
public:
    RegularOutputInstance(const std::string& fileNamePrefix);
    virtual ~RegularOutputInstance() {};
    
    void configure(int fractionIndex, RegularOutputDimension mode);
    void setAxis(int first, int second);
    /// Set point that belongs plant of output
    void setPoint(double* point);
    
    double* point();
    
protected:
    void output(double time);
    
    const std::string m_fileNamePrefix;
    
    RegularOutputDimension m_mode;
    double m_point[SPACE_COORDS_COUNT];
    int m_firstAxis, m_secondAxis;
    int m_fractionIndex;
};
*/
/*
class RegularOutputInstance : public OutputInstanceBase
{
public:
    RegularOutputInstance(
        double period,
        unsigned int pointsCount,
        const std::string& fileName);
    
    virtual ~RegularOutputInstance();
    
    void output(double time);
    
protected:
    /// This function will be called when file is ready and it is time to print data
    virtual void printToFile(double time) {}
    double m_period;
    unsigned int m_pointsCount;
    
    double m_lastOutput;
    bool m_isFirst;
    /// Pointer is used because before first output this class can be copied some times in vector that contained it
    std::ofstream* m_file;
};

class QuantityVsSpaceCoordOutputInstance : public RegularOutputInstance
{
public:
    QuantityVsSpaceCoordOutputInstance(
        double period,
        unsigned int pointsCount,
        const std::string& fileName,
        
        int fraction,
        int qunatity,
        int iteratingCoord,
        bool needAnotherCoordsConvolution
        )
    
protected:
    
};*/

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
