#include "shader.h"
#include <iostream>

Shader& Shader::Use()
{
	//激活当前的着色器对象，便于后续一系列的操作是针对该着色器上进行的
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	unsigned int sVertex, sFragment, gShader;

	//vertex Shader
	//1、使用glCreateShader()创建一个容纳shader对象的容器
	sVertex = glCreateShader(GL_VERTEX_SHADER);

	//2、往创建好的容器中添加源代码，利用glShaderSource()函数
	glShaderSource(sVertex, 1, &vertexSource, NULL);

	//3、对容器内的源码进行编译
	glCompileShader(sVertex);

	//4、检查是否编译错误
	checkCompileErrors(sVertex, "VERTEX");

	//同理。创建并编译片段着色器
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");

	//创建并编译几何着色器
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}

	//有了着色器后，要创建着色器程序
	this->ID = glCreateProgram();
	glAttachShader(this->ID, sVertex);
	glAttachShader(this->ID, sFragment);
	if (geometrySource != nullptr)
	{
		glAttachShader(this->ID, gShader);
	}
	//将顶点着色器和片段着色器链接
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");

	//连接后可删除顶点和片段着色器
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
	{
		glDeleteShader(gShader);
	}


}

void Shader::SetFloat(const char* name, float value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInteger(const char* name, int value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform2f(glGetUniformLocation(this->ID, name), x,y);
}

void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::SetVector3f(const char* name, float x, float y,float z, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform3f(glGetUniformLocation(this->ID, name), x, y,z);
}

void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y,value.z);
}

void Shader::SetVector4f(const char* name, float x, float y, float z,float w, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z,w);
}

void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z,value.w);
}

void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1,false,glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned int object, string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		//用于检测着色器是否编译成功，第一个参数表示要查询的对象，第二个参数则表示要查询对象的内容，第三参数接受函数返回的结果
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			cout<<"| ERROR::SHADER:Compile_time error:Type:"<<type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}