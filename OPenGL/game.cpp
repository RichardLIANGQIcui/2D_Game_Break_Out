#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "text_renderer.h"

#include <irrKlang.h>
#include <algorithm>
#include <sstream>
#include <iostream>

using namespace irrklang;
using namespace std;


//Game_related State data
BallObject* Ball;
SpriteRenderer* Renderer;
GameObject* Player;
ParticleGenerator* Particles;
PostProcessor* Effects;
ISoundEngine* SoundEngine = createIrrKlangDevice();
TextRenderer* Text;

float ShakeTime = 0.0f;


Game::Game(unsigned int width, unsigned int height) :State(GAME_MENU), Keys(), Width(width), Height(height),Level(0),Lives(3)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	delete Text;
	SoundEngine->drop();
}

void Game::Init()
{
	//load shaders
	ResourceManager::LoadShader("D:/LearnOpenGl/BreakOut/shaders/sprite.vs", "D:/LearnOpenGl/BreakOut/shaders/sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("D:/LearnOpenGl/BreakOut/shaders/particle.vs", "D:/LearnOpenGl/BreakOut/shaders/particle.fs", nullptr, "particle");
	ResourceManager::LoadShader("D:/LearnOpenGl/BreakOut/shaders/post_processing.vs", "D:/LearnOpenGl/BreakOut/shaders/post_processing.fs", nullptr, "post_processing");
	//configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

	//load texture
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/background.jpg",false,"background");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/block.png", false, "block");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/particle.png", true, "particle");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/tools/powerup_speed.png", true, "powerup_speed");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/tools/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/tools/powerup_increase.png", true, "powerup_increase");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/tools/powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/tools/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::LoadTexture("D:/LearnOpenGl/BreakOut/textures/tools/powerup_passthrough.png", true, "powerup_passthrough");

	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
	Effects = new PostProcessor(ResourceManager::GetShader("post_processing"), this->Width, this->Height);
	//Text
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("D:/LearnOpenGl/BreakOut/LearnOpenGL-master/resources/fonts/OCRAEXT.TTF", 24);
	//load level
	GameLevel one;
	one.Load("D:/LearnOpenGl/BreakOut/levels/one.lvl",this->Width,this->Height/2);
	GameLevel two;
	two.Load("D:/LearnOpenGl/BreakOut/levels/two.lvl", this->Width, this->Height / 2);
	GameLevel three;
	three.Load("D:/LearnOpenGl/BreakOut/levels/three.lvl", this->Width, this->Height / 2);
	GameLevel four;
	four.Load("D:/LearnOpenGl/BreakOut/levels/four.lvl", this->Width, this->Height / 2);

	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;

	//configue game objects
	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	//configue ball
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));

	//audio
	SoundEngine->play2D("D:/LearnOpenGl/BreakOut/music/Alan Walker - All Falls Down.wav", true);


}

void Game::Updata(float dt)
{
	Ball->Move(dt, this->Width);

	this->DoCollisions();

	Particles->Updata(dt, *Ball, 2,glm::vec2(Ball->Radius/2.0f));

	this->UpdataPowerUps(dt);

	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
		{
			Effects->Shake = false;
		}
	}

	//check loss condition
	if (Ball->Position.y >= this->Height)
	{
		--this->Lives;
		if (this->Lives == 0)
		{
			this->ResetLevel();
			this->State = GAME_MENU;
		}
		this->ResetPlayer();
	}

	// check win condition
	if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Chaos = true;
		this->State = GAME_WIN;
	}
}

void Game::ProcessInput(float dt)
{
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = true;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;
			//this->Level = (this->Level - 1) % 4;
			this->KeysProcessed[GLFW_KEY_S] = true;
		}
	}
	if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
			Effects->Chaos = false;
			this->State = GAME_MENU;
		}
	}
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		// move playerboard
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0.0f)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		Effects->BeginRender();

		//draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width,this->Height)
			, 0.0f);

		//draw level
		this->Levels[this->Level].Draw(*Renderer);

		//draw player
		Player->Draw(*Renderer);

		//draw  PowerUp
		for (PowerUp& powerUp : this->PowerUps)
		{
			if (!powerUp.Destroyed)
			{
				powerUp.Draw(*Renderer);
			}
		}

		//draw particles
		Particles->Draw();

		//draw ball
		Ball->Draw(*Renderer);

		Effects->EndRender();

		Effects->Render(glfwGetTime());

		//render Text
		std::stringstream ss; ss << this->Lives;
		Text->RenderText("LIANGQICUI Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	if (this->State == GAME_MENU)
	{
		Text->RenderText("Maded by LIANGQICUI", 250.0f, this->Height / 2.0f-20.0f, 1.0f);
		Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
	}
	if (this->State == GAME_WIN)
	{
		Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	}
	
}

