#include "CubeAsset.h"

CubeAsset::CubeAsset(glm::vec3 p, glm::vec3 c) {
	pos = p;

	//draw cube
  // model coordinates, origin at centre.
  GLfloat vertex_buffer [] {
    -0.5f + p.x, -0.5f + p.y, -0.5f + p.z //0
    , -0.5f + p.x,  0.5f + p.y, -0.5f + p.z //1
    ,  0.5f + p.x, -0.5f + p.y, -0.5f + p.z //2
    ,  0.5f + p.x,  0.5f + p.y, -0.5f + p.z //3
	//4 more lines for 4 more vertex points
    ,0.5f + p.x, -0.5f + p.y, 0.5f + p.z //4
	, 0.5f + p.x, 0.5f + p.y, 0.5f + p.z //5
	,-0.5f + p.x, -0.5f + p.y, 0.5f + p.z //6
	,-0.5f + p.x, 0.5f + p.y, 0.5f + p.z //7
  }; 

  GLfloat color = 0.2f;

  //set cube color to rgb values in c array
  GLfloat color_buffer[] = {
	0.0f+c.x, 0.0f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.0f+c.z,
	0.0f+c.x, 0.0f+c.y, 0.0f+c.z,
  };

  element_buffer_length = 36;//total points (below)
  GLuint element_buffer []  {
	//each square is made up of 2xtri, connectors of 3 vertex points
    0, 1, 2
    , 1, 3, 2 
	//for 2,3,4,5
	,2, 3, 4
	,3, 5, 4
	//for 1, 3, 5, 7
	,1, 7, 3
	, 7, 5, 3
	//for 0,2, 6 , 4
	, 0, 6, 4
	, 0, 2, 4
	//for 0, 1, 7, 6
	,0, 6, 7
	,0, 1, 7
	//for 6, 4, 5, 7
	,6 ,4 ,5
	,6 ,7, 5
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
}

CubeAsset::~CubeAsset() {
}

glm::vec3 CubeAsset::getPos(){
	return pos;
}

#ifdef DEBUG
#define checkGLError() checkError(__FILE__, __LINE__)
#else
// define symbol to be nothing
#define checkGLError()
#endif

void checkError(std::string file, int line) {
  GLenum gl_error = glGetError();
  if(GL_NO_ERROR != gl_error) {
    std::cerr << "GL error in " << file << " at line " << line << " error: " << gl_error << std::endl;
    exit(-1);
  }
}

void CubeAsset::Draw(GLuint program_token) {
  if(!glIsProgram(program_token)) {
    std::cerr << "Drawing Cube with invalid program" << std::endl;
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
