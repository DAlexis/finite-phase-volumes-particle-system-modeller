/*
 * This file is model-independent and should be used as is in any generated model
 */

#include "axis.h"
#include <algorithm>
#include <fstream>

////////////////////////////////
// Axis

Axis::Axis() :
    m_mode(UNIFORM), m_count(0)
{
}

void Axis::uniformInit(double min, double max, unsigned int count)
{
    m_minValue = min;
    m_maxValue = max;
    m_count = count;
    m_mode = UNIFORM;
}

void Axis::specificInit(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (file.eof()) throw ExSegmentFileIsInvalid();
    
    file >> m_minValue;
    double prevBorder;
    double border = m_minValue;
    while (not file.eof())
    {
        prevBorder = border;
        file >> border;
        if (prevBorder == border) continue; // To delect last line
        m_points.push_back( (border+prevBorder) / 2 );
    }
    m_maxValue = border;
    m_count = m_points.size();
    file.close();
}

double Axis::getPoint(size_t number) const
{
    switch(m_mode)
    {
        case UNIFORM:
            return m_minValue+(m_maxValue-m_minValue)/m_count*(number+0.5);
        break;
        case SPECIFIC:
            return m_points[number];
        break;
        default: return 0;
    }
    
}

double Axis::getSegmentSize(size_t point) const
{
    switch (m_mode) 
    {
        case UNIFORM: 
            return (m_maxValue - m_minValue) / m_count;
        
        case SPECIFIC:
            if (point == 0)         return 2 * (getPoint(0) - m_minValue);
            if (point == m_count-1) return 2 * (m_maxValue  - getPoint(m_count-1));
            return (getPoint(point+1) - getPoint(point-1)) / 2.0;
        default: return 0;
    }
}

size_t Axis::getIndex(double value) const
{
    switch (m_mode) 
    {
        case UNIFORM: 
            return round((value-m_minValue) / (m_maxValue-m_minValue) * (m_count-1) );
        
        case SPECIFIC: {
            size_t left = 0;
            size_t right = m_count-1;
            // Binary search
            while (left+1 != right) {
                size_t center = (right+left) / 2;
                
                if (value <= m_points[center]) {
                    right = center;
                    continue;
                }
                if (m_points[center] <= value) {
                    left = center;
                    continue;
                }
            }
            
            if (m_points[right]-value < value-m_points[left])
                return right;
            else
                return left;
        }
    }
    return 0;
}

size_t Axis::getSegmentsCount() const
{
    return m_count;
}

double Axis::getMinValue() const
{
    return m_minValue;
}

double Axis::getMaxValue() const
{
    return m_maxValue;
}

double Axis::getMinSegmentCenter() const
{
    return getPoint(0);
}

double Axis::getMaxSegmentCenter() const
{
    return getPoint(m_count-1);
}

const std::string& Axis::getId() const
{
    return m_id;
}

const std::string& Axis::getName() const
{
    return m_name;
}

void Axis::setId(const std::string& id)
{
    m_id = id;
}

void Axis::setName(const std::string& name)
{
    m_name = name;
}

void Axis::sortPoints()
{
    std::sort (m_points.begin(), m_points.end());
}
