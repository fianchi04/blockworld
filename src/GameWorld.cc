#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode) {
  asset_manager = std::make_shared<GameAssetManager>(mode);
  asset_manager->AddAsset(std::make_shared<CubeAsset>());
  program_token = asset_manager->returnProgram_token();

  model_loc = glGetUniformLocation(program_token, "Model");
  proj_loc = glGetUniformLocation(program_token, "Projection");
  view_loc = glGetUniformLocation(program_token, "View");
  
}

void GameWorld::Draw() {

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
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, &Projection[0][0]);
  glUniformMatrix4fv(view_loc, 1, GL_FALSE, &View[0][0]);
  glUniformMatrix4fv(model_loc, 1, GL_FALSE, &Model[0][0]);

  asset_manager->Draw();
}
