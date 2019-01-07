#include "Widgets.h"


void ButtonController::setModel(ButtonModel * bm)
{
    buttonModel = bm;
}

void ButtonController::onMousePress(MouseDownEvent * e)
{
    if (e->left_pressed && isInside(e->mouse_pos))
    {
        assert(!buttonModel->pressed);
        buttonModel->pressed = true;
        buttonModel->rectModel.color = buttonModel->bgColorPressed;
    }
}

void ButtonController::onMouseRelease(MouseUpEvent * e)
{
    if (e->left_pressed)
    {
        if (isInside(e->mouse_pos))
            buttonModel->callback();

        buttonModel->pressed = false;
        buttonModel->rectModel.color = buttonModel->bgColorNormal;
    }
}

bool ButtonController::isInside(const glm::vec2 & mouse_pos) const
{
    auto& rectModel = buttonModel->rectModel;
    return rectModel.position.x <= mouse_pos.x && mouse_pos.x <= rectModel.position.x + rectModel.size.x &&
        rectModel.position.y <= mouse_pos.y && mouse_pos.y <= rectModel.position.y + rectModel.size.y;
}

void TextBoxModel::addTextLine(const glm::vec2 & textPos, const std::string & text, float textScale,
                               const glm::vec3 & textColor)
{
    textLines.emplace_back(text, textPos, textColor, textScale);
}

void ItemBoxModel::addItem(const std::string & text)
{
    auto copyRect = itemRect;
    auto copyText = itemText;
    copyText.text = text;
    items.emplace_back(copyText, copyRect);
}

void ItemBoxModel::popItem()
{
    if (items.size())
        items.pop_front();
}
