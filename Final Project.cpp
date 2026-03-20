/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing This Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */
//#include "Keys.h"
#include "PeaShooterBullet.h"
#include "Animator.h"
#include "ZombiesManager.h"
#include "GameManager.h"
#include "SunManager.h"
#include "PeaShooterBulletBoom.h"
#include "PlantsManager.h"
#include <windows.h>		// Header File For Windows
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <gl/freeglut.h>
//#include <GLFW/glfw3.h>
//#include <gl/glut.h>
#include <math.h>
#include <cstdlib>
#include <cmath>

#include"Camera.h"

#include "bits\stdc++.h"
#define STB_IMAGE_IMPLEMENTATION

#include "Font.h"
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"

#include "FireworksParticle.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Input.h"


HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat xrot = 0.0f;       // 当前X轴旋转角度
GLfloat rotationSpeed = 1.0f; // 旋转速度(度/帧)

GLfloat scaleY = 1.0f;        // 当前Y轴缩放因子
GLfloat scaleSpeed = 0.01f;    // 缩放速度
GLfloat maxScale = 3.0f;       // 最大缩放值
GLfloat minScale = 0.1f;       // 最小缩放值
bool increasing = true;        // 是否正在增大


GLfloat zPos = 0.0f;          // 当前Z轴位置
GLfloat moveSpeed = 0.01f;     // 移动速度
GLfloat maxZ = 4.0f;          // 最大Z值
GLfloat minZ = -4.0f;         // 最小Z值
bool movingForward = true;    // 移动方向

GLfloat publicAngle = 0.0f;  // 公转角度

Camera m_Camera;            //摄像机类
Keys m_Keys;				/**< 按键类 */
GLFont m_Font;              /**< 字体类 */

//GLuint	texture[13];			// 存放贴图对象
Cube allCube;               //绘制所有方块的类




LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

// 全局植物模型声明
Plant PeaShooter;
Plant Sunflower1;
Plant Cat;
Plant Chomper;
Plant ColdPeaShooter;
Plant CornCannon;
Plant Cushaw;
Plant DoubleSunflower;
Plant Luker;
Plant LukerKing;
Plant PeaGatling;
Plant Potato;
Plant PotatorMine;
Plant Pumpking;
Plant TallPotato;
Plant Zombie;

FireworksParticle* f;
CSkyBox    m_SkyBox;                      /**< 天空类 */
CTerrain   m_Terrain;                     /**< 地形类 */
CInputSystem* g_Input;
// --- Shader Sources ---
const char* vShader = R"(
#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in ivec4 inBoneIDs;
layout(location = 4) in vec4 inWeights;

uniform mat4 bones[100];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main() {
    vec4 weights = inWeights;
    mat4 boneTransform =
        weights.x * bones[inBoneIDs.x] +
        weights.y * bones[inBoneIDs.y] +
        weights.z * bones[inBoneIDs.z] +
        weights.w * bones[inBoneIDs.w];

    vec4 skinnedPos = boneTransform * vec4(inPos, 1.0);
    vec3 skinnedNorm = normalize(mat3(boneTransform) * inNormal);

    vec4 worldPos = model * skinnedPos;
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(model))) * skinnedNorm;
    TexCoord = inTexCoords;
    gl_Position = projection * view * worldPos;
}
)";

const char* fShader = R"(
#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = 0.3 * lightColor;

    vec3 lighting = ambient + diffuse;
    vec4 texColor = texture(texture_diffuse1, TexCoord);
    FragColor = vec4(lighting * texColor.rgb, texColor.a);
}
)";

// 与动态模型相关的全局变量
unsigned int shader = 0;//着色器变量

glm::mat4 view;//声明存放视图矩阵的变量
glm::mat4 projection;//声明存放投影矩阵的变量


GLint viewLoc;//视图矩阵位置
GLint viewPosLoc;//声明接受摄像机的位置

/// <summary>
/// 选择字体对象（包括格式）
/// </summary>
/// <param name="size"></param>
/// <param name="charset"></param>
/// <param name="face"></param>
void selectFont(int size, int charset, const char* face)
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);//选择字体对象，返回旧的对象
	DeleteObject(hOldFont);//删除旧的对象
}

