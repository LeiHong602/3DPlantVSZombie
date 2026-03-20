#pragma once
#include <gl/glew.h>
#include "stb_image.h"

typedef struct _AUX_RGBImageRec2 {
	GLint sizeX, sizeY;
	unsigned char* data;
}AUX_RGBImageRec2;

class Cube
{
public:
	AUX_RGBImageRec2* LoadBMP(const char* Filename);				// Loads A Bitmap Image
	int LoadGLTextures(const char* tempstr, int i);									// Load Bitmaps And Convert To Textures
	bool LoadAllTextures();

	void DrawGrass();
	void DrawGrass1();
	void DrawStoneBricks();
	void DrawRoad();
	void DrawRoadLine();
	void DrawBirch();
	void DrawBirchBoard();
	void DrawBirchDoorBottom();
	void DrawBirchDoorTop();
	void DrawBricks();
	void BuildHouse();

	/// <summary>
	/// 绘制所有地形,包括房子
	/// </summary>
	void DrawTerrainScene();



private:
	GLuint	texture[13];			// 存放贴图对象
	AUX_RGBImageRec2 g_AUX_RGBImageRec;
};

