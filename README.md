# ParallelParallel

Final Project for CSCI 476

A simulation of fireworks accelerated by parallel programming.

--------------------------------------------------------------------------------

Download:

run "git clone https://github.com/DierF/ParallelParallel.git"

--------------------------------------------------------------------------------

BUILD:

cmake required!

Windows: (Visual Studio 2022 Recommended)

run "./build_windows.bat"

.sln file will be generated in folder "build"

<br>

Linux:

run "bash build_linux.sh"

executable "PParallel" will be generated in folder "build"

--------------------------------------------------------------------------------

Gameplay:

J - Add    a firework(NO repetition if keep pressed)<br>
K - Delete a firework(NO repetition if keep pressed)<br>

W - Move camera forward<br>
S - Move camera backward<br>
A - Move camera left<br>
D - Move camera right<br>
Left Shift - Hold to accelerate camera's motion<br>

Left Mouse Button - Hold and drag to change camera's direction

Space - Pause/Resume objects' motion

O - Start/Stop automatically surrounding mode

Esc - Quit program

--------------------------------------------------------------------------------

3rdParty libraries included:
glfw,
glad,
glm