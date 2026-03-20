#include "Cube.h"


AUX_RGBImageRec2* Cube::LoadBMP(const char* Filename)				// Loads A Bitmap Image
{
	int channels;
	if (!Filename)										// Make Sure A Filename Was Given
	{
		return nullptr;									// If Not Return NULL
	}

	//File = fopen(Filename, "r");							// Check To See If The File Exists
	g_AUX_RGBImageRec.data = stbi_load(Filename, &g_AUX_RGBImageRec.sizeX, &g_AUX_RGBImageRec.sizeY, &channels, 0);
	//if (File)											// Does The File Exist?
	//{
		//fclose(File);									// Close The Handle
	return &g_AUX_RGBImageRec;				// Load The Bitmap And Return A Pointer
	//}
}

int Cube::LoadGLTextures(const char* tempstr, int i)									// Load Bitmaps And Convert To Textures
{
	int Status = false;									// Status Indicator

	//memset(pAUX_RGBImageRec, 0, sizeof(void*) * 1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	//char tempstr[] = "Data/glass.bmp";
	if (LoadBMP(tempstr))
	{
		Status = true;									// Set The Status To TRUE

		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, g_AUX_RGBImageRec.sizeX, g_AUX_RGBImageRec.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, g_AUX_RGBImageRec.data);

		//// Create Linear Filtered Texture
		//glBindTexture(GL_TEXTURE_2D, texture[1]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, g_AUX_RGBImageRec.sizeX, g_AUX_RGBImageRec.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, g_AUX_RGBImageRec.data);

		//// Create MipMapped Texture
		//glBindTexture(GL_TEXTURE_2D, texture[2]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, g_AUX_RGBImageRec.sizeX, g_AUX_RGBImageRec.sizeY, GL_RGB, GL_UNSIGNED_BYTE, g_AUX_RGBImageRec.data);
	}

	if (&g_AUX_RGBImageRec)								// If Texture Exists
	{
		if (g_AUX_RGBImageRec.data)						// If Texture Image Exists
		{
			free(g_AUX_RGBImageRec.data);				// Free The Texture Image Memory
		}
	}
	return Status;										// Return The Status
}

/// <summary>
/// 加载所有贴图
/// </summary>
/// <returns>加载成功返回true，失败返回false</returns>
bool Cube::LoadAllTextures()
{
	glGenTextures(13, &texture[0]);					// 创建13个贴图对象

	//加载贴图（OpenGL建立模型的贴图）
	if (!LoadGLTextures("materials/dirt.bmp", 0))		//加载泥土贴图
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/grass_side.bmp", 1))		// 加载草地边缘贴图
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/grass_top.bmp", 2))								//  加载草地顶部贴图
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/grass_top1.bmp", 3))								//  加载深色草地顶部贴图
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/grass_side_snowed.bmp", 4))								// 加载雪地边缘贴图
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/grass_top_snow.bmp", 5))								// 加载雪地顶部贴图
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/birch_log.bmp", 6))								// 加载白桦原木边缘
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/birch_log_top.bmp", 7))								// 加载白桦原木顶部
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/birch_planks.bmp", 8))								// 加载白桦木板贴图
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/bricks.bmp", 9))								// 加载红砖
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/stone_bricks.bmp", 10))								// 加载石砖
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/birch_door_bottom.bmp", 11))								// 加载门底部
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("materials/birch_door_top.bmp", 12))								// 加载门顶部
	{
		return false;									// If Texture Didn't Load Return FALSE
	}
	return true;
}