/// <summary>
/// 画xyz字用
/// </summary>
/// <param name="str"></param>
void drawCNString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);
	// 计算字符的个数
	// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	// 否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}
	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';
	// 逐个输出字符
	for (i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}
	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

/// <summary>
/// Resize And Initialize The GL Window
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}


/// <summary>
/// 加载max导入的材质和模型
/// </summary>
/// <returns></returns>
//bool LoadModel()//加载3dmax导入模型的材质和模型
//{
//	//先加载材质,不然没有materials数组,加载豌豆射手
//	if (!PeaShooter.loadMTL("materials/Model/PeaShooter/PeaShooter.mtl")) 
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//	// 加载OBJ模型
//	if (!PeaShooter.load("materials/Model/PeaShooter/PeaShooter.obj")) 
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//	// 加载纹理
//	for (auto& mat : PeaShooter.materials) 
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载向日葵
//	if (!Sunflower.loadMTL("materials/Model/Sunflower/Sunflower.mtl")) 
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//	// 加载OBJ模型
//	if (!Sunflower.load("materials/Model/Sunflower/Sunflower.obj")) 
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//	// 加载纹理
//	for (auto& mat : Sunflower.materials) 
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载猫猫射手
//	if (!Cat.loadMTL("materials/Model/Cat/Cat.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//	
//	if (!Cat.load("materials/Model/Cat/Cat.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : Cat.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载食人花
//	if (!Chomper.loadMTL("materials/Model/Chomper/Chomper.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!Chomper.load("materials/Model/Chomper/Chomper.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : Chomper.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载寒冰射手
//	if (!ColdPeaShooter.loadMTL("materials/Model/ColdPeaShooter/ColdPeaShooter.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!ColdPeaShooter.load("materials/Model/ColdPeaShooter/ColdPeaShooter.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : ColdPeaShooter.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载玉米加农炮
//	if (!CornCannon.loadMTL("materials/Model/CornCannon/CornCannon.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!CornCannon.load("materials/Model/CornCannon/CornCannon.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : CornCannon.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载窝瓜
//	if (!Cushaw.loadMTL("materials/Model/Cushaw/Cushaw.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!Cushaw.load("materials/Model/Cushaw/Cushaw.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : Cushaw.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载双头向日葵
//	if (!DoubleSunflower.loadMTL("materials/Model/DoubleSunflower/DoubleSunflower.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!DoubleSunflower.load("materials/Model/DoubleSunflower/DoubleSunflower.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : DoubleSunflower.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载地刺
//	if (!Luker.loadMTL("materials/Model/Luker/Luker.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!Luker.load("materials/Model/Luker/Luker.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : Luker.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	//加载地刺王
//	if (!LukerKing.loadMTL("materials/Model/LukerKing/LukerKing.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!LukerKing.load("materials/Model/LukerKing/LukerKing.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : LukerKing.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	// PeaGatling
//	if (!PeaGatling.loadMTL("materials/Model/PeaGatling/PeaGatling.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!PeaGatling.load("materials/Model/PeaGatling/PeaGatling.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : PeaGatling.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	// Potato
//	if (!Potato.loadMTL("materials/Model/Potato/Potato.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!Potato.load("materials/Model/Potato/Potato.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : Potato.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	// PotatorMine
//	if (!PotatorMine.loadMTL("materials/Model/PotatoMine/PotatoMine.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!PotatorMine.load("materials/Model/PotatoMine/PotatoMine.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : PotatorMine.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	// Pumpkin
//	if (!Pumpking.loadMTL("materials/Model/Pumpking/Pumpking.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!Pumpking.load("materials/Model/Pumpking/Pumpking.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : Pumpking.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	// TallPotato
//	if (!TallPotato.loadMTL("materials/Model/TallPotato/TallPotato.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!TallPotato.load("materials/Model/TallPotato/TallPotato.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : TallPotato.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	// Zombie
//	if (!Zombie.loadMTL("materials/Model/Zombie/Zombie.mtl"))
//	{
//		MessageBox(NULL, "Failed to load mtl!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	if (!Zombie.load("materials/Model/Zombie/Zombie.obj"))
//	{
//		MessageBox(NULL, "Failed to load model!", "Error", MB_OK);
//		return FALSE;
//	}
//
//	for (auto& mat : Zombie.materials)
//	{
//		if (!mat.map_Kd.empty()) {
//			mat.textureID = loadTexture(mat.map_Kd.c_str());
//		}
//	}
//
//	return TRUE;
//}

