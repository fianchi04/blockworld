#include "CubeAsset.h"

CubeAsset::CubeAsset() {
	

	//draw cube
  // model coordinates, origin at centre.
  GLfloat vertex_buffer [] {
    -0.5, -0.5, -0.5 //0
    , -0.5,  0.5, -0.5 //1
    ,  0.5, -0.5, -0.5 //2
    ,  0.5,  0.5, -0.5 //3
	//4 more lines for 4 more vertex points
    ,0.5, -0.5, 0.5 //4
	, 0.5, 0.5, 0.5 //5
	,-0.5, -0.5, 0.5 //6
	,-0.5, 0.5, 0.5 //7
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
}

CubeAsset::~CubeAsset() {
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

//set up stuff to send to shader/ added 8/12
	
// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
// Camera matrix
glm::mat4 View       = glm::lookAt(
					glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
					glm::vec3(0,0,0), // and looks at the origin
					glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
          		   );
// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 Model      = glm::mat4(1.0f);
// Our ModelViewProjection : multiplication of our 3 matrices
glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

//send Projection, view and model to shader (new stuff)
  glUniformMatrix4fv(0, 1, GL_FALSE, &Projection[0][0]);
  glUniformMatrix4fv(1, 1, GL_FALSE, &View[0][0]);
  glUniformMatrix4fv(2, 1, GL_FALSE, &Model[0][0]);



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