/// <summary>
/// 绘制浅色草地
/// </summary>
void Cube::DrawGrass()
{
	glEnable(GL_TEXTURE_2D);
	GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };									// 环境光
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };								// 漫反射
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制深色草地
/// </summary>
void Cube::DrawGrass1()
{
	glEnable(GL_TEXTURE_2D);
	GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };									// 环境光
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };								// 漫反射
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制石砖
/// </summary>
void Cube::DrawStoneBricks()//画石砖
{
	glEnable(GL_TEXTURE_2D);
	GLfloat mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };									// 环境光
	GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };								// 漫反射
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制道路
/// </summary>
void Cube::DrawRoad()
{
	glDisable(GL_TEXTURE_2D);
	// 设置Phong材质参数
	GLfloat mat_ambient1[] = { 0.2, 0.2, 0.2, 1.0 };									// 环境光
	GLfloat mat_diffuse1[] = { 0.2, 0.2, 0.2, 1.0 };								// 漫反射
	GLfloat mat_specular1[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess1 = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess1);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	// Top Face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom Face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制道路白线
/// </summary>
void Cube::DrawRoadLine()
{
	glDisable(GL_TEXTURE_2D);
	// 设置Phong材质参数
	GLfloat mat_ambient1[] = { 0.5, 0.5, 0.5, 1.0 };									// 环境光
	GLfloat mat_diffuse1[] = { 0.9, 0.9, 0.9, 1.0 };								// 漫反射
	GLfloat mat_specular1[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess1 = 100.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess1);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	// Top Face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom Face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制白桦原木
/// </summary>
void Cube::DrawBirch()
{
	glEnable(GL_TEXTURE_2D);
	GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };									// 环境光
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };								// 漫反射
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制白桦木板
/// </summary>
void Cube::DrawBirchBoard()//建立白桦木板
{
	glEnable(GL_TEXTURE_2D);
	GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };									// 环境光
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };								// 漫反射
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制白桦木门下半部分
/// </summary>
void Cube::DrawBirchDoorBottom()//建立白桦木板门底部
{
	glEnable(GL_TEXTURE_2D);
	GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };									// 环境光
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };								// 漫反射
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制白桦木门上半部分
/// </summary>
void Cube::DrawBirchDoorTop()//建立白桦木板门顶部
{
	glEnable(GL_TEXTURE_2D);
	GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };									// 环境光
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };								// 漫反射
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制红砖
/// </summary>
void Cube::DrawBricks()//建立红砖
{
	glEnable(GL_TEXTURE_2D);
	GLfloat mat_ambient[] = { 1, 1, 1, 1.0 };									// 环境光
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };								// 漫反射
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };								// 镜面反射
	GLfloat mat_shininess = 50.0;												// 高光指数
	GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };							// 无光(黑色光),用于关闭某种属性光时应用

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

/// <summary>
/// 绘制房子
/// </summary>
void Cube::BuildHouse()//建立房子
{
	int houseSize = 9;//房子的大小,一定要大于等于7,且为奇数
	int houseTall = 5;//房子的高度
	int x, y, z, tx, ty, tz;
	int vex[4][2] = { {-1,-(houseSize - 7) / 2},{-1,6 + (houseSize - 7) / 2},{-houseSize,6 + (houseSize - 7) / 2},{-houseSize,-(houseSize - 7) / 2} };//房子四个顶点的位置,从右上开始,顺时针
	for (int i = 0; i < 4; i++)//建立房子的四根立柱
	{
		for (int j = 0; j < houseTall; j++)
		{
			glPushMatrix();
			glTranslatef(vex[i][0] * 2, j * 2, vex[i][1] * 2);
			DrawBirch();
			glPopMatrix();
		}
	}

	//建立房子的四面墙
	for (int i = 0; i < 4; i++)
	{
		int j = (i + 1) % 4;
		x = vex[i][0];//起始点
		z = vex[i][1];
		tx = vex[j][0];//目标点
		tz = vex[j][1];
		if (x == tx)//如果是竖着的情况
		{
			int dleta = (tz - z) / (houseSize - 1);//判断方向
			for (z = z + dleta; z != tz; z = z + dleta)
			{
				glPushMatrix();

				glTranslatef(x * 2, 0, z * 2);

				glPushMatrix();
				glRotatef(90, 1, 0, 0);//让横梁倒过来
				DrawBirch();
				glPopMatrix();

				for (y = 1; y < houseTall - 1; y++)
				{
					glTranslatef(0, 2, 0);
					if (x == vex[0][0] && z == 3 && y <= 2)
					{
						if (y == 1)
							DrawBirchDoorBottom();
						else if (y == 2)
							DrawBirchDoorTop();
					}
					else
						DrawBirchBoard();
				}
				glTranslatef(0, 2, 0);

				glPushMatrix();
				glRotatef(90, 1, 0, 0);
				DrawBirch();
				glPopMatrix();

				glPopMatrix();
			}
		}
		else//如果是横着的情况
		{
			int dleta = (tx - x) / (houseSize - 1);//判断方向
			for (x = x + dleta; x != tx; x = x + dleta)
			{
				glPushMatrix();
				glTranslatef(x * 2, 0, z * 2);

				glPushMatrix();
				glRotatef(90, 0, 0, 1);
				DrawBirch();
				glPopMatrix();

				for (y = 1; y < houseTall - 1; y++)
				{
					glTranslatef(0, 2, 0);
					DrawBirchBoard();
				}
				glTranslatef(0, 2, 0);

				glPushMatrix();
				glRotatef(90, 0, 0, 1);
				DrawBirch();
				glPopMatrix();

				glPopMatrix();
			}
		}
	}


	//建立地板
	x = vex[0][0] - 1;
	z = vex[0][0] + 1;
	for (int i = 0; i < houseSize - 2; i++)
	{
		for (int j = 0; j < houseSize - 2; j++)
		{
			glPushMatrix();
			glTranslatef((x - i) * 2, 0, (z + j) * 2);
			DrawBirchBoard();
			glPopMatrix();
		}
	}

	//建立屋顶
	y = houseTall - 1;
	for (int i = 0; i < houseSize; i++)
	{
		z = vex[0][1] + i;
		if (i < houseSize / 2 + 1)
			y++;
		else
			y--;

		//做屋顶棚
		glPushMatrix();
		glTranslatef(0, y * 2, z * 2);
		for (x = 1; x <= houseSize; x++)
		{
			glPushMatrix();
			glTranslatef(-x * 2, 0, 0);
			DrawBricks();
			glPopMatrix();
		}
		glPopMatrix();

		//做屋顶墙面
		for (int y1 = houseTall; y1 < y; y1++)
		{
			glPushMatrix();
			glTranslatef(vex[0][0] * 2, y1 * 2, z * 2);
			if (z == vex[0][1] + houseSize / 2)
				DrawBirch();
			else
				DrawBricks();
			glTranslatef(-(houseSize - 1) * 2, 0, 0);
			if (z == vex[0][1] + houseSize / 2)
				DrawBirch();
			else
				DrawBricks();
			glPopMatrix();

		}
	}
}

