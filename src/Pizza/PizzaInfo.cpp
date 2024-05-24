/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** PizzaInfo
*/

#include "PizzaInfo.hpp"
#include <algorithm>

plazza::PizzaInfo::PizzaInfo()
{
    initializePizzaInfoVectors();
}

plazza::PizzaInfo::PizzaInfo(plazza::PizzaType type, plazza::PizzaSize size) : _type(type), _size(size)
{
    initializePizzaInfoVectors();
}

plazza::PizzaInfo::PizzaInfo(std::string &stringType, std::string &stringSize)
{
    initializePizzaInfoVectors();
    findStringType(stringType);
    findStringSize(stringSize);
}

void plazza::PizzaInfo::initializePizzaInfoVectors()
{
    initializeTypeVector();
    initializeSizeVector();
}

void plazza::PizzaInfo::initializeTypeVector()
{
    _typeVector.push_back(std::make_pair("regina", Regina));
    _typeVector.push_back(std::make_pair("margarita", Margarita));
    _typeVector.push_back(std::make_pair("americana", Americana));
    _typeVector.push_back(std::make_pair("fantasia", Fantasia));
}

void plazza::PizzaInfo::initializeSizeVector()
{
    _sizeVector.push_back(std::make_pair("S", S));
    _sizeVector.push_back(std::make_pair("M", M));
    _sizeVector.push_back(std::make_pair("L", L));
    _sizeVector.push_back(std::make_pair("XL", XL));
    _sizeVector.push_back(std::make_pair("XXL", XXL));
}

void plazza::PizzaInfo::findStringType(std::string &stringType)
{
    auto it = std::find_if(_typeVector.begin(), _typeVector.end(),
    [stringType] (const std::pair<std::string, plazza::PizzaType> pair)
    {return pair.first == stringType;});

    if (it == _typeVector.end())
        throw PizzaInfo::PizzaInfoError("Unrecognized pizza type");
    _type = it->second;
}

void plazza::PizzaInfo::findStringSize(std::string &stringSize)
{
    auto it = std::find_if(_sizeVector.begin(), _sizeVector.end(),
    [stringSize] (const std::pair<std::string, plazza::PizzaSize> pair)
    {return pair.first == stringSize;});

    if (it == _sizeVector.end())
        throw PizzaInfo::PizzaInfoError("Unrecognized pizza size");
    _size = it->second;
}

std::string plazza::PizzaInfo::getPizzaTypeString()
{
    auto it = std::find_if(_typeVector.begin(), _typeVector.end(),
    [this] (const std::pair<std::string, plazza::PizzaType> pair)
    {return pair.second == _type;});

    if (it == _typeVector.end())
        throw PizzaInfo::PizzaInfoError("Unrecognized pizza type");
    return it->first;
}

std::string plazza::PizzaInfo::getPizzaSizeString()
{
    auto it = std::find_if(_sizeVector.begin(), _sizeVector.end(),
    [this] (const std::pair<std::string, plazza::PizzaSize> pair)
    {return pair.second == _size;});

    if (it == _sizeVector.end())
        throw PizzaInfo::PizzaInfoError("Unrecognized pizza size");
    return it->first;
}

plazza::PizzaType plazza::PizzaInfo::getPizzaType()
{
    return _type;
}

plazza::PizzaSize plazza::PizzaInfo::getPizzaSize()
{
    return _size;
}

void  plazza::PizzaInfo::setPizzaType(plazza::PizzaType type)
{
    _type = type;
}

void  plazza::PizzaInfo::setPizzaSize(plazza::PizzaSize size)
{
    _size = size;
}

void operator<<(std::string &objInfos, plazza::PizzaInfo &pizza)
{
    objInfos += std::to_string(pizza.getPizzaType());
    objInfos += ' ';
    objInfos += std::to_string(pizza.getPizzaSize());
}

void operator>>(const std::string &objInfos, plazza::PizzaInfo &pizza)
{
    std::string line;
    std::istringstream objInfosStream(objInfos);
    unsigned int iteration = 0;

    while (std::getline(objInfosStream, line, ' ')) {
        if (iteration == 0)
            pizza.setPizzaType(static_cast<plazza::PizzaType>(std::atoi(line.c_str())));
        if (iteration == 1)
            pizza.setPizzaSize(static_cast<plazza::PizzaSize>(std::atoi(line.c_str())));
        iteration++;
    }
}