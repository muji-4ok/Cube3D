#pragma once
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <utility>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TextModel.h"
#include "RectangleModel.h"
#include "WindowModel.h"
#include "Events.h"


struct ButtonModel
{
    ButtonModel(const glm::vec2& rectPos, const glm::vec2& rectSize, const std::string& text,
                float textScale, const glm::vec3& bgColorNormal, const glm::vec3& bgColorPressed,
                const glm::vec3& textColor, const std::function<void()>& callback)
        : rectModel(rectPos, bgColorNormal, rectSize), textModel(text, {}, textColor, textScale),
        bgColorNormal(bgColorNormal), bgColorPressed(bgColorPressed), callback(callback)
    {
    }

    RectangleModel rectModel;
    TextModel textModel;
    bool pressed = false;
    glm::vec3 bgColorPressed;
    glm::vec3 bgColorNormal;
    std::function<void()> callback;
};

struct TextBoxModel
{
    TextBoxModel(const glm::vec2& rectPos, const glm::vec2& rectSize, const glm::vec3& bgColor)
        : rectModel(rectPos, bgColor, rectSize)
    {
    }

    void addTextLine(const glm::vec2& textPos, const std::string& text, float textScale,
                     const glm::vec3& textColor);

    RectangleModel rectModel;
    std::vector<TextModel> textLines;
};

struct ItemBoxModel
{
    // iRectSize, horPadding, itemHorSpace - in pixels
    // everything else - in NDC
    ItemBoxModel(const glm::vec2& rectPos, const glm::vec2& rectSize, const glm::vec3& bgColor,
                 const glm::vec2& iRectSize, const glm::vec3& iBgColor, float iTextScale, const glm::vec3& iTextColor,
                 float horPadding, float itemHorSpace)
        : rectModel(rectPos, bgColor, rectSize), itemRect({}, iBgColor, iRectSize),
        itemText("", {}, iTextColor, iTextScale),
        placeHolderItemText("...", {}, iTextColor, iTextScale),
        horPadding(horPadding), itemHorSpace(itemHorSpace)
    {
    }

    void addItem(const std::string& text);
    void addItemFront(const std::string& text);
    virtual std::string popItem();
    virtual void clearItems();

    RectangleModel rectModel;
    std::deque<std::pair<TextModel, RectangleModel>> items;
    RectangleModel itemRect;
    TextModel itemText;
    TextModel placeHolderItemText;
    float horPadding;
    float itemHorSpace;
};

class ButtonController
{
public:
    ButtonController(ButtonModel* bm, const WindowModel* wm) : buttonModel(bm), windowModel(wm) {}

    void setModel(ButtonModel* bm);
    void onMousePress(MouseDownEvent* e);
    void onMouseRelease(MouseUpEvent* e);

private:
    bool isInside(const glm::vec2& mouse_pos) const;
    const WindowModel* windowModel;
    ButtonModel* buttonModel;
};
