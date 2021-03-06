#include "Cubelet.h"


char ColorUtil::toChar(SideColor c)
{
    switch (c)
    {
        case Yellow:
            return 'Y';
        case White:
            return 'W';
        case Red:
            return 'R';
        case Orange:
            return 'O';
        case Blue:
            return 'B';
        case Green:
            return 'G';
        default:
            assert(0);
    }
}

SideColor ColorUtil::toEnum(char c)
{
    switch (c)
    {
        case 'Y':
            return Yellow;
        case 'W':
            return White;
        case 'R':
            return Red;
        case 'O':
            return Orange;
        case 'B':
            return Blue;
        case 'G':
            return Green;
        default:
            assert(0);
    }
}

glm::vec3 ColorUtil::toGlmVec(SideColor c)
{
    switch (c)
    {
        case Yellow:
            return { 1.0f, 1.0f, 0.0f };
        case White:
            return { 1.0f, 1.0f, 1.0f };
        case Red:
            return { 1.0f, 0.0f, 0.0f };
        case Orange:
            return { 1.0f, 0.5f, 0.0f };
        case Blue:
            return { 0.0f, 0.0f, 1.0f };
        case Green:
            return { 0.0f, 1.0f, 0.0f };
        case None:
            return { 0.0f, 0.0f, 0.0f };
        default:
            assert(0);
    }
}

std::vector<float> ColorUtil::toStdVec(SideColor c)
{
    switch (c)
    {
        case Yellow:
            return { 1.0f, 1.0f, 0.0f };
        case White:
            return { 1.0f, 1.0f, 1.0f };
        case Red:
            return { 1.0f, 0.0f, 0.0f };
        case Orange:
            return { 1.0f, 0.5f, 0.0f };
        case Blue:
            return { 0.0f, 0.0f, 1.0f };
        case Green:
            return { 0.0f, 1.0f, 0.0f };
        case None:
            return { 0.0f, 0.0f, 0.0f };
        default:
            assert(0);
    }
}

float ColorUtil::getHue(const glm::vec3& c)
{
    const auto& r = c[0];
    const auto& g = c[1];
    const auto& b = c[2];
    const auto& v = std::max({ r, g, b });
    const auto& u = std::min({ r, g, b });

    if (v - u < 0.1)
        return 180.0f;

    float out;

    if (v == r)
        out = 60.0f * (g - b) / (v - u);
    else if (v == g)
        out = 120.0f + 60.0f * (b - r) / (v - u);
    else
        out = 240.0f + 60.0f * (r - g) / (v - u);

    if (out < 0)
        out += 360.0f;

    return out;
}

SideColor ColorUtil::guessColor(const glm::vec3 & colorVec)
{
    static std::map<SideColor, float> colorMap{
        { Yellow, ColorUtil::getHue(ColorUtil::toGlmVec(Yellow)) },
        { White, ColorUtil::getHue(ColorUtil::toGlmVec(White)) },
        { Red, ColorUtil::getHue(ColorUtil::toGlmVec(Red)) },
        { Orange, ColorUtil::getHue(ColorUtil::toGlmVec(Orange)) },
        { Blue, ColorUtil::getHue(ColorUtil::toGlmVec(Blue)) },
        { Green, ColorUtil::getHue(ColorUtil::toGlmVec(Green)) },
    };

    auto colorHue = ColorUtil::getHue(ColorUtil::normalizedColor(colorVec));
    SideColor bestGuess = White;
    float bestDiff = 1e6;

    for (auto it = colorMap.begin(); it != colorMap.end(); ++it)
    {
        auto diff = std::abs(it->second - colorHue);

        if (it->first == Red)
            diff = std::min(diff, std::abs(360.0f - colorHue));

        if (diff < bestDiff)
        {
            bestDiff = diff;
            bestGuess = it->first;
        }
    }

    return bestGuess;
}

glm::vec3 ColorUtil::normalizedColor(const glm::vec3 & c)
{
    return c / 255.0f;
}

Cubelet::Cubelet(int i, int j, int k) noexcept : i(i), j(j), k(k)
{
    reset_colors();
    reset_rotation();
    reset_translation();
}

void Cubelet::reset_colors()
{
    colors.clear();
    append(colors, k == 0 ? ColorUtil::toStdVec(Yellow) : ColorUtil::toStdVec(None));
    append(colors, k == 2 ? ColorUtil::toStdVec(White) : ColorUtil::toStdVec(None));
    append(colors, i == 0 ? ColorUtil::toStdVec(Red) : ColorUtil::toStdVec(None));
    append(colors, i == 2 ? ColorUtil::toStdVec(Orange) : ColorUtil::toStdVec(None));
    append(colors, j == 0 ? ColorUtil::toStdVec(Blue) : ColorUtil::toStdVec(None));
    append(colors, j == 2 ? ColorUtil::toStdVec(Green) : ColorUtil::toStdVec(None));
}

void Cubelet::reset_rotation()
{
    rotation = glm::mat4(1.0f);
}

void Cubelet::reset_translation()
{
    translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.35f * (i - 1), 0.35f * (j - 1), 0.35f * (k - 1)));

    full_translation = glm::scale(translation, glm::vec3(0.35f));
    translation = glm::scale(translation, glm::vec3(1.0f / 3.0f));
}

void Cubelet::rotate(RotationAxis rot)
{
    switch (rot)
    {
        case X:
            swap_3(colors, 1, 5);
            swap_3(colors, 5, 0);
            swap_3(colors, 0, 4);
            break;
        case Y:
            swap_3(colors, 1, 3);
            swap_3(colors, 3, 0);
            swap_3(colors, 0, 2);
            break;
        case Z:
            swap_3(colors, 2, 5);
            swap_3(colors, 5, 3);
            swap_3(colors, 3, 4);
            break;
    }
}

void Cubelet::setFaceColor(int index, SideColor c)
{
    auto colorVec = ColorUtil::toStdVec(c);
    colors[index * 3] = colorVec[0];
    colors[index * 3 + 1] = colorVec[1];
    colors[index * 3 + 2] = colorVec[2];
}

template<typename T>
void append(std::vector<T>& v1, const std::vector<T>& v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}

template<typename T>
void swap_3(std::vector<T>& v, int offset1, int offset2)
{
    for (int i = 0; i < 3; ++i)
        std::swap(v[offset1 * 3 + i], v[offset2 * 3 + i]);
}
