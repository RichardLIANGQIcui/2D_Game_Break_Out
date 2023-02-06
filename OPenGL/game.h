
#ifndef GAME_H
#define GAME_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "game_level.h"
#include "power_up.h"

using namespace std;

//ʹ��ö���ඨ����Ϸ״̬
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

//����һ����Ϸ�࣬���ڷ�װʵ����Ϸ�ĳ�ʼ��������״̬���������롢��Ⱦ�����ȹ���
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


	//����������������
	Game(unsigned int width, unsigned int height);
	~Game();

	//����һ����ʼ����������ʼ�����е���Ϸ״̬(�����������е���ɫ���������ؿ���
	void Init();
	//��Ϸ����
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