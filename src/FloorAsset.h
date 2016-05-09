#ifndef FLOORASSET_H
#define FLOORASSET_H

#include <vector>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GameAsset.h"

class FloorAsset : public GameAsset {
 public:
  //create floor, pass postion vector and color vector
  FloorAsset(glm::vec3,glm::vec3);
  ~FloorAsset();
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

#endif // FLOORASSET_H