/// <summary>
/// 创建shader
/// </summary>
/// <param name="vSrc"></param>
/// <param name="fSrc"></param>
/// <returns>输出新的shader</returns>
unsigned int createShaderProgram(const char* vSrc, const char* fSrc)
{
	unsigned int v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &vSrc, nullptr);
	glCompileShader(v);

	int success;
	char infoLog[512];
	glGetShaderiv(v, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(v, 512, nullptr, infoLog);
		std::cout << "Vertex Shader Error: " << infoLog << std::endl;
	}

	unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f, 1, &fSrc, nullptr);
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(f, 512, nullptr, infoLog);
		std::cout << "Fragment Shader Error: " << infoLog << std::endl;
	}

	unsigned int prog = glCreateProgram();
	glAttachShader(prog, v);
	glAttachShader(prog, f);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(prog, 512, nullptr, infoLog);
		std::cout << "Shader Program Error: " << infoLog << std::endl;
	}

	glDeleteShader(v);
	glDeleteShader(f);
	return prog;
}

/// <summary>
/// 初始化函数
/// </summary>
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	GLfloat lightPos[4] = { 50.0f, 25.0f, 50.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	GLfloat SpecularLight[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	GLfloat DiffuseLight[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	GLfloat AmbientLight[4] = { 0.2f,0.2f, 0.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	

	glewInit();  //初始化glew,要在所有glew相关函数前初始化

	glfwInit();//初始化glfw

	shader = createShaderProgram(vShader, fShader);//初始化shader

	ZombiesManager::shader = shader;//将着色器传递给僵尸管理器

	projection = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 200.0f);//初始化投影矩阵,新老版本的投影矩阵的最近与最远可视距离要一致

	glUseProgram(shader);//使用着色器shader
	
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);//初始化投影矩阵
	viewLoc = glGetUniformLocation(shader, "view");//获取视图矩阵的位置

	glUniform3f(glGetUniformLocation(shader, "lightPos"), 5.0f, 10.0f, 5.0f);//初始化光照位置
	glUniform3f(glGetUniformLocation(shader, "lightColor"), 1.0f, 1.0f, 1.0f);//初始化光照颜色
	viewPosLoc = glGetUniformLocation(shader, "viewPos");//获取着色器中存放摄像机的位置

	//将纹理单元绑定到着色器采样器统一变量的关键操作 
	GLint locTex = glGetUniformLocation(shader, "texture_diffuse1"); //告诉着色器中的漫反射纹理采样器使用纹理单元0来读取漫反射纹理
	if (locTex >= 0)
		glUniform1i(locTex, 0);

	GLint locHas = glGetUniformLocation(shader, "hasDiffuseTexture");
	if (locHas >= 0)
		glUniform1i(locHas, 1);


	//zombies= CommonZombie(glm::vec3(20, 5, 0), glm::vec3(0, 1, 0), 90, glm::vec3(0.1f), shader);


	bool flag= allCube.LoadAllTextures();//加载贴图
	if (flag == FALSE)
		return FALSE;

	//flag= LoadModel();//加载3dmax导入模型的材质和模型
	//if (flag == FALSE)
	//	return FALSE;

	PlantsManager::Init();

	Vector3 v = Vector3(0, 20, 0);
	f = new FireworksParticle(v);

	/** 初始化天空 */
	if (!m_SkyBox.init())
	{
		MessageBox(NULL, "初始化天空失败!", "错误", MB_OK);
		exit(0);
	}
	/** 初始化地形 */
	if (!m_Terrain.init())
	{
		MessageBox(NULL, "初始化地形失败!", "错误", MB_OK);
		exit(0);
	}

	/** 创建输入系统 */
	g_Input = new CInputSystem();

	/** 输入系统初始化 */
	g_Input->Init(hWnd, (HINSTANCE)GetModuleHandle(NULL),
		true, IS_USEKEYBOARD);

	return TRUE;										// Initialization Went OK
}

/** 更新摄像机 */
void UpdateCamera()
{
	m_Camera.setViewByMouse();

	int speedPow = 6;

	/** 键盘按键响应 */
	if (m_Keys.IsPressed(VK_SHIFT))                      /**< 按下SHIFT键时加速 */
	{
		m_Camera.setSpeed(0.6f);
	}
	if (!m_Keys.IsPressed(VK_SHIFT))
	{
		m_Camera.setSpeed(0.2f);
	}
	if (g_Input->GetKeyboard()->KeyDown(DIK_W))                                       /**< 'W'键按下 */
		m_Camera.moveCamera(m_Camera.getSpeed()* speedPow);          /**< 移动摄像机 */

	if (g_Input->GetKeyboard()->KeyDown(DIK_S))                                        /**< 'S'键按下 */
		m_Camera.moveCamera(-m_Camera.getSpeed() * speedPow);         /**< 移动摄像机 */

	if (g_Input->GetKeyboard()->KeyDown(DIK_A))											/**< 'A'键按下 */
		m_Camera.yawCamera(-m_Camera.getSpeed() * speedPow);          /**< 移动摄像机 */

	if (g_Input->GetKeyboard()->KeyDown(DIK_D))											/**< 'D'键按下 */
		m_Camera.yawCamera(m_Camera.getSpeed() * speedPow);            /**< 移动摄像机 */

	if (g_Input->GetKeyboard()->KeyDown(DIK_E))										/** 'E'键按下 */
		m_Camera.upCamera(m_Camera.getSpeed()*0.4);            /**< 向上移动摄像机 */

	if (g_Input->GetKeyboard()->KeyDown(DIK_Q))										/**'Q'键按下 */
		m_Camera.upCamera(-m_Camera.getSpeed()*0.4);            /**< 向下移动摄像机 */

}

/// <summary>
/// 每帧更新函数
/// </summary>
void Update()
{
	UpdateCamera();
	PlantsManager::Update();
	ZombiesManager::Update();
	SunManager::Update();
	GameManager::Update();
	g_Input->Update();

	if (GameManager::GetIsGameSuccess())
	{
		f->Update();
	}
	
}


/// <summary>
/// 绘制xyz轴
/// </summary>
void DrawXYZ()//绘制xyz轴
{
	glUseProgram(0);//关闭着色器不然看不见
	// 设置线宽
	glLineWidth(2.0f);

	glPushMatrix();
	glTranslatef(50, 25, 50);

	// 绘制 X 轴（绿色）
	glColor3f(0.0f, 1.0f, 0.0f); // 绿色
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // 起点
	glVertex3f(10.0f, 0.0f, 0.0f); // 终点
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.95, 0.1, 0);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.95, -0.1, 0);
	glEnd();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0X00FF);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, 0.0f, 0.0f);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	selectFont(48, GB2312_CHARSET, "微软雅黑");
	glColor3f(0.0f, 1.0f, 0.0f);//设置字体颜色
	glRasterPos3f(3.0f, -0.2f, 0.0f);
	drawCNString("x");

	// 绘制 Y 轴（红色）
	glColor3f(1.0f, 0.0f, 0.0f); // 红色
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // 起点
	glVertex3f(0.0f, 10.0f, 0.0f); // 终点
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.1, 0.95, 0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.1, 0.95, 0);
	glEnd();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0X00FF);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -10.0f, 0.0f);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	selectFont(48, GB2312_CHARSET, "微软雅黑");
	glColor3f(1.0f, 0.0f, 0.0f);//设置字体颜色
	glRasterPos3f(0.0f, 3.0f, 0.0f);
	drawCNString("y");

	// 绘制 Z 轴（蓝色）
	glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // 起点
	glVertex3f(0.0f, 0.0f, 10.0f); // 终点
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0.1, 0.95);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, -0.1, 0.95);
	glEnd();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0X00FF);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -10.0f);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	selectFont(48, GB2312_CHARSET, "微软雅黑");
	glColor3f(0.0f, 0.0f, 1.0f);//设置字体颜色
	glRasterPos3f(0.0f, 0.0f, 3.0f);
	drawCNString("z");

	glPopMatrix();
}


