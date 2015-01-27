#ifndef EMPIRIC_CURVES_READER_H_INCLUDED
#define EMPIRIC_CURVES_READER_H_INCLUDED

#include <string>
#include <vector>

class EmpiricCurvesReader
{
public:
    enum InterpolationMode
    {
        NEAREST = 0,
        LINEAR,
    };
    
    EmpiricCurvesReader(std::string filename, bool xScaleLog = false, bool yScaleLog = false);
    
    void setInterpolation(InterpolationMode interpolationMode);
    double get(double x);
    double operator[](double x);
    
private:
    std::pair<double, double> findNearestIndexes(double x);
    std::vector<std::pair<double, double>> m_points;
    InterpolationMode m_interpolationMode = LINEAR;
};

#endif // EMPIRIC_CURVES_READER_H_INCLUDED
