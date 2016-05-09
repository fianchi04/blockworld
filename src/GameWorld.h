#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <memory>

#include <GL/gl.h>
//added 8/12
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <stdlib.h> 
#include <time.h>  

#include "common.h"
#include "GameAssetManager.h"
#include "CubeAsset.h"
#include "DiamondAsset.h"
#include "FloorAsset.h"

/**
 * GameWorld allows us to separate the management of the game world from the
 * nuts and bolts of game loop initialisation.  The GameWorld currently has
 * a very simplified scene graph consisiting of a single GameAssetManager.
 */
class GameWorld {
 public:
  /**
   * We thread the ApplicationMode through the GameWorld ss we want to read it
   * in from the user.  Threading the state through the various function calls
   * is preferable (in this case) to having some kind of global state.
   */
  GameWorld(ApplicationMode);

  /**
   * Calling Draw() will draw the entire world.
   */
  void Draw();
//public functions for movement
  void move_forward();
  void move_back();
  void move_left();
  void move_right();
  void add_cube();
  void add_diamond();
  void move_jump(GLfloat);
  bool canJump();
//camera data
  void set_camera(GLfloat, GLfloat); //x,y
  int jumplength = 0;

 private:
  GLfloat randomGen();
  bool jump1 = false;
  std::shared_ptr<GameAssetManager> asset_manager;
  //vectors
  glm::vec3 position = glm::vec3(3,10,1);
  glm::vec3 mdirection; //walk direction
  glm::vec3 vright; //walk direction sideways
  glm::vec3 direction;

  GLuint program_token;
//shader data locations
  GLuint model_loc;
  GLuint proj_loc;
  GLuint view_loc;
  GLfloat jumpspeed = 0.0;
  GLfloat speed = 0.1; //movement speed
  GLfloat camerax = 1.0; //pos of mouse camera
  GLfloat cameray = 1.0; 
};
#endif // GAMEWORLD_H
