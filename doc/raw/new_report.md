---
title: CS675 2022 - Assignment 3 - FMX Rendering and Animation
authors: "Ashutosh Sathe (21q050012), Animesh (21q050015)"
fontsize: 10pt
geometry: 
    - top=1.75cm
    - bottom=1.75cm
    - left=1.5cm
    - right=1.5cm
...

# Declaration

> The entire code of this assignment is purely our own work and we have not taken any assistance from other students or copied the code from internet and at any point of time we both will be able to explain any part of the code that we have written.

1. Ashutosh Sathe (21q050012)
2. Animesh (21q050015)

# Rendered video link

[`https://youtu.be/3X2_qD4iIy4`](https://youtu.be/3X2_qD4iIy4)

# Controls

## Hierarchy controls

* `Alt + Left/Right` -- Move to the left or the right sibling of current node
* `Alt + Up` -- Move to the parent of the current node
* `Alt + Down` -- Move to the first children of the current node
* Numbers 1 through 6 -- Select a particular degree of freedom of the current node
* Left bracket (`[`) -- Increase the parameter of the selected degree of freedom
* Right bracket (`]`) -- Decrease the parameter of the selected degree of freedom
* Period (`.`) -- Move to the next animation entity (a parameterized hierarchy node)
* Comma (`,`) -- Move to the previous animation entity (a parameterized hierarchy node)

## Rider and bike combined movements
* `7` -- Moving both rider and bike backward in the direction of rider's view
* `8` -- Moving both rider and bike forward in the direction of rider's view
* `9` -- Rotating both rider and bike anticlockwise along the selected dof axis
* `0` -- Rotating both rider and bike clockwise along the selected dof axis

## Light controls
* `F1` -- Toggle global light 1 on/off
* `F2` -- Toggle global light 2 on/off
* `F3` -- Toggle rider spotlight on/off
* `F4` -- Toggle bike headlight on/off 

## Camera controls
* `B` -- Selecting global camera
* `N` -- Selecting third person camera
* `M` -- Selecting first person camera
* `Y/G/H/J/` -- Change global camera lookat
* `Alt + Y/G/H/J/T/U` -- Move global camera

# Lights

We have included 4 lights in the scene, global light 1, global light 2, rider's spotlight and bike headlight. First two lights are point lights which are used to lights up the whole FMX track, whereas the latter two light are spotlights (i.e whose spread is limited by an angle range). All the lights illuminate the environment using *"Blinn-Phong model"*.

For placing the global lights we have chosen some `3D` coordinates enables us to light up the scene properly, thus global lights are static. On the other hand, rider's spotlight follows the rider everywhere in the scene and bike headlight position and angle changes as the bike changes the same. For limiting the throw of these spotlights we have chosen appropriate angle ranges. This is implemented in `includes/light.hpp`.

# Cameras

# Textures

Figure 1 shows all the textures used in the assignment. We were unable to UV map all the complex geometry within time so we just UV-mapped a single quad in track, bike and rider. We also make sure to properly move this texture with bike and rider separately. The texture also interacts with all the lights. All the texture mapped geometry is drawn with `renderTexturedGL(...)` implemented in `includes/main.hpp`. Our skybox code is heavily inspired from the LearnOpenGL's cubemap tutorial although we use our own modeling machinery (`triangle.hpp`, `point.hpp`) in order to accomplish the goal. The skybox texture was taken from [`http://humus.name/index.php?page=Textures`](http://humus.name/index.php?page=Textures) and is licensed under Creative Commons 3 license.


![All the textures used in this assignment. Top row: Left -- bike headlight, Middle -- Torso back logo, Right -- Track texture. Middle row: negative parts (XYZ) of skybox. Bottom row: positive parts (XYZ) of skybox.](textures.png)

# References

* OpenGL Tutorials -- Tutorial 07 -- [`https://github.com/paragchaudhuri/cs475-tutorials`](https://github.com/paragchaudhuri/cs475-tutorials)
* Hierarchical Modeling -- [`http://graphics.cs.cmu.edu/nsp/course/15-462/Spring04/slides/05-hierarchy.pdf`](http://graphics.cs.cmu.edu/nsp/course/15-462/Spring04/slides/05-hierarchy.pdF)
* Enabling `glVertexAttribPointer` such that could make arranging a single VBO for hierarchical modeling easy -- [`https://stackoverflow.com/a/39684775`](https://stackoverflow.com/a/39684775)
* Track obstacle inspiration -- [`https://www.youtube.com/watch?v=VC1FeM9QuEg`](https://www.youtube.com/watch?v=VC1FeM9QuEg)
