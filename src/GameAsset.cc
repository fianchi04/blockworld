#include "GameAsset.h"

GameAsset::GameAsset(){


}

/**
 * Check if AABBs are intersecting (passed min and max against the min and max of the current asset
**/
bool GameAsset::collides(glm::vec3 minB, glm::vec3 maxB){
  if (max.x > minB.x && min.x < maxB.x &&
      max.y > minB.y && min.y < maxB.y &&
      max.z > minB.z && min.z < maxB.z){
		return true;
		}
	return false;
}

/**
 * Receives a reference to an asset, calls returnMin() and returnMax() and passes these to the AABB collision
 *  detection- needed for asset->asset collision
**/
bool GameAsset::collides(GameAsset &a){
   return collides(a.returnMin(), a.returnMax());
}

/**
 * returns the min coordinate vector of an asset
**/
glm::vec3 GameAsset::returnMin(){
	return min;
}
/**
 * returns the max coordinate vector of an asset
**/
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



