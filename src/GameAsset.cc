#include "GameAsset.h"

GameAsset::GameAsset(){


}

/**
 * check if AABBs are intersecting
**/
bool GameAsset::collides(glm::vec3 minB, glm::vec3 maxB){
  if (max.x > minB.x && min.x < maxB.x &&
      max.y > minB.y && min.y < maxB.y &&
      max.z > minB.z && min.z < maxB.z){
		return true;
		}
	return false;
}

bool GameAsset::collides(GameAsset &a){
   return collides(a.returnMin(), a.returnMax());
}

glm::vec3 GameAsset::returnMin(){
	return min;
}
glm::vec3 GameAsset::returnMax(){
	return max;
}

/**
 * generate min and max coords for AABBs
**/
void GameAsset::genBB(glm::vec3 p, glm::vec3 param){

  min = p - glm::vec3(param.x, param.y, param.z);
  max = p + glm::vec3(param.x, param.y, param.z);

}
/*
void sweep_and_prune(){
   boost::uuids::uuid u;
   for (boost::uuids::uuid::const_iterator it=u.begin(); it!=u.end(); ++it) {
        boost::uuids::uuid::value_type v = *it;
   }*/


