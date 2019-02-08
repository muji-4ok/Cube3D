#pragma once
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <utility>
#include <functional>
#include <mutex>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TextModel.h"
#include "RectangleModel.h"
#include "WindowModel.h"
#include "Events.h"


/*
* Inherit from this class to create a new button
*
* --rectPos defines position of the button, relative to viewport. It's a 2D vector with values from -1.0f to 1.0f.
* When value is positive, then actual position will be mapped from bottom-left -> top-right. Vise versa if negative.
*
* --rectSize defines size of the button, relative to viewport. It's a 2D with values from 0.0f to 1.0f.
* (Should be {0.15f, 0.07f} if you want the button to look the same as the others.
*
* --text will be shown on the screen. It can contain newlines as well.
*
* --textScale is a positive float. All characters will be scaled according to it.
* (Should be 0.5f if you want the button to look the same as the others).
*
* --bgColorNormal is the color of the button when it's not pressed.
*
* --bgColorPressed is the color of the button when it's pressed.
*
* --textColor is the color of the text.
*
* --callback is the function that is called when the button is successfuly pressed. (The button if released when
* inside the rectangle of the button)
*/
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
    TextBoxModel(const glm::vec2& rectPos, const glm::vec2& rectSize, const glm::vec3& bgColor,
                 const glm::vec3& textColor, const std::string& text, float textScale)
        : rectModel(rectPos, bgColor, rectSize), textModel(text, {}, textColor, textScale)
    {
    }

    RectangleModel rectModel;
    TextModel textModel;
};

struct ItemBoxModel
{
    // iRectSize, horPadding, itemHorSpace - in pixels
    // everything else - in NDC
    ItemBoxModel(const glm::vec2& rectPos, const glm::vec2& rectSize, const glm::vec3& bgColor,
                 const glm::vec2& iRectSize, const glm::vec3& iBgColorPassive, const glm::vec3& iBgColorActive,
                 float iTextScale, const glm::vec3& iTextColor, float horPadding, float itemHorSpace)
        : rectModel(rectPos, bgColor, rectSize), itemRect({}, iBgColorPassive, iRectSize),
        iBgColorActive(iBgColorActive), itemText("", {}, iTextColor, iTextScale),
        placeHolderItemText("...", {}, iTextColor, iTextScale),
        horPadding(horPadding), itemHorSpace(itemHorSpace)
    {
    }

    void addItem(const std::string& text);
    void addItemFront(const std::string& text);
    virtual std::string popItem();
    virtual void clearItems();
    virtual bool isEmpty();

    const RectangleModel& getRectModel() const
    {
        return rectModel;
    }
    const std::deque<std::pair<TextModel, RectangleModel>>& getItems() const
    {
        return items;
    }
    const RectangleModel& getItemRect() const
    {
        return itemRect;
    }
    const float& getHorPadding() const
    {
        return horPadding;
    }
    const float& getItemHorSpace() const
    {
        return itemHorSpace;
    }
    const glm::vec3& getIBgColorActive() const
    {
        return iBgColorActive;
    }
    const TextModel& getPlaceHolderItemText() const
    {
        return placeHolderItemText;
    }

private:
    RectangleModel rectModel;
    std::deque<std::pair<TextModel, RectangleModel>> items;
    RectangleModel itemRect;
    TextModel itemText;
    TextModel placeHolderItemText;
    float horPadding;
    float itemHorSpace;
    glm::vec3 iBgColorActive;

protected:
    std::mutex mutex;
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
