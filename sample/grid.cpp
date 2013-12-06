#include "grid-template.h"
#include "config-reader.h"

GridElement::GridElement() :
    coordinates(0),
    volume(0),
    prev(0),
    next(0),
    data(0),
    grid(0)
{ }

GridElement::~GridElement()
{
    if (coordinates) delete[] coordinates;
    if (prev) delete[] prev;
    if (next) delete[] next;
    if (data) delete data;
}

void GridElement::init(Grid* parent)
{
    grid = parent;
    coordinates = new double[grid->gridDescription->axis.size()];
    size = new double[grid->gridDescription->axis.size()];
    prev = new GridElement*[grid->gridDescription->axis.size()];
    next = new GridElement*[grid->gridDescription->axis.size()];
}
/*
void GridDescription::readProperties(const PropertyTree& properties)
{
    BOOST_FOREACH(const PropertyTree::value_type &v, properties.get_child("axis"))
    {
        axis.push_back(Axis());
        axis.back().configure(v.second);
    }
}*/

Grid::Grid() :
    elements(0),
    elementsCount(0)
{
    
}

Grid::~Grid()
{
    if (elements) delete[] elements;
}

void Grid::configureGrid(const GridDescription& description)
{
    gridDescription = &description;
    // Calculating offsets
    offsets.reserve(gridDescription->axis.size());
    calculateOffsets();
    
    // Preparing internal buffers for double and uint coords to operate using them
    currentCoords_d.reserve(gridDescription->axis.size());
    currentCoords_ui.reserve(gridDescription->axis.size());
    for (uint i = 0; i != gridDescription->axis.size(); i++)
    {
        currentCoords_ui[i] = 0;
        currentCoords_d[i] = 0.0;
    }
    
    // Allocating memory
    elements = new GridElement[elementsCount];
    
    // Setting grid pointer
    for (size_t i = 0; i != elementsCount; i++)
        elements[i].init(this);
    
    // Initialising size, neighbor pointers, etc.
    recursiveInitGridElements(0);
    
    // Initialising grid elements containment
    for (size_t i = 0; i != elementsCount; i++)
    {
        elements[i].data = createElementContainment(&(elements[i]));
    }
}

void Grid::calculateOffsets()
{
    offsets[gridDescription->axis.size()-1] = 1;
    elementsCount = gridDescription->axis.back().getSegmentsCount();
    for (int i = gridDescription->axis.size()-2; i >= 0 ; i--) {
        offsets[i] = offsets[i+1]*gridDescription->axis[i+1].getSegmentsCount();
        elementsCount *= gridDescription->axis[i].getSegmentsCount();
    }
}

GridElement* Grid::accessElement(const std::vector<uint>& coords)
{
    size_t resInd = 0;
    for (uint i=0; i!=gridDescription->axis.size(); i++)
    {
        resInd += offsets[i]*coords[i];
    }
    return &(elements[resInd]);
}

GridElement* Grid::accessElement(const std::vector<double>& coords)
{
    size_t resInd = 0;
    for (uint i=0; i!=gridDescription->axis.size(); i++)
    {
        resInd += offsets[i]*gridDescription->axis[i].getIndex(coords[i]);
    }
    return &(elements[resInd]);
}

void Grid::recursiveInitGridElements(unsigned int depth)
{
    if (depth < gridDescription->axis.size())
    {
        for (uint i=0; i!=gridDescription->axis[depth].getSegmentsCount(); i++)
        {
            currentCoords_ui[depth] = i;
            recursiveInitGridElements(depth+1);
        }
    } else {
        // Initialising GridElement
        GridElement *currentElement = accessElement(currentCoords_ui);
        currentElement->volume = 1;
        for (uint i=0; i!=gridDescription->axis.size(); i++)
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

IGridElementContainment* Grid::createElementContainment(GridElement* element)
{
    return NULL;
}
