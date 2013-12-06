#include "config-reader.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/algorithm/string.hpp>

PropertyTree ConfigReader::pt;

void ConfigReader::readConfig(const std::string& filename)
{
    try {
        boost::property_tree::info_parser::read_info(filename, pt);
    }
    catch(boost::property_tree::ini_parser::ini_parser_error &exception) {
        std::cout << std::endl;
        std::cout << "Parsing error in " << exception.filename()
            << ":" << exception.line() << " - " << exception.message() << std::endl;
        throw 1;
    }
    catch(...) {
        std::cout << std::endl;
        std::cout << "Config reading error" << std::endl;
        throw 1;
    }
}

void ConfigReader::getList(const std::string parameter, StringVector& whereToPut)
{
    std::string list = pt.get<std::string>(parameter);
    boost::split(whereToPut, list, boost::is_any_of(",; "));
}

const PropertyTree& ConfigReader::getSubtree(const PropertyTree& where, const std::string& name)
{
    return where.get_child(name);
}

const PropertyTree& ConfigReader::getSubtree(const std::string& name)
{
    return getSubtree(pt, name);
}
