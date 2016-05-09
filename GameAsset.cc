#include "GameAsset.h"

GameAsset::GameAsset(){


}

bool collides(glm::vec3 minB, glm::vec3 maxB){
  if (max.x < minB.x && min.x > maxB.x &&
      max.y < minB.y && min.y > maxB.y &&
      max.z < minB.z && min.z > maxB.z){
		return true;
		}
	return false;
}

void GameAsset::genBB(glm::vec3 p){

  min = p - glm::vec3(0.5, 0.5, 0.5);
  max = p + glm::vec3(0.5, 0.5, 0.5);

}
