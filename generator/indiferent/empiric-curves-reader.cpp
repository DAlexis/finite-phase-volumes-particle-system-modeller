#include "empiric-curves-reader.h"
#include <fstream>
#include <algorithm>

#include <iostream>

using namespace std;

EmpiricCurvesReader::EmpiricCurvesReader(std::string filename, bool xScaleLog, bool yScaleLog)
{
    ifstream inpFile(filename, ifstream::in);
    while (!inpFile.eof())
    {
        double x, y;
        inpFile >> x;
        if (inpFile.eof()) break;
        inpFile >> y;
        m_points.push_back(pair<double, double>(x, y));
    }
    inpFile.close();
    sort(
        m_points.begin(),
        m_points.end(),
        [](const std::pair<double, double>& p1, const std::pair<double, double>& p2)
        {
            return p1.first < p2.first;
        }
    );
}

void EmpiricCurvesReader::setInterpolation(InterpolationMode interpolationMode)
{
    m_interpolationMode = interpolationMode;
}


double EmpiricCurvesReader::get(double x)
{
    return (*this)[x];
}

double EmpiricCurvesReader::operator[](double x)
{
    std::pair<double, double> bounds = findNearestIndexes(x);
    double x1 = m_points[bounds.first].first;
    double x2 = m_points[bounds.second].first;
    double y1 = m_points[bounds.first].second;
    double y2 = m_points[bounds.second].second;
    switch(m_interpolationMode)
    {
        default:
        case NEAREST:
            if (x-x1 < x2-x)
                return y1;
            else
                return y2;
        
        case LINEAR:
            if (x < m_points.front().first)
                return m_points[0].second;
            if (x > m_points.back().first)
                return m_points.back().second;
            
            return (x-x1) / (x2-x1) * (y2-y1) + y1;
    }
}

std::pair<double, double> EmpiricCurvesReader::findNearestIndexes(double x)
{
    unsigned int left = 0, right = m_points.size()-1, center = (right+left)/2;
    
    while (right-left > 1)
    {
        if (m_points[center].first > x)
            right = center;
        else
            left = center;
        center = (right+left)/2;
    }
    return std::pair<double, double>(left,right);
}
