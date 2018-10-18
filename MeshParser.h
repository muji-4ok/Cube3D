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

    std::vector<float>&& parse();

private:
    std::vector<Point> vertices;
    std::vector<Point> normals;
};

