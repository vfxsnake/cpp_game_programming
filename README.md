# cpp_game_programming
SFML and ImGui 2d game engine implementing the Entity Component System (ECS)

## adding ImGui extenral libraries to clone
* git clone https://github.com/ocornut/imgui.git external/imgui
* git clone https://github.com/SFML/imgui-sfml.git external/imgui branch 2.6.x

* the SFML version should be 2.6.1 in order to work with imgui-sfml
* the current make file available in the project is configured to run in windows only


## compile and run using configured the Makefile. 
* make run

### gp-4300_A2 branch

Program Specification

for all entities the position is set to the center position of the shape.

* Player:
    - the player is represented by a shape defined in the config file
    - there should be one and only one player at the time (it should be cleaned after dead).
    - the player mus spawn in the center of the screen at the beginning of the game
      and after it des.
    - the player dies when collide with enemies or enemies debris.
    - the player moves by a speed red from the config file.
    - the available player movements are:
        Up (W key), Left (A key), Down (s key), Right (D key).
    - the player movement is constrained to the dimensions of the screen.
    - the player should shoot a bullet toward the mouse pointer when the left mouse
      button is clicked. the speed, size and lifespan of the bullets is read from the config file.
    - *lifespan should drive the transparency of the object. so it slowly disappear.

* Special Ability
    - a free to implement ability, this special move should be fire by the right click of the mouse
    - multiple entities must be fired
    - a unique graphic should be used
    - a unique game mechanic is introduced via a new component.
    - a cool down timer must be implemented for the special weapon.
    - the properties of the special ability are not indicated in the config file.

* Enemy(s)
    - enemies will spawn in a random location on the screen every x frames specified in the config file.
    - the enemy should be spawn in a valid region into the screen.
    - Enemies should have random number of vertices between a min and max defined in the config file.
    - Enemies will be given a random color upon spawning.
    - Enemies will be given a random speed upon spawning with in a min and max defined in the config.
    - Enemies must bounce when reaching the edges of the screen.
    - when large enemies collides with bullets or player, the are destroyed, and N small enemies spawn in
      it's place, N should be the number of vertices of the original shape, each debris should be 
      the same number of vertices form the original shape. the velocity direction should be 360/N around
      the center of the original shape. no specification for the velocity length.
    
* Score:
    - each time an anemy spawns, it is given a score component of N* 100,
    - small debris will have N*2*100.
    - if a bulled from the player kills an enemy, the game scores is increased by the score for the killed enemies.
    - the score should be displayed wit the font specified by the config file,the position of the score is the 
      top-left corner of the screen.

* Drawing:
    - in the render system, all entities should be given a slow rotation which makes the game look nicer.
    - Any special effects which do not alter game play can be added (sound fx or so.)
    - Any entity with a life spawn is considered alive, it should have its color alpha channel set to a ratio
      from 0 to 1 depending how mutch time has left.
    
* GUI:
    - must use ImGui 
    - list of entities showing id, tag,and position.
    - additional list of entities by tag.
    - each element in the list should have a method or button to delete it.
    - the UI most control the spawning interval time.
    - a button or method to spawn enemies on demand.
    - the ui should be simple to use and visually clean.
    - Note* it could use tabs for clarity:
        - system tab contains:
            - checkbox for stopping movements calculations.
            - checkbox for stopping life spawn calculations.
            - checkbox for stopping automatic spawning.
            - slider to change spawn interval.
            - button for manually spawning.
            - button for stopping rendering.
        - entity manager tab contains:
            - similar to a tree view listing all entities.
            - sub trees grouping entities by tag (bullets,enemies, player, small)
            - all entities list, showing the list of all entities.
            - each entity entry should have a button for destroing it. 
            - each entity will show id(int), str name, tuple(int, int)position, 

* Misc:
    - 'P' key pauses the game.
    - 'ESC' closes the game.

### Config file interpretation:

Each line of the configuration file specifies one of the config settings of the program, starting with the type:
** note that the order of the types can vary.

* Window W H FL FS
    - Declares a SFML Window size (width and height).
    - FL int (frame limit)
    - FS bool/int (0,1) full screen mode 1 or not 0.

* Player SR CR S FR FG FB OR OG OB OT V
    - Shape Radius SR int
    - Collision Radius CR int
    - Speed S float
    - Fill Color FR, FG, FB int, int, int
    - Outline Color OR, OG, OB int, int, int
    - Outline Thickness OT int
    - Shape Vertices V int

* Enemy SR SC SMIN SMAX OR OG OB OT VMin VMax L SI
    - Shape Radius SR int
    - Collision Radius CR int
    - Min Max speed SMIN, SMAX float float
    - Outline Color OR OG OB int, int, int
    - Outline Thickness OT int
    - Min Max Vertices VMIN, VMAX int , int
    - Small Lifespan L int
    - Spawn Interval SI int

* Bullet Sr CR S FR FG FB OR OG OB OT V L
    - Shape Radius SR int
    - Collision Radius CR int
    - Min Max speed SMIN, SMAX float float
    - Fill Color FR, FG, FB int, int, int
    - Outline Color OR, OG, OB int, int, int
    - Outline Thickness OT int
    - Shape Vertices V int
    - Small Lifespan L int

