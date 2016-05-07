#include "GameWorld.h"

/** 
* Create cube and diamond assets and assign location and color
**/
GameWorld::GameWorld (ApplicationMode mode) {
  
  //add indiv cubes
  srand(time(NULL));
  asset_manager = std::make_shared<GameAssetManager>(mode);
  asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(16.0, 1.0, 2.0),glm::vec3(1.0, 0.0, 0.0))); //making a cube
  asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(10.0, 1.0, 10.0),glm::vec3(1.0, 1.0, 0.0))); //2nd cube

  //add diamond
  asset_manager->AddAsset(std::make_shared<DiamondAsset>(glm::vec3(12.0, 2.0, 12.0),glm::vec3(0.5, 0.5, 0.5)));

  //generate cube floor
  asset_manager->AddAsset(std::make_shared<FloorAsset>(glm::vec3(10.0, -1.0, 10.0), glm::vec3(0.0, 0.0, randomGen())));
  asset_manager->AddAsset(std::make_shared<FloorAsset>(glm::vec3(10.0, 5.0, 34.0), glm::vec3(0.0, 0.0, randomGen())));

  //generate some stairs
  for (int k = 0; k<20; k++){
	for (int l = 0; l<5; l++){
		asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.5 + k, 0.0 + l, 20.0 + l), glm::vec3(0.0, 0.0, randomGen())));
	}
  } 


  //generate walls
  for (int x = 0; x< 20; x++){
	for(int y = 0; y<5; y++){
		asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0 + x, 0.0 + y, 0.0), glm::vec3(1.0, randomGen()*0.2, randomGen()*0.2))); 
		asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0, 0.0 + y, 0.0 + x), glm::vec3(1.0, randomGen()*0.2, randomGen()*0.2))); 
		asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(20.0, 0.0 + y, 0.0 + x), glm::vec3(1.0, randomGen()*0.2, randomGen()*0.2)));
		asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0 + x, 0.0 + y, 44.0), glm::vec3(1.0, randomGen()*0.2, randomGen()*0.2)));  
	}
  }
   for(int z = 0; z< 25; z++){
	for( int y = 0; y<5; y++){
		asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(0.0, 0.0 + y, 20.0 + z), glm::vec3(1.0, randomGen()*0.2, randomGen()*0.2)));
		asset_manager->AddAsset(std::make_shared<CubeAsset>(glm::vec3(20.0, 0.0 + y, 20.0 + z), glm::vec3(1.0, randomGen()*0.2, randomGen()*0.2)));  
	}
  }

	

  //generate random cubes
  for (int o = 0; o< 10; o++){
	asset_manager->AddAsset(std::make_shared<DiamondAsset>(glm::vec3(randomGen() * 20.0, randomGen()*2.0, randomGen()*20.0), glm::vec3(0.0, randomGen(), 0.5)));
  }

  program_token = asset_manager->returnProgram_token();

  model_loc = glGetUniformLocation(program_token, "Model");
  proj_loc = glGetUniformLocation(program_token, "Projection");
  view_loc = glGetUniformLocation(program_token, "View");
  
}

/**
 * generate random float between 0 and 1
**/
GLfloat GameWorld::randomGen(){
  return static_cast<float>(rand())/static_cast<float>(RAND_MAX);
}

/**
 * fix camera to the center of the screen and limit range
**/
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

/**
 * keyboard input for moving forwards with collision detection
**/
void GameWorld::move_forward(){
	position+=mdirection*speed;	
	//check collision, if collides move back
	if(asset_manager->checkCollision(position)){
		position-=mdirection*speed;
	}
}

/**
 * keyboard input for moving back with collision detection
**/
void GameWorld::move_back(){
	position-=mdirection*speed;
	//check collision, if collides move back
	if(asset_manager->checkCollision(position)){
		position+=mdirection*speed;
	}
}

/**
 * keyboard input for moving left with collision detection
**/
void GameWorld::move_left(){
	position-=vright*speed;
	//check collision, if collides move back
	if(asset_manager->checkCollision(position)){
		position+=vright*speed;
	}
}

/**
 * keyboard input for moving right with collision detection
**/
void GameWorld::move_right(){
	position+=vright*speed;
	//check collision, if collides move back
	if(asset_manager->checkCollision(position)){
		position-=vright*speed;
	}
}

/**
 * keyboard input for jump with controlled jump speed
**/
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

/**
 * checks if jumping is enabled (can only jump once between player touching solid ground)
**/
bool GameWorld::canJump(){
 return jump1;
}

/**
 * loop to draw the world and all assets and qualities
**/
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