void Game::ResetLevel()
{
	if (this->Level == 0)
	{
		this->Levels[0].Load("D:/LearnOpenGl/BreakOut/levels/one.lvl", this->Width, this->Height / 2);
	}
	else if (this->Level == 1)
	{
		this->Levels[1].Load("D:/LearnOpenGl/BreakOut/levels/two.lvl", this->Width, this->Height / 2);
	}
	else if (this->Level == 2)
	{
		this->Levels[2].Load("D:/LearnOpenGl/BreakOut/levels/three.lvl", this->Width, this->Height / 2);
	}
	else if (this->Level == 3)
	{
		this->Levels[3].Load("D:/LearnOpenGl/BreakOut/levels/four.lvl", this->Width, this->Height / 2);
	}
	this->Lives = 3;
}

void Game::ResetPlayer()
{
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

	Effects->Chaos = Effects->Confuse = false;
	Ball->PassThrough =Ball->Sticky= false;
	Player->Color = glm::vec3(1.0f);
	Ball->Color = glm::vec3(1.0f);
}

//获取球的方向的原理就是，将球的位移矢量与上下左右四个向量进行点乘，得到值最大的方向即为球的方向
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f,1.0f),//UP
		glm::vec2(1.0f,0.0f),//RIGHT
		glm::vec2(0.0f,-1.0f),//DOWN
		glm::vec2(-1.0f,0.0f)//LEFT
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0;i < 4;i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

//道具功能实现
bool ShouldSpawn(unsigned int chance)
{
	unsigned int random = rand() % chance;
	return random == 0;
}

void Game::SpawnPowerUps(GameObject& block)
{
	if (ShouldSpawn(75))
	{
		this->PowerUps.push_back(
			PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position,
				ResourceManager::GetTexture("powerup_speed")));
	}
	if (ShouldSpawn(75))
	{
		this->PowerUps.push_back(
			PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position,
				ResourceManager::GetTexture("powerup_sticky")));
	}
	if (ShouldSpawn(75))
	{
		this->PowerUps.push_back(
			PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position,
				ResourceManager::GetTexture("powerup_passthrough")));
	}
	if (ShouldSpawn(75))
	{
		this->PowerUps.push_back(
			PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.Position,
				ResourceManager::GetTexture("powerup_increase")));
	}
	if (ShouldSpawn(15))
	{
		this->PowerUps.push_back(
			PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position,
				ResourceManager::GetTexture("powerup_confuse")));
	}
	if (ShouldSpawn(15))
	{
		this->PowerUps.push_back(
			PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position,
				ResourceManager::GetTexture("powerup_chaos")));
	}
}

//激活道具,启用道具功能
void ActivatePowerUp(PowerUp& powerUp)
{
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = true;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->PassThrough = true;
		Player->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
		{
			Effects->Confuse = true;
		}
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
		{
			Effects->Chaos = true;
		}
	}
}

bool IsOtherPowerUpActive(vector<PowerUp>& powerUps, string type)
{
	for (const PowerUp& powerUp : powerUps)
	{
		if (powerUp.Activated)
		{
			if (powerUp.Type == type)
			{
				return true;
			}
		}
	}
	return false;
}
//更新道具的变化状态,对失效道具进行移除
void Game::UpdataPowerUps(float dt)
{
	for (PowerUp& powerUp : this->PowerUps)
	{
		//道具位置变化
		powerUp.Position += powerUp.Velocity * dt;

		if (powerUp.Activated)
		{
			//道具的持续时间要被减少
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f)
			{
				//将道具身上的效果移除
				powerUp.Activated = false;
				//不同道具类型不一样，所以要分类讨论
				if (powerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{
						Ball->Sticky = false;
						Player->Color = glm::vec3(1.0f);
					}
				}
				if (powerUp.Type == "pass-through")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
					{
						Ball->PassThrough = false;
						Player->Color = glm::vec3(1.0f);
					}
				}
				if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{
						Effects->Confuse = false;
					}
				}
				if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{
						Effects->Chaos = false;
					}
				}
			}
		}
	}
	this->PowerUps.erase(remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp& powerUp){ return powerUp.Destroyed && powerUp.Activated; }), this->PowerUps.end());

}

