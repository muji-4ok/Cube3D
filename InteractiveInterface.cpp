#include "InteractiveInterface.h"


std::string InstructionsBoxModel::restoreItem()
{
    std::string res;

    if (rotationsQueue.size())
    {
        addItemFront(std::string(1, rotationsQueue.back()));
        res.push_back(rotationsQueue.back());
        rotationsQueue.pop_back();
    }

    return res;
}

std::string InstructionsBoxModel::popItem()
{
    auto res = Base::popItem();

    if (res.size())
        rotationsQueue.push_back(res[0]);

    return res;
}

void InstructionsBoxModel::clearItems()
{
    Base::clearItems();
    rotationsQueue.clear();
}
