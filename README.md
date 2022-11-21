# Assignment 3 -- FMX Rendering and Animation

## Declaration

> The entire code of this assignment is purely our own work and we have not taken any assistance from other students or copied the code from internet and at any point of time we both will be able to explain any part of the code that we have written.

## Members

1. Ashutosh Sathe (21q050012)
2. Animesh (21q050015)

## Movie

[`https://youtu.be/3X2_qD4iIy4`](https://youtu.be/3X2_qD4iIy4) or 

[`https://drive.google.com/file/d/1YHvtwAg5F9scsN7r4panFjI6Sw0rcVp9/view?usp=sharing`](https://drive.google.com/file/d/1YHvtwAg5F9scsN7r4panFjI6Sw0rcVp9/view?usp=sharing)

## Controls

## Hierarchy controls

* `Alt + Left/Right` -- Move to the left or the right sibling of current node
* `Alt + Up` -- Move to the parent of the current node
* `Alt + Down` -- Move to the first children of the current node
* Numbers 1 through 6 -- Select a particular degree of freedom of the current node
* Left bracket (`[`) -- Increase the parameter of the selected degree of freedom
* Right bracket (`]`) -- Decrease the parameter of the selected degree of freedom
* Period (`.`) -- Move to the next animation entity (a parameterized hierarchy node)
* Comma (`,`) -- Move to the previous animation entity (a parameterized hierarchy node)

### Rider and bike combined movements

* `7` -- Moving both rider and bike backward in the direction of rider's view
* `8` -- Moving both rider and bike forward in the direction of rider's view
* `9` -- Rotating both rider and bike anticlockwise along the selected dof axis
* `0` -- Rotating both rider and bike clockwise along the selected dof axis

### Light controls

* `F1` -- Toggle global light 1 on/off
* `F2` -- Toggle global light 2 on/off
* `F3` -- Toggle rider spotlight on/off
* `F4` -- Toggle bike headlight on/off 

### Camera controls

* `B` -- Selecting global camera
* `N` -- Selecting third person camera
* `M` -- Selecting first person camera
* `Y/G/H/J/` -- Change global camera lookat
* `Alt + Y/G/H/J/T/U` -- Move global camera