//碰撞实现
bool CheckCollision(GameObject& one, GameObject& two)
{
	//collision x-axis
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;

	//collision y-axis
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;

	return collisionX && collisionY;
}



Collision CheckCollision(BallObject& one, GameObject& two)
{
	//检测圆球与砖块碰撞，其原理是通过找离球心最近的砖块上的点，然后将改点与圆心连接，其矢量大小再与圆的半径作对比
	
	//get center point circle first
	glm::vec2 center(one.Position + one.Radius);

	//calculate AABB info
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);

	//get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	//get the cloest dot of circle
	glm::vec2 cloest = aabb_center + clamped;

	difference = cloest - center;

	if (glm::length(difference) < one.Radius)
	{
		return make_tuple(true, VectorDirection(difference), difference);
	}
	else
	{
		return make_tuple(false, UP, glm::vec2(0.0f,0.0f));
	}
}


void Game::DoCollisions()
{
	//1.对场景中的砖块进行碰撞检测
	for (GameObject& box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			
			if (get<0>(collision))
			{
				if (!box.IsSolid)
				{
					box.Destroyed = true;
					this->SpawnPowerUps(box);
					SoundEngine->play2D("D:/LearnOpenGl/BreakOut/music/bleep.wav", false);
				}
				else
				{
					ShakeTime = 1.0f;
					Effects->Shake = true;
					SoundEngine->play2D("D:/LearnOpenGl/BreakOut/music/solid.wav", false);
				}

				//if box is solid,collision resolution
				Direction dir = get<1>(collision);
				glm::vec2 diff_vector = get<2>(collision);
				if (!(Ball->PassThrough && !box.IsSolid))
				{
					if (dir == LEFT || dir == RIGHT)
					{
						Ball->Velocity.x = -Ball->Velocity.x;

						float penetration = Ball->Radius - abs(diff_vector.x);
						if (dir == LEFT)
						{
							Ball->Position.x += penetration;
						}
						else
						{
							Ball->Position.x -= penetration;
						}
					}
					if (dir == UP || dir == DOWN)
					{
						Ball->Velocity.y = -Ball->Velocity.y;

						float penetration = Ball->Radius - abs(diff_vector.y);
						if (dir == UP)
						{
							Ball->Position.y -= penetration;
						}
						else
						{
							Ball->Position.y += penetration;
						}
					}
				}
			}
		}

		//2.check collisions for powerup
		for (PowerUp& powerUp : this->PowerUps)
		{
			if (!powerUp.Destroyed)
			{
				if (powerUp.Position.y >= this->Height)
				{
					powerUp.Destroyed = true;
				}

				if (CheckCollision(*Player, powerUp))
				{
					ActivatePowerUp(powerUp);
					powerUp.Destroyed = true;
					powerUp.Activated = true;
					SoundEngine->play2D("D:/LearnOpenGl/BreakOut/music/powerup.wav", false);
				}
			}
		}

		//3.check collisions for playerpad
		Collision result = CheckCollision(*Ball, *Player);
		if (!Ball->Stuck && get<0>(result))
		{
			float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
			//球的中心位置到中心点的距离
			float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
			float percentage = distance / (Player->Size.x / 2.0f);

			float strength = 2.0f;
			glm::vec2 oldVelocity = Ball->Velocity;

			Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * strength * percentage;
			Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);

			//keep speed consistent over both axes
			Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);

			Ball->Stuck = Ball->Sticky;

			SoundEngine->play2D("D:/LearnOpenGl/BreakOut/music/bleep.wav", false);

		}
	}



}

