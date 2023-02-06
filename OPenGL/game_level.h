#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include<glm/glm.hpp>

#include"game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

using namespace std;

class GameLevel
{
public:
	//level state
	vector<GameObject> Bricks;
	//constructor
	GameLevel(){}

	//load level from file
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);

	//rebder level
	void Draw(SpriteRenderer& renderer);

	//check if the level is completed
	bool IsCompleted();

private:
	//initialize level from tile data
	void init(vector<vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};


#endif