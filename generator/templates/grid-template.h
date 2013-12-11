#ifndef GRID_TEMPLATE_H_INCLUDED
#define GRID_TEMPLATE_H_INCLUDED

#include "axis.h"
#include "global-defines.h"

template <int AxisCount, class AssociatedData>
class GridElement
{
public:
    /// Fraction coordinates
    double coordinates[AxisCount];
    double size[AxisCount];
    double volume;
    
    /// Neighbors pointers
    GridElement* prev[AxisCount];
    GridElement* next[AxisCount];
    
    void* parentGrid;
    
    AssociatedData data;
    
    GridElement() : parentGrid(0)
    {
        for (unsigned int i=0; i<AxisCount; i++)
        {
            prev[i] = 0; next[i] = 0; size[i] = 0; coordinates[i] = 0;
        }
        volume = 1.0;
    }
    
    void init(void* parent) { parentGrid = parent; }
};

template <int AxisCount>
class GridDescription
{
public:
    Axis axis[AxisCount];
};

template <int AxisCount, class AssociatedData>
class Grid
{
public:
    typedef GridElement<AxisCount, AssociatedData> GridElementType;
    
    void constructGrid(const GridDescription<AxisCount>& description)
    {
        gridDescription = &description;
        // Calculating offsets
        calculateOffsetsAndElementsCount();
        
        // Preparing internal buffers for double and uint coords to operate using them
        for (uint i = 0; i != AxisCount; i++)
        {
            currentCoords_ui[i] = 0;
            currentCoords_d[i] = 0.0;
        }
        // Allocating memory
        elements = new GridElementType[elementsCount];
        
        // Setting grid pointer
        for (size_t i = 0; i != elementsCount; i++)
            elements[i].init(this);
        
        // Initialising size, neighbor pointers, etc.
        recursiveInitGridElements(0);
    }
    
    Grid() : elements(0), elementsCount(0) {}
    ~Grid() { if (elements) delete[] elements; }
    
    GridElementType* accessElement(const uint* coords)
    {
        size_t resInd = 0;
        for (uint i=0; i!=AxisCount; i++)
        {
            resInd += offsets[i]*coords[i];
        }
        return &(elements[resInd]);
    }

    GridElementType* accessElement(const double* coords)
    {
        size_t resInd = 0;
        for (uint i=0; i!=AxisCount; i++)
        {
            resInd += offsets[i]*gridDescription->axis[i].getIndex(coords[i]);
        }
        return &(elements[resInd]);
    }

    
protected:
    const GridDescription<AxisCount>* gridDescription;
    GridElementType* elements;
    uint offsets[AxisCount];
    uint elementsCount;
    
    /// Service buffer to save double coordinates inside class
    double currentCoords_d[AxisCount];
    /// Service buffer to save unsigned int coordinates inside class
    uint currentCoords_ui[AxisCount];
    
    void recursiveInitGridElements(unsigned int depth)
    {
        if (depth < AxisCount)
        {
            for (uint i=0; i!=gridDescription->axis[depth].getSegmentsCount(); i++)
            {
                currentCoords_ui[depth] = i;
                recursiveInitGridElements(depth+1);
            }
        } else {
            // Initialising GridElement
            GridElementType *currentElement = accessElement(currentCoords_ui);
            currentElement->volume = 1;
            for (uint i=0; i!=AxisCount; i++)
            {
                // Previous cells
                if (currentCoords_ui[i] != 0) {
                    currentCoords_ui[i]--;
                    currentElement->prev[i] = accessElement(currentCoords_ui);
                    currentCoords_ui[i]++;
                }
                // Next cells
                if (currentCoords_ui[i] != gridDescription->axis[i].getSegmentsCount()-1)
                {
                    currentCoords_ui[i]++;
                    currentElement->next[i] = accessElement(currentCoords_ui);
                    currentCoords_ui[i]--;
                }
                // Coordinates
                currentElement->coordinates[i] = gridDescription->axis[i].getPoint(currentCoords_ui[i]);
                // Segment size
                currentElement->size[i] = gridDescription->axis[i].getSegmentSize(currentCoords_ui[i]);
                // Volume
                currentElement->volume *= currentElement->size[i];
            }
        }
    }
    
    /// Caculate offsets array and elements count
    void calculateOffsetsAndElementsCount()
    {
        offsets[AxisCount-1] = 1;
        for (int i = AxisCount-2; i >= 0 ; i--)
            offsets[i] = offsets[i+1]*gridDescription->axis[i+1].getSegmentsCount();
        elementsCount = offsets[0]*gridDescription->axis[0].getSegmentsCount();
    }
};

#endif // GRID_TEMPLATE_H_INCLUDED
