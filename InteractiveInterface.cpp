#include "InteractiveInterface.h"


std::string InstructionsBoxModel::restoreItem()
{
    std::string res;

    if (rotationsQueue.size())
    {
        addItemFront(InstructionsSanitizer::toStringNotation(rotationsQueue.back()));
        res.push_back(rotationsQueue.back());
        rotationsQueue.pop_back();
    }

    return res;
}

std::string InstructionsBoxModel::popItem()
{
    auto res = Base::popItem();

    if (res.size())
        rotationsQueue.push_back(InstructionsSanitizer::toCharNotation(res));

    return res;
}

void InstructionsBoxModel::clearItems()
{
    Base::clearItems();
    rotationsQueue.clear();
}

bool InstructionsBoxModel::isEmpty()
{
    return Base::isEmpty() && !rotationsQueue.size();
}

char InstructionsSanitizer::toCharNotation(const std::string & in)
{
    if (in.size() == 1)
        return in[0];
    else if (in.size() == 2)
        return std::tolower(in[0]);
    else
        throw std::runtime_error("Incorrect input");
}

std::string InstructionsSanitizer::toStringNotation(char r)
{
    if (r <= 'Z')
        return std::string(1, r);
    else
        return std::string(1, std::toupper(r)) + '\'';
}

char InstructionsSanitizer::reverseChar(char r)
{
    if (r <= 'Z')
        return r + 32;
    else
        return r - 32;
}
