
#ifndef GAMEASSET_H
#define GAMEASSET_H

#include <iostream>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <boost/uuid/uuid.hpp> //uuid class
#include <boost/uuid/uuid_generators.hpp> //generators
#include <boost/uuid/uuid_io.hpp> //streaming operators

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
