#include "game_level.h"

#include <fstream>
#include <sstream>

#include <string>

using namespace std;

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
	//clear old data
	this->Bricks.clear();
	//load from file
	unsigned int tileCode;
	GameLevel level;
	string line;
	ifstream fstream(file);
	vector<vector<unsigned int>> tileData;

	if (fstream)
	{
		while (getline(fstream, line))
		{
			istringstream sstream(line);
			vector<unsigned int> row;
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
		{
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : this->Bricks)
	{
		if (!tile.Destroyed)
		{
			tile.Draw(renderer);
		}
	}
}

//判断一个游戏是否完成的体现，即所有的可被破坏的砖头都要被破坏才算通关
bool GameLevel::IsCompleted()
{
	for (GameObject& tile : this->Bricks)
	{
		if (!tile.IsSolid && !tile.Destroyed)
		{
			return false;
		}
	}
	return true;
}

void GameLevel::init(vector<vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	//calculate dimensions
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / static_cast<float>(height);

	for (unsigned int y = 0;y < height;y++)
	{
		for (unsigned int x = 0;x < width;x++)
		{
			//check block type from level data
			if (tileData[y][x] == 1)
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2)
				{
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tileData[y][x] == 3)
				{
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tileData[y][x] == 4)
				{
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tileData[y][x] == 5)
				{
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
			}
		}
	}
}