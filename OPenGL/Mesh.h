//#ifndef MESH_H
//#define MESH_H
//
//#include <glad/glad.h>
//
//#include<glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "Shader.h"
//
//#include <string>
//#include <vector>
//using namespace std;
//
//#define MAX_BONE_INFLIENCE 4
//
//struct Vertex {
//	//位置
//	glm::vec3 Position;
//	//法线
//	glm::vec3 Normal;
//	//纹理坐标
//	glm::vec2 TexCoords;
//	//正切
//	glm::vec3 Tangent;
//	//双切
//	glm::vec3 Bitangent;
//	//网格顶点索引
//	int m_BoneIDs[MAX_BONE_INFLIENCE];
//	//网格权重
//	float m_Weights[MAX_BONE_INFLIENCE];
//};
//
//struct Texture
//{
//	unsigned int id;
//	string type;
//	string path;
//};
//
//class Mesh
//{
//public:
//	//网格数据
//	vector<Vertex> vertices;
//	vector<unsigned int> indices;
//	vector<Texture> textures;
//	unsigned int VAO;
//
//	//构造器
//	Mesh(vector<Vertex> vertices, vector<unsigned int>indices, vector<Texture> textures)
//	{
//		this->vertices = vertices;
//		this->indices = indices;
//		this->textures = textures;
//		//初始化网格
//		setupMesh();
//	}
//
//	//渲染网格
//	void Draw(Shader& shader)
//	{
//		//绑定纹理
//		unsigned int diffuseNr = 1;
//		unsigned int specularNr = 1;
//		unsigned int normalNr = 1;
//		unsigned int heightNr = 1;
//		for (unsigned int i = 0;i < textures.size();i++)
//		{
//			//激活纹理单元
//			glActiveTexture(GL_TEXTURE0 + i);
//
//			string number;
//			string name = textures[i].type;
//			if (name == "texture_diffuse")
//			{
//				number = to_string(diffuseNr++);
//			}
//			else if (name == "texture_specular")
//			{
//				number = to_string(specularNr++);
//			}
//			else if (name == "texture_normal")
//			{
//				number = to_string(normalNr++);
//			}
//			else if (name == "texture_height")
//			{
//				number = to_string(heightNr++);
//			}
//
//			//设置采样器绑定纹理单元
//			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
//			//最后绑定纹理
//			glBindTexture(GL_TEXTURE_2D, textures[i].id);
//		}
//
//		//绘制网格
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		glad_glActiveTexture(GL_TEXTURE0);
//
//	}
//	
//private:
//	unsigned int VBO, EBO;
//
//	//初始化所有的缓冲、数组
//	void setupMesh()
//	{
//		//创建缓冲和数组
//		glGenVertexArrays(1, &VAO);
//		glGenBuffers(1, &VBO);
//		glGenBuffers(1, &EBO);
//
//		glBindVertexArray(VAO);
//
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//	
//		//设置顶点属性
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//		glEnableVertexAttribArray(3);
//		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
//		glEnableVertexAttribArray(4);
//		glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
//		glEnableVertexAttribArray(5);
//		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
//		glEnableVertexAttribArray(6);
//	
//		glBindVertexArray(0);
//	}
//
//};
//
//
//
//#endif
