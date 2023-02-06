
#ifndef GAME_H
#define GAME_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "game_level.h"
#include "power_up.h"

using namespace std;

//使用枚举类定义游戏状态
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

//the four possible directions
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

//Dedfines a collision typedef that represents collision data
typedef tuple<bool, Direction, glm::vec2> Collision;

//Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
//Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

const float BALL_RADIUS = 12.5f;

//设置一个游戏类，用于封装实现游戏的初始化、更新状态、处理输入、渲染场景等功能
class Game
{
public:
	//game state
	GameState    State;
	bool         Keys[1024];
	bool                    KeysProcessed[1024];
	unsigned int Width, Height;
	vector<GameLevel> Levels;
	unsigned int Level;
	vector<PowerUp> PowerUps;
	unsigned int Lives;


	//构造器和析构函数
	Game(unsigned int width, unsigned int height);
	~Game();

	//定义一个初始化函数，初始化所有的游戏状态(包括加载所有的着色器、纹理、关卡）
	void Init();
	//游戏处理
	void ProcessInput(float dt);
	void Updata(float dt);
	void Render();
	void DoCollisions();

	void ResetLevel();
	void ResetPlayer();

	void SpawnPowerUps(GameObject& block);
	void UpdataPowerUps(float dt);

};

#endif