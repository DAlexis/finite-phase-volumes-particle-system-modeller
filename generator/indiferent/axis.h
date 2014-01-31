/*
 * This file is model-independent and should be used as is in any generated model
 */

#ifndef AXIS_H_INCLUDED
#define AXIS_H_INCLUDED

#include <string>
#include <vector>

/// Stores axis parameters. Provides getting segment center and size by index, segment index by coordinate etc.
class Axis
{
public:
    Axis();
    
    void uniformInit(double min, double max, unsigned int count);
    void specificInit(const std::string& fileName);
    
    /// Get segment's middle coordinate value by point's number
    double getPoint(size_t number) const;
    /// Returns segment number that contains point value
    size_t getIndex(double value) const;
    /// Returns segment wigth by point's number
    double getSegmentSize(size_t point) const;
    /// Returns count of segments axis devided to
    size_t getSegmentsCount() const;
    
    /// Absolute minimal value that belongs any segment
    double getMinValue() const;
    /// Minimal coordinate of segment's center
    double getMinSegmentCenter() const;
    /// Absolute maximal value that belongs any segment
    double getMaxValue() const;
    /// Maximal coordinate of segment's center
    double getMaxSegmentCenter() const;
    
    /// Get axis id
    const std::string& getId() const;
    /// Get axis name
    const std::string& getName() const;
    
    /// Set axis id (axis' machine name)
    void setId(const std::string& id);
    /// Set axis human-readable name (for output etc.)
    void setName(const std::string& name);
    
    class Exception : public std::exception {};
    
    class ExInvalidConfig : public Exception
    {
        public:
            virtual const char* what() const throw() { return "Invalid configuration file"; }
    };
    
    class ExSegmentFileIsInvalid : public Exception
    {
        public:
            virtual const char* what() const throw() { return "File with segments is invalid"; }
    };
    
private:
    enum DivisionMode {
        UNIFORM = 0,
        SPECIFIC
    };
    
    void sortPoints();
    
    DivisionMode m_mode;
    std::vector<double> m_points;
    
    double m_minValue, m_maxValue;
    
    std::string m_id;
    std::string m_name;
    size_t m_count;
};

#endif // AXIS_H_INCLUDED
