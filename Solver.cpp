#include "Solver.h"


std::string FastSolver::generateSolution() const
{
    auto cube_string = toString();

    // std::cerr << "cubeString: " << cube_string << '\n';

    char* c_cube = const_cast<char *>(cube_string.c_str());
    std::string sol;

    auto ret = solution(
        c_cube,
        42,
        1000,
        0,
        "cache"
    );

    if (!ret)
        return "";
    else
        sol = std::string(ret);

    auto moves = splitWords(sol);
    std::string seq;
    auto to_notation = [](const std::string &s) -> std::string {
        bool doub = false;
        bool revr = false;

        if (s.size() > 1)
        {
            doub = (s[1] == '2');
            revr = (s[1] == '\'');
        }

        std::string out;

        if (doub)
        {
            out.assign(2, s[0]);
        }
        else
        {
            auto c = s[0];

            if (revr)
                switch (c)
                {
                    case 'U':
                        c = 'u';
                        break;
                    case 'D':
                        c = 'd';
                        break;
                    case 'L':
                        c = 'l';
                        break;
                    case 'R':
                        c = 'r';
                        break;
                    case 'F':
                        c = 'f';
                        break;
                    case 'B':
                        c = 'b';
                        break;
                }

            out.push_back(c);
        }

        return out;
    };

    for (const auto &m : moves)
        seq += to_notation(m);

    return seq;
}

std::string FastSolver::toString() const
{
    auto vec_to_tuple = [this](int i, int j, int k, int index) -> std::tuple<double, double, double> {
        auto first = this->cubeModel->cubelets[i][j][k].colors[index * 3];
        auto second = this->cubeModel->cubelets[i][j][k].colors[index * 3 + 1];
        auto third = this->cubeModel->cubelets[i][j][k].colors[index * 3 + 2];

        return { first, second, third };
    };

    auto u = vec_to_tuple(1, 2, 1, 5);
    auto r = vec_to_tuple(2, 1, 1, 3);
    auto f = vec_to_tuple(1, 1, 2, 1);
    auto d = vec_to_tuple(1, 0, 1, 4);
    auto l = vec_to_tuple(0, 1, 1, 2);
    auto b = vec_to_tuple(1, 1, 0, 0);

    auto vec_to_char = [&vec_to_tuple, &u, &r, &f, &d, &l, &b, this](int i, int j, int k, int index) -> char {
        auto t = vec_to_tuple(i, j, k, index);

        if (t == r)
            return 'R';
        else if (t == b)
            return 'B';
        else if (t == d)
            return 'D';
        else if (t == l)
            return 'L';
        else if (t == f)
            return 'F';
        else if (t == u)
            return 'U';
    };
    std::vector<std::string> faces(6);

    for (int k = 0; k < 3; ++k)
        for (int i = 0; i < 3; ++i)
            faces[0].push_back(vec_to_char(i, 2, k, 5));

    for (int j = 2; j >= 0; --j)
        for (int k = 2; k >= 0; --k)
            faces[1].push_back(vec_to_char(2, j, k, 3));

    for (int j = 2; j >= 0; --j)
        for (int i = 0; i < 3; ++i)
            faces[2].push_back(vec_to_char(i, j, 2, 1));

    for (int k = 2; k >= 0; --k)
        for (int i = 0; i < 3; ++i)
            faces[3].push_back(vec_to_char(i, 0, k, 4));

    for (int j = 2; j >= 0; --j)
        for (int k = 0; k < 3; ++k)
            faces[4].push_back(vec_to_char(0, j, k, 2));

    for (int j = 2; j >= 0; --j)
        for (int i = 2; i >= 0; --i)
            faces[5].push_back(vec_to_char(i, j, 0, 0));

    std::string out;

    for (const auto &f : faces)
        out += f;

    return out;
}

std::string OptimalSolver::generateSolution() const
{
    auto cubeString = toString();
    auto sol = solve_string(cubeString.c_str());
    auto moves = splitWords(sol);
    std::string out;

    for (const auto&m : moves)
        out += InstructionsSanitizer::toCharNotation(m);

    return out;
}