/** 输出文字信息 */
void PrintText()
{
	glUseProgram(0);//关闭着色器 不然看不见
	char string[128];                               /**< 用于保存输出字符串 */
	char temp[128];                                 /**< 用于保存阳光值字符串 */
	char moveMessage[256];
	char plantMessage[256];                         //用于保存种植信息的字符串
	glPushAttrib(GL_CURRENT_BIT);                   /**< 保存现有颜色属性信息 */
	glColor3f(0.0f, 1.0f, 1.0f);                      /**< 设置文字颜色 */
	sprintf(string, "当前位置:X=%3.1f  Y=%3.1f  Z=%3.1f  Speed =%3.1f",
		m_Camera.getPosition().x, m_Camera.getPosition().y, m_Camera.getPosition().z, m_Camera.getSpeed()); /**< 字符串赋值 */
	m_Font.PrintText(string, -5.0f, 4.0f);

	sprintf(temp, "阳光值:%d", SunManager::GetSunCount());
	m_Font.PrintText(temp, -5.0f, 3.5f);

	sprintf(moveMessage, "WASD：控制摄像机移动  上下左右：控制种植位置");
	m_Font.PrintText(moveMessage, -5.0f, 3.0f);

	sprintf(plantMessage, "小键盘1：向日葵（50阳光） 2：豌豆射手（150阳光） 3：土豆（75阳光）");
	m_Font.PrintText(plantMessage, -5.0f, 2.5f);



	///** 输出帧速 */
	//CaculateFrameRate();                               /**< 计算帧速 */
	//sprintf(string, "FPS:%d", (int)m_Fps);               /**< 字符串赋值 */
	//m_Font.PrintText(string, -5.0f, 3.0f);              /**< 输出字符串 */
	glPopAttrib();

}


