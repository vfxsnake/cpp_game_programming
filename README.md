# cpp_game_programming
SFML and ImGui 2d game engine implementing the Entity Component System (ECS)

## adding ImGui extenral libraries to clone
* git https://github.com/ocornut/imgui.git external/imgui
* git clone https://github.com/SFML/imgui-sfml.git external/imgui branch 2.6.x

* the SFML version should be 2.6.1 in order to work with imgui-sfml
    - in linux you must compile this version for your system.
    - cd SFML-2.6.1
    - mkdir build
    - cd build
    - cmake ..
    - make -j$(nproc)
    - sudo make install

## compile and run using configured the Makefile. 
* make run

### gp-4300_assigment_1 branch

Program Specification
this assignment will wirte a C++/SFML program.
* read the descriptions of shapes (spheres and rectangles) from a config file.
* draw the shapes to scree.
* each shape will have a name, position, speed, color, and properties uniques for each shape:
    - Rectangles will have width and height.
    - Circles will have Radius.
    ** Note that the position origin is in the top left corner of the screen in SFML.
* the shape should move along the x and y axis of the screen and they should bounce
    when the reach the limits of the the window.
* the center of the text (name of the shape ) should be at center of the shape.
* the program must have implemented a ImGui interface with the following requirements:
    - List all shapes and select in a combo box.
    - the shape selected will populate the ui info so the ui can edit the values.
    - the ui requires a checkbox for toggling the visibility fo the shape (draw).
    - the ui requires a slider to scale the shape with a range form 0 to 4.
    - a slider for x velocity with a range from -8 to 8.
    - a slider for y velocity with a range from -8 to 8.
    - a color widget to change the color of the shape.
    - a text field to change the name of the shape.

### Config file interpretation:

Each line of the configuration file specifies one of the config settings of the program, starting with the type:
** note that the order of the types can vary.

* Window W H
    - Declares a SFML Window size (width and height).

* Font F S R G B 
    - F : font file name std::string with no spaces.
    - S : font size int
    - R G B : color int, int, int from 0 to 255 

* Rectangle N X Y SX SY R G B W H
    - N : Name of the shape std::string
    - X, Y : initial position (float x, float y)
    - SX SY : initial speed (float x, float y)
    - R, G, B : color (int r, int g, int b)
    - W, H : size (float w, float h)

* Circle N X Y SX SY R G B R 
    - N : Name of the shape std::string
    - X, Y : initial position (float x, float y)
    - SX SY : initial speed (float x, float y)
    - R, G, B : color (int r, int g, int b)
    - R : radius

