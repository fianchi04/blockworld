#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode) {

  srand(time(NULL));
  asset_manager = std::make_shared<GameAssetManager>(mode);
  asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0, 0.0, 0.0),glm::vec3(1.0, 0.0, 0.0))); //making a cube
  asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(1.0, 1.0, 1.0),glm::vec3(1.0, 1.0, 0.0))); //2nd cube

  //generate cube floor
  for (int i = 0; i<20; i++){
	for (int j = 0; j< 20; j++){
	  asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0 + i, -1.0, 0.0 + j), glm::vec3(randomGen(), 0.0, 0.0)));
	}
  }
  program_token = asset_manager->returnProgram_token();

  model_loc = glGetUniformLocation(program_token, "Model");
  proj_loc = glGetUniformLocation(program_token, "Projection");
  view_loc = glGetUniformLocation(program_token, "View");
  
}
//generate random float between 0 and 1
GLfloat GameWorld::randomGen(){
  return static_cast<float>(rand())/static_cast<float>(RAND_MAX);
}

void GameWorld::set_camera(GLfloat x, GLfloat y){
	camerax-= x;
	cameray-= y;

  //limit on camera range
	if (cameray > 1.5){
		cameray = 1.5;}
	if (cameray < -1.5){
		cameray = -1.5;}
}

//keyboard input

void GameWorld::move_forward(){
	position+=mdirection*speed;	
	//check collision, if collides move back
	if(asset_manager->checkCollision(position)){
		position-=mdirection*speed;
	}
}
void GameWorld::move_back(){
	position-=mdirection*speed;
	//check collision, if collides move back
	if(asset_manager->checkCollision(position)){
		position+=mdirection*speed;
	}
}
void GameWorld::move_left(){
	position-=vright*speed;
	//check collision, if collides move back
	if(asset_manager->checkCollision(position)){
		position+=vright*speed;
	}
}
void GameWorld::move_right(){
	position+=vright*speed;
	//check collision, if collides move back
	if(asset_manager->checkCollision(position)){
		position-=vright*speed;
	}
}


void GameWorld::move_jump(GLfloat speed){
      //if jump has lasted less than 15 updates
      if(jumplength < 10){
      //add passed speed to jumpspeed
      jumpspeed += speed;
      //counter to see how many frames jump has been on for
      jumplength ++;
      }
	//stop jumping too fast
	if(jumpspeed > 2){
		jumpspeed = 2;
       }
      
}

bool GameWorld::canJump(){
 return jump1;
}

void GameWorld::Draw() {
   
   //every update slow jump speed
   jumpspeed -= 0.05;
   //stop jumpspeed going below 0
   if(jumpspeed < 0.0){
      jumpspeed = 0.0;
   }

   //minus gravity + jumpspeed from position y
   position.y -= (0.98-jumpspeed)*speed;
   //if player collides with cube, move back and reset jump counters
   if(asset_manager->checkCollision(position)){
                jump1 = false;
		jumplength = 0;
		position.y+=(0.98-jumpspeed)*speed;
	}
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
