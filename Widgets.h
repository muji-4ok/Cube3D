#pragma once
#include <string>
#include <vector>
#include <deque>
#include <glm/glm.hpp>


struct TextBoxModel
{
    std::vector<std::string> textLines;
    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 bgColor;
    glm::vec3 textColor;
};

struct ButtonModel : public TextBoxModel
{
    bool pressed;
};

struct ItemBoxModel
{
    std::deque<TextBoxModel> items;
};

class TextBoxController
{
public:
    void setText(const std::string& text);

private:
    TextBoxModel* textBoxModel;
};

class ButtonController : public TextBoxController
{
public:
    virtual void onMousePress() = 0;
    virtual void onMouseRelease() = 0;

private:
    ButtonModel* buttonModel;
};

class ItemBoxController
{
public:
    void popFront();

private:
    ItemBoxModel* itemBoxModel;
};
