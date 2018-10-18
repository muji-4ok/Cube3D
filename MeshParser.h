#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>

struct Point
{
    float x;
    float y;
    float z;
};

class MeshParser
{
public:
    MeshParser();
    ~MeshParser();

private:
    std::vector<Point> points;
};

