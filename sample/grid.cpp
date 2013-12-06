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



IGridElementContainment* Grid::createElementContainment(GridElement* element)
{
    return NULL;
}
