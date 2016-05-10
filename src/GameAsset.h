
#ifndef GAMEASSET_H
#define GAMEASSET_H

#include <iostream>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


/**
 * holds methods/variables which need to be access via inheritance by all asset types
**/
class GameAsset {
 public:
  GameAsset();
  virtual void Draw(GLuint) = 0;
  virtual glm::vec3 getPos() = 0;
  void genBB(glm::vec3, glm::vec3);
  bool collides(glm::vec3, glm::vec3);
  bool collides(GameAsset &a);
  glm::vec3 returnMin();
  glm::vec3 returnMax();

 private:
  glm::vec3 min;
  glm::vec3 max;

};

#endif
