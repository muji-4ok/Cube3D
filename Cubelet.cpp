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

SideColor ColorUtil::guessColor(const glm::vec3 & colorVec)
{
    const auto& r = colorVec[0];
    const auto& g = colorVec[1];
    const auto& b = colorVec[2];
    const auto& v = std::max({ r, g, b });
    const auto& u = std::min({ r, g, b });
    float h;
    float s;

    if (u == v)
        h = 180.0f;

    if (v == r)
        h = 60.0f * (g - b) / (v - u);
    else if (v == g)
        h = 120.0f + 60.0f * (b - r) / (v - u);
    else
        h = 240.0f + 60.0f * (r - g) / (v - u);

    if (v != 0)
        s = (v - u) / v;
    else
        s = 0.0f;

    if (b / r > 2.0f && b / g > 2.0f)
        return Blue;
    else if (g / r > 2.0f)
        return Green;
    else if (h > 150.0f || h < 6.0f)
        return Red;
    else if (h < 20.0f && s < 150.0f)
        return White;
    else if (h < 20.0f)
        return Orange;
    else if (h < 50.0f)
        return Yellow;
    else
        return White;
    /*
if blue / red > 2 and blue / green > 2:
    return 'blue'
elif green / red > 2:
    return 'green'

if hue > 150 :
    return 'red'
elif hue < 20 and s < 150:
    return 'white'
elif hue < 20:
    return 'orange'
elif hue < 50:
    return 'yellow'
else:
    return 'white'
     */
    //auto getHue = [](const glm::vec3& c) -> float {
        //const auto& r = c[0];
        //const auto& g = c[1];
        //const auto& b = c[2];
        //const auto& v = std::max({ r, g, b });
        //const auto& u = std::min({ r, g, b });

        //if (u == v)
            //return 180.0f;

        //if (v == r)
            //return 60.0f * (g - b) / (v - u);
        //else if (v == g)
            //return 120.0f + 60.0f * (b - r) / (v - u);
        //else
            //return 240.0f + 60.0f * (r - g) / (v - u);
    //};
    //static std::map<SideColor, float> colorMap{
        //{ Yellow, getHue(ColorUtil::toGlmVec(Yellow)) },
        //{ White, getHue(ColorUtil::toGlmVec(White)) },
        //{ Red, getHue(ColorUtil::toGlmVec(Red)) },
        //{ Orange, getHue(ColorUtil::toGlmVec(Orange)) },
        //{ Blue, getHue(ColorUtil::toGlmVec(Blue)) },
        //{ Green, getHue(ColorUtil::toGlmVec(Green)) },
    //};

    //auto colorHue = getHue(colorVec);
    //SideColor bestGuess = White;
    //float bestDiff = 1e6;

    //for (auto it = colorMap.begin(); it != colorMap.end(); ++it)
    //{
        //auto diff = std::abs(it->second - colorHue);

        //if (diff < bestDiff)
        //{
            //bestDiff = diff;
            //bestGuess = it->first;
        //}
    //}

    //return bestGuess;
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
