#include "DiamondAsset.h"

/**
 * Draw diamond based on location of vertices relative to 0, assign colour based on RGB
**/
DiamondAsset::DiamondAsset(glm::vec3 p, glm::vec3 c) {
	pos = p;
	//draw diamond
	//model coords x y z, 00 at centre
  GLfloat vertex_buffer[]{
	0.0f + p.x, 0.5f + p.y, 0.0f + p.z,
	0.5f + p.x, 0.0f + p.y, -0.5f + p.z,
	0.5f + p.x, 0.0f + p.y, 0.5f + p.z,
	-0.5f + p.x, 0.0f + p.y, 0.5f + p.z,
	-0.5f + p.x, 0.0f + p.y, -0.5f + p.z,
	0.0f + p.x, -0.5f + p.y, 0.0f + p.z,
  }; 


  GLfloat color = 0.2f;

  //set cube color to rgb values in c array
  GLfloat color_buffer[] = {
	0.1f+c.x, 0.0f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.1f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.1f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.1f+c.z,
	0.0f+c.x, 0.1f+c.y, 0.0f+c.z,
	0.1f+c.x, 0.1f+c.y, 0.0f+c.z,
  };

  element_buffer_length = 24;//total points (below)
  GLuint element_buffer []  {
	//each face is a triangle made from 3 points
	//top half
	0, 3, 2,
	0, 2, 1,
	0, 1, 4,
	0, 4, 3,
	//bottom half
	5, 2, 1,
	5, 1, 4,
	5, 4, 3,
	5, 3, 2,

	
  };

  // Transfer buffers to the GPU
  //

  // create buffer
  glGenBuffers(1, &vertex_buffer_token);

  // immediately bind the buffer and transfer the data
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_token);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, vertex_buffer, GL_STATIC_DRAW);

  glGenBuffers(1, &element_buffer_token);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_token);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * element_buffer_length, element_buffer, GL_STATIC_DRAW);

  //color buffer
  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, color_buffer, GL_STATIC_DRAW);

 //vector stores parameter/2 of shape (from centre)
  glm::vec3 param = glm::vec3(0.5, 0.5, 0.5);
  //generate min and max coordinates for aabb collision detection
  genBB(p, param);


}

DiamondAsset::~DiamondAsset() {
}

/**
 * return position of diamond
 **/
glm::vec3 DiamondAsset::getPos(){
	return pos;
}

#ifdef DEBUG
#define checkGLError() checkError(__FILE__, __LINE__)
#else
// define symbol to be nothing
#define checkGLError()
#endif


/**
 * check for error in drawing diamond
**/
void DiamondAsset::Draw(GLuint program_token) {
  if(!glIsProgram(program_token)) {
    std::cerr << "Drawing Diamond with invalid program" << std::endl;
    return;
  }


  GLint validation_ok;
  glValidateProgram(program_token);
  glGetProgramiv(program_token, GL_VALIDATE_STATUS, &validation_ok);
  if(!validation_ok) {
    GLint maxLength = 0;
    glGetProgramiv(program_token, GL_INFO_LOG_LENGTH, &maxLength);

    //The maxLength includes the NULL character
    std::vector<char> errorLog(maxLength);
    glGetProgramInfoLog(program_token, maxLength, &maxLength, &errorLog[0]);

    std::cerr << "Invalid program " << program_token << " with error code " << validation_ok << std::endl;
    for(auto c: errorLog) {
      std::cerr << c;
    }
    exit(-1);
  }

  GLuint position_attrib = glGetAttribLocation(program_token, "position");
  GLuint color_attrib = glGetAttribLocation(program_token, "inputcolor");
  checkGLError();

  GLuint anim_location = glGetUniformLocation(program_token, "Anim");
  //angle change every frame
   angle += 0.01f;

  anim = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(pos.x, pos.y, pos.z));  


 
  glUniformMatrix4fv(anim_location, 1, GL_FALSE, &anim[0][0]);

  glUseProgram(program_token);
  checkGLError();

  // use the previously transferred buffer as the vertex array.  This way
  // we transfer the buffer once -- at construction -- not on every frame.
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_token);
  glVertexAttribPointer(
                        position_attrib,               /* attribute */
                        3,                             /* size */
                        GL_FLOAT,                      /* type */
                        GL_FALSE,                      /* normalized? */
                        0,                             /* stride */
                        (void*)0                       /* array buffer offset */
                        );
  glEnableVertexAttribArray(position_attrib);

  checkGLError();

  //drawing colors of cube to send to shaders
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(color_attrib, 
		3,
		GL_FLOAT,  
		GL_FALSE,
		0,
		(void*)0
		);
  glEnableVertexAttribArray(color_attrib);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_token);
  glDrawElements(
                 GL_TRIANGLES,
                 element_buffer_length,
                 GL_UNSIGNED_INT,
                 (GLvoid*) 0
                 );

  checkGLError();

  glDisableVertexAttribArray(position_attrib);
}
