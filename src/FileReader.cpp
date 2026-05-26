#include "FileReader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string FileReader::read(const std::string& path)
{
    std::ifstream file(path);

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}