# cpp_game_programming
SFML and ImGui 2d game engine implementing the Entity Component System (ECS)

## adding ImGui extenral libraries to clone
* git clone https://github.com/ocornut/imgui.git external/imgui
* git clone https://github.com/SFML/imgui-sfml.git external/imgui branch 2.6.x

* the SFML version should be 2.6.1 in order to work with imgui-sfml
* the current make file available in the project is configured to run in windows only


## compile and run using configured the Makefile. 
* make run

### gp-4300_A3 branch

----------------------------------------------------------------
Program Specification
----------------------------------------------------------------
the game must have the following features:

Assets:
- entities in the game will be rendered using various Textures and Animations called assets and fonts
- Assets are loaded once at the beginning of the program and stored in the asset class, which is stored by the game engine Class.
- All assets are defined in the assets.txt with the syntax defined below.
None: 
- all entity position denote the center of their rectangular sprite
It also denotes the center of the bounding box, if it has one.
This is set via sprite.setOrigin() in the Animation class constructor.

Player: 
- the player Entity in the game is represented by MegaMan, which has several different Animations: Stand Run, and Air.
  you most determine which state the player is currently in and assign the correct Animation.

- the player moves with the following controls:
  left: A key, Right: D key Jump W key, Shoot: space key.
- the player can move left, right or shoot at any time during the game, this means the player can move left right while in the air.
- the player can only jump if it is currently standing on the a tile.
- if the jump key is held, the player should not continuously jump, but instead it should only jump once per button press. if the player lets go off the jump key
  (mid-jump) it should start falling back down immediately.
- if the player moves left or right the player's sprite will face in that direction until the other direction has been pressed.
- bullets shot by the player travel in the direction the player is facing.
- the player does not collide with 'Dec'(decoration) entities in the level
- if the players falls bellow the bottom of the screen they re-spawn at the start.
- the player should have a gravity component which constantly accelerates it downward ot the screen until it collides with a tile.
- the player has a maximum speed specified in the level file.
- the player will be given a CBoundingBox of a size specified in the level file.
- the player's sprite and bounding box are centered on the player's position.

Animations:
- Animation are implemented by storing multiples frames inside a texture
- the Animation class handles frame advancement based on animation speed
- you need to implement Animation::hasEnded() which returns true if an animation has finished its last frame, false otherwise.
- animation can be repeating (loop forever) or not-repeating (play once)
- Any entity with a non-repeating animation should be destroyed once its animation's hasEnded() returns true (has finished one cycle).  

Decorations Entities:
- Decorations ("Dec") are simply drawn to the screen, do not interact with any other entities in the game in a any way.
- decorations can be given any animation in the game but the should be reserved for things like clouds bushes etc.

Tiles:
- Tiles are Entities that define the level geometry and interact with players
- tiles can be given a CBoundingBox equal to the size of the animation
  tile->get<CAnimation>().animation.getSize()
- the current animation displayed for a tile can be retrieved with:
  tile->get<CAnimation>().animation.getName()
- tiles have different behavior depending on which Animation the are given.

Brick Tiles:
- bricks tiles are given the brick Animation
- when a brick tile collides with a bullet, or is hit by a player from below:
  - its animation should changes to Explosion non repeat.
  - should be destroyed when the animation finishes.
  - its CBoundingBox component should be removed.

Question Tiles:
- questions are given the "Question" animation when created
- when a Question tile is hit gy a player from below:
  - it's animation changes to the darker Question2 animation
  - a temporary lifespan entity "Coin" should appear for 30 frames 64 pixels above.

GUI:
- Display options to turn off  each system independently.
- fro each entity in the game the gui ust list the id tag adn position of the entity.
- must display a list of all entities as well as list of entities by tag with a button to destroy each one.
- button to show tiles, show bonding box and enable textures.

Rendering:
- entities are render in the order the were stored in the entityManager.

Bonus:
- special effects
- special weapon

Misc:
- P key should pause the game
- T toggles drawing textures.
- C toggles drawing bounding boxes
- G toggle drawing of the grid
- ESC should go back to the main menu or quit if on the main menu.