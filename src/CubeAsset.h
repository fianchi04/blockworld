#ifndef CUBEASSET_H
#define CUBEASSET_H

#include <vector>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameAsset.h"

class CubeAsset : public GameAsset {
 public:
  //create cube, pass postion vector and color vector
  CubeAsset(glm::vec3,glm::vec3);
  ~CubeAsset();
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
