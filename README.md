# Cube3D
3D rubik's cube in GLFW OpenGL C++. Webcam integration with OpenCV

## Building
* Compile OptimalCubeSolver as C code
* Compile FastKociemba as C++ code
* Also needed libraries: GLFW, GLAD, GLM, OpenCV, FreeType

## Adding custom cube solvers
1) Create a new button model class in InteractiveInteface.h subclassed from ButtonModel in Widgets.h.
2) Create a new solver class in Solver.h/Solver.cpp subclassed accordingly. Useful functions in ColorUtil in Cubelet.h.
3) Construct all objects in Rubick_Cube.cpp.
4) Add pointer to button model and call needed functions in InteractiveView class.