int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1.1, 1, 200);//近平面距离太小可能导致物体有些面透明
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	//glTranslatef(0, 0, -1);

	view = m_Camera.setLook();//设置摄像机位置
	Vector3 cameraPos = m_Camera.getPosition();
	glUseProgram(shader);
	glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);//设置着色器中的摄像机位置，以便处理光线渲染
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);//将视图矩阵传给着色器
	glLoadMatrixf(glm::value_ptr(view));// 手动加载 GLM 矩阵到 OpenGL，这样可以使老版OpenGL的绘制也能看到

	
	glDisable(GL_LIGHTING);
	
	DrawXYZ();

	SunManager::Draw();
	
	// 启用必要状态 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_CULL_FACE);//取消背面消隐
	
	//DrawDanceModle();//绘制模型
	//zombies.DrawDynamicModel();
	
	
	ZombiesManager::Draw();
	
	glUseProgram(0);
	//DrawGrass();

	allCube.DrawTerrainScene();//画场景

	PlantsManager::Draw();

	if (GameManager::GetIsGameSuccess())
	{
		f->Render();
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1.1, 1, 1500);//得再修改一遍透视矩阵，太远会导致僵尸的腿消失,太近会导致看不到天空盒
	m_SkyBox.render();//绘制天空盒
	m_Terrain.render();//绘制地形
	
	PrintText();
	//glFlush();
	return TRUE;										// Keep Going
}

/// <summary>
/// 关闭窗口
/// </summary>
/// <param name=""></param>
/// <returns></returns>
GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	int nX = 0;
	int nY = 0;

	int wid = GetSystemMetrics(SM_CXSCREEN);		/**< 获取当前屏幕宽 */
	int hei = GetSystemMetrics(SM_CYSCREEN);		/**< 获取当前屏幕高 */
	nX = (wid - width) / 2;                    /**< 计算窗口居中用 */
	nY = (hei - height) / 2;

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								nX, nY,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		(BYTE)bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			m_Keys.SetPressed(wParam);              //设置key类中的数组
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			m_Keys.SetReleased(wParam);             //设置key类中的数组
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	char tempstr1[] = "Would You Like To Run In Fullscreen Mode?";
	char tempstr2[] = "Start FullScreen?";
	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL, tempstr1, tempstr2,MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}
	char tempstr3[] = "NeHe's OpenGL Framework";
	// Create Our OpenGL Window
	if (!CreateGLWindow(tempstr3,640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					Update();                       //每帧更新函数
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow(tempstr3,640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return ((int)msg.wParam);							// Exit The Program
}