std::string OptimalSolver::toString() const
{
    auto vec_to_tuple = [this](int i, int j, int k, int index) -> std::tuple<double, double, double> {
        auto first = this->cubeModel->cubelets[i][j][k].colors[index * 3];
        auto second = this->cubeModel->cubelets[i][j][k].colors[index * 3 + 1];
        auto third = this->cubeModel->cubelets[i][j][k].colors[index * 3 + 2];

        return { first, second, third };
    };

    auto u = vec_to_tuple(1, 2, 1, 5);
    auto r = vec_to_tuple(2, 1, 1, 3);
    auto f = vec_to_tuple(1, 1, 2, 1);
    auto d = vec_to_tuple(1, 0, 1, 4);
    auto l = vec_to_tuple(0, 1, 1, 2);
    auto b = vec_to_tuple(1, 1, 0, 0);

    auto vec_to_char = [&vec_to_tuple, &u, &r, &f, &d, &l, &b, this](int i, int j, int k, int index) -> char {
        auto t = vec_to_tuple(i, j, k, index);

        if (t == r)
            return 'R';
        else if (t == b)
            return 'B';
        else if (t == d)
            return 'D';
        else if (t == l)
            return 'L';
        else if (t == f)
            return 'F';
        else if (t == u)
            return 'U';

        assert(false);
    };
    std::vector<std::string> notation;

    // UF
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(1, 2, 2, 5);
    notation.back()[1] = vec_to_char(1, 2, 2, 1);

    // UR
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(2, 2, 1, 5);
    notation.back()[1] = vec_to_char(2, 2, 1, 3);

    // UB
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(1, 2, 0, 5);
    notation.back()[1] = vec_to_char(1, 2, 0, 0);

    // UL
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(0, 2, 1, 5);
    notation.back()[1] = vec_to_char(0, 2, 1, 2);

    // DF
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(1, 0, 2, 4);
    notation.back()[1] = vec_to_char(1, 0, 2, 1);

    // DR
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(2, 0, 1, 4);
    notation.back()[1] = vec_to_char(2, 0, 1, 3);

    // DB
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(1, 0, 0, 4);
    notation.back()[1] = vec_to_char(1, 0, 0, 0);

    // DL
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(0, 0, 1, 4);
    notation.back()[1] = vec_to_char(0, 0, 1, 2);

    // FR
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(2, 1, 2, 1);
    notation.back()[1] = vec_to_char(2, 1, 2, 3);

    // FL
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(0, 1, 2, 1);
    notation.back()[1] = vec_to_char(0, 1, 2, 2);

    // BR
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(2, 1, 0, 0);
    notation.back()[1] = vec_to_char(2, 1, 0, 3);

    // BL
    notation.emplace_back(2, '.');
    notation.back()[0] = vec_to_char(0, 1, 0, 0);
    notation.back()[1] = vec_to_char(0, 1, 0, 2);

    // UFR
    notation.emplace_back(3, '.');
    notation.back()[0] = vec_to_char(2, 2, 2, 5);
    notation.back()[1] = vec_to_char(2, 2, 2, 1);
    notation.back()[2] = vec_to_char(2, 2, 2, 3);

    // URB
    notation.emplace_back(3, '.');
    notation.back()[0] = vec_to_char(2, 2, 0, 5);
    notation.back()[1] = vec_to_char(2, 2, 0, 3);
    notation.back()[2] = vec_to_char(2, 2, 0, 0);

    // UBL
    notation.emplace_back(3, '.');
    notation.back()[0] = vec_to_char(0, 2, 0, 5);
    notation.back()[1] = vec_to_char(0, 2, 0, 0);
    notation.back()[2] = vec_to_char(0, 2, 0, 2);

    // ULF
    notation.emplace_back(3, '.');
    notation.back()[0] = vec_to_char(0, 2, 2, 5);
    notation.back()[1] = vec_to_char(0, 2, 2, 2);
    notation.back()[2] = vec_to_char(0, 2, 2, 1);

    // DRF
    notation.emplace_back(3, '.');
    notation.back()[0] = vec_to_char(2, 0, 2, 4);
    notation.back()[1] = vec_to_char(2, 0, 2, 3);
    notation.back()[2] = vec_to_char(2, 0, 2, 1);

    // DFL
    notation.emplace_back(3, '.');
    notation.back()[0] = vec_to_char(0, 0, 2, 4);
    notation.back()[1] = vec_to_char(0, 0, 2, 1);
    notation.back()[2] = vec_to_char(0, 0, 2, 2);

    // DLB
    notation.emplace_back(3, '.');
    notation.back()[0] = vec_to_char(0, 0, 0, 4);
    notation.back()[1] = vec_to_char(0, 0, 0, 2);
    notation.back()[2] = vec_to_char(0, 0, 0, 0);

    // DBR
    notation.emplace_back(3, '.');
    notation.back()[0] = vec_to_char(2, 0, 0, 4);
    notation.back()[1] = vec_to_char(2, 0, 0, 0);
    notation.back()[2] = vec_to_char(2, 0, 0, 3);

    std::string out;

    for (const auto& n : notation)
        out += n + ' ';

    return out;
}

std::vector<std::string> splitWords(const std::string & s)
{
    std::istringstream iss(s);
    std::vector<std::string> out;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(out));
    return out;
}
