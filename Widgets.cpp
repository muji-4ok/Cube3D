#include "Widgets.h"

void ItemBoxController::popFront()
{
    itemBoxModel->items.pop_front();
}

void TextBoxController::setText(const std::string & text)
{
    textBoxModel->textLines.clear();

    std::string word;

    for (const auto&c : text)
        if (c == '\n')
            if (word.size())
            {
                textBoxModel->textLines.push_back(word);
                word.clear();
            }
        else
            word.push_back(c);

    if (word.size())
        textBoxModel->textLines.push_back(word);
}
