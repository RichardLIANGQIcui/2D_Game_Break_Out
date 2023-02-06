//#ifndef MODEL_H
//#define MODEL_H
//
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//#include <glad/glad.h>
//
//#include<glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#include "Mesh.h"
//#include "Shader.h"
//
//#include <string>
//#include<fstream>
//#include<sstream>
//#include <iostream>
//#include <map>
//#include <vector>
//using namespace std;
//
//unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
//
//class Model
//{
//public:
//	//定义模型包含数据
//	vector<Texture> textures_loaded;//存储所有加载过的纹理
//	vector<Mesh> meshes;
//	string directory;
//	bool gammaCorrection;
//
//	//构造器
//	Model(string const& path, bool gamma = false) :gammaCorrection(gamma)
//	{
//		loadModel(path);
//	}
//
//	//绘制模型
//	void Draw(Shader& shader)
//	{
//		for (unsigned int i = 0;i < meshes.size();i++)
//		{
//			meshes[i].Draw(shader);
//		}
//	}
//
//private:
//	//从文件中读取模型信息，并加载模型并存储所有的网格到容器中
//	void loadModel(string const& path)
//	{
//		//通过Assimp读取文件
//		Assimp::Importer importer;
//		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
//			aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//
//		//判断指针是否为空
//		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//		{
//			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
//			return;
//		}
//
//		//获取文件的目录路径
//		directory = path.substr(0, path.find_last_of('/'));
//
//		//将获取的数据转换到网格类的数据
//		processNode(scene->mRootNode, scene);
//
//	}
//
//	//递归函数，处理从父节点到子节点的网格数据，转换模型数据并存储到网格中
//	void processNode(aiNode* node, const aiScene* scene)
//	{
//		//处理每个节点中所有网格，每个节点都包含网格的索引
//		for (unsigned int i = 0;i < node->mNumMeshes;i++)
//		{
//			//首先通过索引获取每个ASSIMP网格
//			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//			//接着将获取到的模型网格通过processMesh函数处理得到网格类的网格，并存储
//			meshes.push_back(processMesh(mesh, scene));
//		}
//		//接着利用递归处理子节点的网格数据
//		for (unsigned int i = 0;i < node->mNumChildren;i++)
//		{
//			processNode(node->mChildren[i], scene);
//		}
//	}
//
//	//网格处理函数
//	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
//	{
//		//建立临时变量用于给网格中的属性赋值
//		vector<Vertex> vertices;
//		vector<unsigned int> indices;
//		vector<Texture> textures;
//
//		//遍历每个ASSIMP网格顶点并转化存储
//		for (unsigned int i = 0;i < mesh->mNumVertices;i++)
//		{
//			Vertex vertex;
//			glm::vec3 vector;
//			//获取顶点位置属性
//			vector.x = mesh->mVertices[i].x;
//			vector.y = mesh->mVertices[i].y;
//			vector.z = mesh->mVertices[i].z;
//			vertex.Position = vector;
//			//法线
//			if (mesh->HasNormals())
//			{
//				vector.x = mesh->mNormals[i].x;
//				vector.y = mesh->mNormals[i].y;
//				vector.z = mesh->mNormals[i].z;
//				vertex.Normal = vector;
//			}
//			//纹理坐标
//			if (mesh->mTextureCoords[0])
//			{
//				glm::vec2 vec;
//
//				vec.x = mesh->mTextureCoords[0][i].x;
//				vec.y = mesh->mTextureCoords[0][i].y;
//				vertex.TexCoords = vec;
//				//tangent
//				vector.x = mesh->mTangents[i].x;
//				vector.y = mesh->mTangents[i].y;
//				vector.z = mesh->mTangents[i].z;
//				vertex.Tangent = vector;
//				// bitangent
//				vector.x = mesh->mBitangents[i].x;
//				vector.y = mesh->mBitangents[i].y;
//				vector.z = mesh->mBitangents[i].z;
//				vertex.Bitangent = vector;
//			}
//			else
//			{
//				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//			}
//
//			//一个顶点属性填充完后，加入数组中
//			vertices.push_back(vertex);
//		}
//
//		//遍历每个网格的面数，将面中的索引保存下来用于后续着色器绘制
//		for (unsigned int i = 0;i < mesh->mNumFaces;i++)
//		{
//			aiFace face = mesh->mFaces[i];
//			for (unsigned int j = 0;j < face.mNumIndices;j++)
//			{
//				indices.push_back(face.mIndices[j]);
//			}
//		}
//		
//		//处理材料
//		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//
//		//1-漫反射纹理贴图
//		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//		//2-高光贴图
//		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//		//3-法线贴图
//		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
//		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//		//4-权重贴图
//		vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
//		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
//
//		//通过得到的顶点、索引、纹理构造网格并返回
//		return Mesh(vertices, indices, textures);
//	}
//
//	//加载纹理函数
//	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
//	{
//		vector<Texture> textures;
//		for (unsigned int i = 0;i < mat->GetTextureCount(type);i++)
//		{
//			aiString str;
//			//获取纹理位置并存到变量str中
//			mat->GetTexture(type, i, &str);
//			//检查纹理是否被加载，如果被加载则使用已加载过的，未加载则进行加载
//			bool skip = false;
//			for (unsigned int j = 0;j < textures_loaded.size();j++)
//			{
//				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
//				{
//					textures.push_back(textures_loaded[j]);
//					skip = true;
//					break;
//				}
//			}
//			if (!skip)
//			{
//				Texture texture;
//				texture.id = TextureFromFile(str.C_Str(), this->directory);
//				texture.type = typeName;
//				texture.path = str.C_Str();
//				textures.push_back(texture);
//				textures_loaded.push_back(texture);
//			}
//		}
//
//		return textures;
//	}
//
//};
//
//
//unsigned int TextureFromFile(const char* path, const string& directory, bool gamma )
//{
//	string filename = string(path);
//	filename = directory + '/' + filename;
//
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID;
//}
//
//
//
//#endif
