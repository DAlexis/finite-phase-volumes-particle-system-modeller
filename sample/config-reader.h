#ifndef CONFIG_READER_H_INCLUDED
#define CONFIG_READER_H_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include <exception>
#include <string>

typedef std::vector<std::string> StringVector;
typedef boost::property_tree::ptree PropertyTree;

class ConfigReader
{
public:
    template<class T>
    static T get(const std::string& name)
    {
        return pt.get<T>(name);
    }
    
    static const PropertyTree& getSubtree(const std::string& name);
    static const PropertyTree& getSubtree(const PropertyTree& where, const std::string& name);
    
    static void getList(const std::string parameter, StringVector& whereToPut);
    
    static void readConfig(const std::string& filename);
    
    /// @todo Add function getDirectory to get config files directory for loading additional files from there
    
    class Exception : public std::exception {};
    
private:
    
    static boost::property_tree::ptree pt;
};

#endif //CONFIG_READER_H_INCLUDED