/// <summary>
/// 绘制所有地形,包括房子
/// </summary>
void Cube::DrawTerrainScene()
{
	const int cellLong = 10;
	const int cellWide = 7;
	int grassColor = 1;//控制草地颜色

	int x, y, z;
	for (x = 0; x < cellLong; x++)//画草坪
	{
		for (z = 0; z < cellWide; z++)
		{
			glPushMatrix();
			glTranslatef(x * 2, 0, z * 2);
			if (grassColor == 1)
				DrawGrass();
			else
				DrawGrass1();
			glPopMatrix();
			grassColor = (grassColor + 1) % 2;
		}
	}

	for (x = -1; x <= cellLong; x++)//画石砖
	{
		glPushMatrix();
		glTranslatef(x * 2, 0, -1 * 2);
		DrawStoneBricks();
		glPopMatrix();
	}
	for (x = -1; x <= cellLong; x++)//画石砖
	{
		glPushMatrix();
		glTranslatef(x * 2, 0, cellWide * 2);
		DrawStoneBricks();
		glPopMatrix();
	}
	for (z = 0; z < cellWide; z++)//画石砖
	{
		glPushMatrix();
		glTranslatef(-1 * 2, 0, z * 2);
		DrawStoneBricks();
		glPopMatrix();
	}
	for (z = 0; z < cellWide; z++)//画石砖
	{
		glPushMatrix();
		glTranslatef(cellLong * 2, 0, z * 2);
		DrawStoneBricks();
		glPopMatrix();
	}

	int roadLine = 0;//控制什么时候画线
	for (x = cellLong + 1; x < cellLong + 4; x++)//画马路
	{
		for (z = -2 * cellWide; z < 3 * cellWide; z++)
		{
			glPushMatrix();
			glTranslatef(x * 2, 0, z * 2);
			if (x == cellLong + 2)
			{
				if (roadLine > 0)
					DrawRoadLine();
				else
					DrawRoad();
				roadLine = (roadLine + 1) % 4;
			}
			else
				DrawRoad();
			glPopMatrix();
		}
	}

	roadLine = 0;
	glPushMatrix();
	glTranslatef(3 * 2, 0, 0);
	for (x = cellLong + 1; x < cellLong + 4; x++)//画第二条马路
	{
		for (z = -2 * cellWide; z < 3 * cellWide; z++)
		{
			glPushMatrix();
			glTranslatef(x * 2, 0, z * 2);
			if (x == cellLong + 2)
			{
				if (roadLine > 0)
					DrawRoadLine();
				else
					DrawRoad();
				roadLine = (roadLine + 1) % 4;
			}
			else
				DrawRoad();
			glPopMatrix();
		}
	}
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2, 0, 0);
	BuildHouse();//画房子
	glPopMatrix();
}

