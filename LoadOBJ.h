#pragma once
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include<math.h>
#include"Camera.h"
#include"bits\stdc++.h"

// 新增顶点数据结构体
struct Vertex_Old {
	GLfloat x, y, z;    // 位置坐标
	GLfloat nx, ny, nz; // 法线向量
	GLfloat u, v;       // 纹理坐标
	int materialID;     // 材质索引
};

struct Material_Old {     //新增材质数据结构
	std::string name;
	GLfloat Ka[3] = { 0.2f, 0.2f, 0.2f }; // 环境光默认值
	GLfloat Kd[3] = { 0.4f, 0.4f, 0.4f }; // 漫反射默认值
	GLfloat Ks[3] = { 0.5f, 0.5f, 0.5f }; // 镜面反射默认值
	GLfloat Ns = 10.0f;                 // 高光指数默认值
	std::string map_Kd;                  // 漫反射贴图路径
	GLuint textureID = 0;
	int cachedIndex = -1;                 // 缓存字段
};

GLuint loadTexture(const char* filename);

class ObjLoader
{
protected:
	std::vector<Vertex_Old> vertices;
	std::vector<unsigned int> indices;

	std::string currentMaterial; // 当前使用的材质名称

	Vector3 m_position;          //模型的位置
	Vector3 m_rotation;          //模型的旋转
	Vector3 m_scale;             //模型的缩放

public:
	// 材质组，给各顶点的材质分组
	struct MaterialGroup {
		int materialID;
		size_t start;
		size_t end;
	};
	std::vector<MaterialGroup> materialGroups;
	size_t currentStart = 0;
	std::string prevMaterial;

	std::vector<Material_Old> materials;

	ObjLoader();

	ObjLoader(const Vector3& pos, const Vector3& rot, const Vector3& sca);

	bool load(const char* path);

	// 查询材质名称对应的材质在materials材质数组中的索引
	int findMaterialIndex(const std::string& name);

	// 新增材质加载方法
	bool loadMTL(const char* mtlPath);

	//根据不同材质绘制顶点
	void draw();

	//返回模型位置
	Vector3 GetPosition() const { return m_position; };
};



