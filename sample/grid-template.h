#ifndef GRID_TEMPLATE_H_INCLUDED
#define GRID_TEMPLATE_H_INCLUDED

#include "axis.h"
#include "global-defines.h"

template <int AxisCount, int VariablesCount>
class GridElement
{
public:
    double variables[VariablesCount];
    
    /// Fraction coordinates
    double coordinates[AxisCount];
    double size[AxisCount];
    double volume;
    
    /// Neighbors pointers
    GridElement* prev[AxisCount];
    GridElement* next[AxisCount];
    
    GridElement()
    {
        for (unsigned int i=0; i<AxisCount; i++)
        {
            prev[i] = 0; next[i] = 0; size[i] = 0; coordinates[i] = 0;
        }
        volume = 1.0;
    }
};

template <int AxisCount>
class GridDescription
{
public:
    Axis axis[AxisCount];
};

template <int AxisCount, int VariablesCount>
class Grid
{
public:
    void constructGrid(const GridDescription<AxisCount>& description);
    
    GridElement<AxisCount, VariablesCount>* accessElement(const uint* coords);
    GridElement<AxisCount, VariablesCount>* accessElement(const double* coords);
    
protected:
    const GridDescription<AxisCount>* description;
    GridElement<AxisCount, VariablesCount>* elements;
    uint offset[AxisCount];
    uint elementsCount;
    
    /// Service buffer to save coordinates inside class
    double currentCoords_d[AxisCount];
    uint currentCoords_ui[AxisCount];
    
    
    void recursiveInitGridElements(unsigned int depth);
};

#endif // GRID_TEMPLATE_H_INCLUDED
