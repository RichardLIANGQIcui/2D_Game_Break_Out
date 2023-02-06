
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

using namespace std;

class ResourceManager
{
public:
	//resource storage
	static map<string, Shader> Shaders;
	static map<string, Texture2D> Textures;

	//loads a shader program from file
	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, string name);

	//retrives a stored shader
	static Shader GetShader(string name);

	//load a texture from file
	static Texture2D LoadTexture(const char* file, bool alpha, string name);

	//retrives a stored texture
	static Texture2D GetTexture(string name);

	//de-alloccates all loaded resources
	static void Clear();

private:
	ResourceManager(){}

	//loads and generates a shader from file
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	//loads a single texture from file
	static Texture2D loadTextureFromFile(const char* file, bool alpha);

};

#endif