#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D
{
public:
	unsigned int ID;

	//texture image dinebsions
	unsigned int Width, Height;

	//texture Format
	unsigned int Internal_Format;
	unsigned int Image_Format;

	//texture configuration
	unsigned int Wrap_S;
	unsigned int Wrap_T;
	unsigned int Filter_Min;
	unsigned int Filter_Max;

	Texture2D();

	//generate texture from image data
	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	//binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;


};
#endif