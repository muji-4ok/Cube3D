#include "Widgets.h"


void InterfaceModel::set_orthogonal_projection(float width, float height)
{
    projection = glm::ortho(0.0f, width, 0.0f, height);
}

SolveButton::SolveButton() : rectModel(glm::vec2(0.0f, 0.0f), bgColorNormal, glm::vec2(200.0f, 50.0f)),
                             textModel("Solve", glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f),
                             rectView(&rectModel),
                             textView(&textModel)
{
}

void SolveButton::draw(const glm::mat4 & projection) const
{
    rectView.draw(projection);
    textView.draw(projection);
}

bool SolveButton::isInside(const glm::vec2 & mouse_pos) const
{
    return rectModel.position.x <= mouse_pos.x && mouse_pos.x <= rectModel.position.x + rectModel.size.x &&
        rectModel.position.y <= mouse_pos.y && mouse_pos.y <= rectModel.position.y + rectModel.size.y;
}

void SolveButton::onMousePress()
{
    pressed = true;
    rectModel.color = bgColorPressed;
}

void SolveButton::onMouseRelease()
{
    pressed = false;
    rectModel.color = bgColorNormal;
}
