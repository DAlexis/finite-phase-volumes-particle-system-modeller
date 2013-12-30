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

class OutputInstanceBase
{
public:
    OutputInstanceBase(
        int quantity,
        double period,
        unsigned int pointsCount,
        const std::string& fileName);
    
    virtual ~OutputInstanceBase();
    
    void output(double time);
    
    void setParent(OutputMaker* parent);
    
protected:
    /// This function will be called when file is ready and it is time to print data
    virtual void printToFile(double time) {}
    int m_quantity;
    double m_period;
    unsigned int m_pointsCount;
    std::string m_fileName;
    double m_lastOutput;
    bool m_isFirst;
    /// Pointer is used because before first output this class can be copied some times in vector that contained it
    std::ofstream* m_file;
    OutputMaker* m_parent;
};

class OutputMaker
{
public:
    OutputMaker(Space* space);
    ~OutputMaker();
    
    void addInstance(OutputInstanceBase* instance);
    
    void output(double time);
    
    Space* m_space;
    
private:
    std::vector<OutputInstanceBase*> m_instances;
};

#endif //OUTPUT_H_INCLUDED
