#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode) {
  asset_manager = std::make_shared<GameAssetManager>(mode);
  asset_manager->AddAsset(std::make_shared<CubeAsset>());
  program_token = asset_manager->returnProgram_token();

  model_loc = glGetUniformLocation(program_token, "Model");
  proj_loc = glGetUniformLocation(program_token, "Projection");
  view_loc = glGetUniformLocation(program_token, "View");
  
}

void GameWorld::set_camera(GLfloat x, GLfloat y){
	camerax-= x;
	cameray-= y;
}
void GameWorld::move_forward(){}
void GameWorld::move_back(){}
void GameWorld::move_left(){}
void GameWorld::move_right(){}

void GameWorld::Draw() {
   //where camera is looking
   glm::vec3 direction(
	cos(cameray) * sin(camerax),
	sin(cameray),
	cos(cameray) * cos(camerax)
	);

   mdirection = glm::vec3(
	cos(cameray) * sin(camerax),
	0,
	cos(cameray) * cos(camerax)
	);

   vright = glm::vec3(
	sin(camerax - 3.14f/2.0f),
	0,
	cos(camerax - 3.14f/2.0f)
	);

   //camera perspec
   glm::vec3 vup = glm::cross (vright, direction);


  //set up stuff to send to shader/ added 8/12	
// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
// Camera matrix
glm::mat4 View       = glm::lookAt( //camera angle and location to send to shader
			position, //location
			position + direction, //location + camera angle
			vup //camera perspective
				);
// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 Model      = glm::mat4(1.0f);
// Our ModelViewProjection : multiplication of our 3 matrices
glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

//send Projection, view and model to shader (new stuff)
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &Projection[0][0]);
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, &View[0][0]);
  glUniformMatrix4fv(model_loc, 1, GL_FALSE, &Model[0][0]);

  asset_manager->Draw();
}
