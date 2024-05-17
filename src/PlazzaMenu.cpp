/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** PlazzaMenu
*/

#include "PlazzaMenu.hpp"

plazza::PlazzaMenu::PlazzaMenu()
{
    parsePlazzaMenuFile(std::string("plazzaMenu.cfg"));
    if (_plazzaMenuVector.empty())
        throw PlazzaMenu::ParsingPlazzaMenuFailed("plazza menu is empty");
    for (auto &pizza : _plazzaMenuVector) {
        if (pizza->getIngredientNumber() == 0)
            throw PlazzaMenu::ParsingPlazzaMenuFailed("pizza " + pizza->getPizzaName() + " has no ingredient");
        if (pizza->getCookingTime() == 0)
            throw PlazzaMenu::ParsingPlazzaMenuFailed("pizza " + pizza->getPizzaName() + " has no cooking time set");
    }
}

std::vector<std::shared_ptr<plazza::Pizza>> plazza::PlazzaMenu::getMenu()
{
    return _plazzaMenuVector;
}

std::vector<std::string> plazza::PlazzaMenu::getIngredients()
{
    return _plazzaIngredientsVector;
}

std::vector<std::string> plazza::PlazzaMenu::getPizzaSizes()
{
    return _plazzaPizzaSizesVector;
}

void plazza::PlazzaMenu::parsePizzaIngredients(std::string &line)
{
    std::size_t pos = 0;
    std::string ingredient;

    while (pos != std::string::npos) {
        pos = line.find_first_of(',');
        ingredient = line.substr(0, pos);
        line.erase(0, pos + 1);
        if (line.length() != 0) {
            if (std::find(_plazzaIngredientsVector.begin(), _plazzaIngredientsVector.end(), ingredient) != _plazzaIngredientsVector.end()) {
                _plazzaMenuVector.back()->pushIngredient(ingredient);
            } else {
                throw PlazzaMenu::ParsingPlazzaMenuFailed("Ingredient \"" + ingredient + "\" from Pizza " + _plazzaMenuVector.back()->getPizzaName() + " doesn't exist in Ingredient List");
            }
        }
    }
}

void plazza::PlazzaMenu::parsePizzaIngredientsLine(std::string &line)
{
    std::string ingredientsLine = "ingredients=";

    if (line.compare(0, ingredientsLine.length(), ingredientsLine) == 0) {
        line.erase(0, ingredientsLine.length());
        parsePizzaIngredients(line);
    }
}

void plazza::PlazzaMenu::parsePizzaCookingTimeLine(std::string &line)
{
    std::string cookingTimeLine = "cookingTime=";

    if (line.compare(0, cookingTimeLine.length(), cookingTimeLine) == 0) {
        line.erase(0, cookingTimeLine.length());
        std::size_t cookingTime = std::atoi(line.c_str());
        if (cookingTime == 0)
            throw PlazzaMenu::ParsingPlazzaMenuFailed("Pizza " + _plazzaMenuVector.back()->getPizzaName() + " cookingTime value is incorrect");
        _plazzaMenuVector.back()->setCookingTime(cookingTime);
    }
}

void plazza::PlazzaMenu::parsePizzaNameLine(std::string &line)
{
    std::string pizzaNameLine = "pizzaName=";

    if (line.compare(0, pizzaNameLine.length(), pizzaNameLine) == 0) {
        line.erase(0, pizzaNameLine.length());
        for (auto &pizza : _plazzaMenuVector) {
            if (pizza->getPizzaName() == line)
                throw PlazzaMenu::ParsingPlazzaMenuFailed("Pizza " + pizza->getPizzaName() + " already exist");
        }
        _plazzaMenuVector.push_back(std::make_shared<plazza::Pizza>());
        _plazzaMenuVector.back()->setPizzaName(line);
    }
}

void plazza::PlazzaMenu::parseIngredientsList(std::string &line)
{
    std::size_t pos = 0;
    std::string ingredient;

    while (pos != std::string::npos) {
        pos = line.find_first_of(',');
        ingredient = line.substr(0, pos);
        line.erase(0, pos + 1);
        if (line.length() != 0) {
            if (std::find(_plazzaIngredientsVector.begin(), _plazzaIngredientsVector.end(), ingredient) == _plazzaIngredientsVector.end()) {
                _plazzaIngredientsVector.push_back(ingredient);
            } else {
                throw PlazzaMenu::ParsingPlazzaMenuFailed("Ingredient \"" + ingredient + "\" already exist in Ingredient List");
            }
        }
    }
}

void plazza::PlazzaMenu::parseIngredientsListLine(std::string &line)
{
    std::string ingredientsListLine = "ingredientsList=";

    if (line.compare(0, ingredientsListLine.length(), ingredientsListLine) == 0) {
        line.erase(0, ingredientsListLine.length());
        parseIngredientsList(line);
    }
}

void plazza::PlazzaMenu::parsePizzaSizesList(std::string &line)
{
    std::size_t pos = 0;
    std::string pizzaSize;

    while (pos != std::string::npos) {
        pos = line.find_first_of(',');
        pizzaSize = line.substr(0, pos);
        line.erase(0, pos + 1);
        if (line.length() != 0) {
            if (std::find(_plazzaPizzaSizesVector.begin(), _plazzaPizzaSizesVector.end(), pizzaSize) == _plazzaPizzaSizesVector.end()) {
                _plazzaPizzaSizesVector.push_back(pizzaSize);
            } else {
                throw PlazzaMenu::ParsingPlazzaMenuFailed("Pizza Size \"" + pizzaSize + "\" already exist in PIzzaSize List");
            }
        }
    }
}

void plazza::PlazzaMenu::parsePizzaSizesListLine(std::string &line)
{
    std::string pizzaSizesListLine = "pizzaSizesList=";

    if (line.compare(0, pizzaSizesListLine.length(), pizzaSizesListLine) == 0) {
        line.erase(0, pizzaSizesListLine.length());
        parsePizzaSizesList(line);
    }
}

void plazza::PlazzaMenu::parsePlazzaMenuFile(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open())
        throw PlazzaMenu::ParsingPlazzaMenuFailed("File name " + fileName + " doesn't exist");
    while (std::getline(file, line)) {
        parseIngredientsListLine(line);
        parsePizzaSizesListLine(line);
        parsePizzaNameLine(line);
        parsePizzaCookingTimeLine(line);
        parsePizzaIngredientsLine(line);
    }
}
