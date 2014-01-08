/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef GRID_TEMPLATE_H_INCLUDED
#define GRID_TEMPLATE_H_INCLUDED

#include "axis.h"

/** @brief This is a grid contains elements that are inherited from GridElementType
 */ 
template <int AxisCount, class ElementType>
class Grid
{
public:
    typedef Grid<AxisCount, ElementType> GridInstance;
    
    class GridDescription
    {
    public:
        Axis axis[AxisCount];
    };
    
    class GridElementBase
    {
    public:
        /// Fraction coordinates
        double coordinates[AxisCount];
        double size[AxisCount];
        double volume;
        
        /// Neighbors pointers
        GridElementBase* prev[AxisCount];
        GridElementBase* next[AxisCount];
        
        Grid* parentGrid;
        size_t elementIndex;
        
        GridElementBase() : parentGrid(0), elementIndex(0)
        {
            for (unsigned int i=0; i<AxisCount; i++)
            {
                prev[i] = 0; next[i] = 0; size[i] = 0; coordinates[i] = 0;
            }
            volume = 1.0;
            // Tell associated data where it associated to
        }
        
        virtual ~GridElementBase()
        {}
        
        void init(Grid* parent, size_t index)
        {
            parentGrid = parent;
            elementIndex = index;
        }
    };
    
    void constructGrid(const GridDescription& description)
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
        elements = new ElementType[elementsCount];
        
        // Initialising size, neighbor pointers, etc.
        recursiveInitGridElements(0);
        
        // Setting grid pointer
        for (size_t i = 0; i != elementsCount; i++)
            elements[i].init(this, i);
    }
    
    Grid() : elements(0), elementsCount(0) {}
    virtual ~Grid()
    {
        if (elements) delete[] elements;
    }
    
    ElementType* accessElement_ui(const uint* coords)
    {
        size_t resInd = 0;
        for (uint i=0; i!=AxisCount; i++)
        {
            resInd += offsets[i]*coords[i];
        }
        return &(elements[resInd]);
    }

    ElementType* accessElement_d(const double* coords)
    {
        size_t resInd = 0;
        for (uint i=0; i!=AxisCount; i++)
        {
            resInd += offsets[i]*gridDescription->axis[i].getIndex(coords[i]);
        }
        return &(elements[resInd]);
    }

    ElementType* elements;
    const GridDescription* gridDescription;
    uint elementsCount;
    
protected:
    
    uint offsets[AxisCount];
    
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
            GridElementBase *currentElement = accessElement_ui(currentCoords_ui);
            currentElement->volume = 1;
            for (uint i=0; i!=AxisCount; i++)
            {
                // Previous cells
                if (currentCoords_ui[i] != 0) {
                    currentCoords_ui[i]--;
                    currentElement->prev[i] = accessElement_ui(currentCoords_ui);
                    currentCoords_ui[i]++;
                }
                // Next cells
                if (currentCoords_ui[i] != gridDescription->axis[i].getSegmentsCount()-1)
                {
                    currentCoords_ui[i]++;
                    currentElement->next[i] = accessElement_ui(currentCoords_ui);
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