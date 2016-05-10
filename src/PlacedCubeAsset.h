#ifndef PLACEDCUBEASSET_H
#define PLACEDCUBEASSET_H

#include <vector>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameAsset.h"




class PlacedCubeAsset : public GameAsset {
 public:
  //create cube, pass postion vector and color vector
  PlacedCubeAsset(glm::vec3,glm::vec3);
  ~PlacedCubeAsset();
  virtual glm::vec3 getPos();
  virtual void Draw(GLuint);

 private:
  GLuint element_buffer_length;
  GLuint vertex_buffer_token, element_buffer_token;
  GLuint colorbuffer;
  glm::vec3 pos;
  glm::mat4 anim;
  GLfloat angle;

};

#endif // CUBEASSET_H
