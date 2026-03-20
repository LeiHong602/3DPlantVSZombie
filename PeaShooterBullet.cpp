#include "PeaShooterBullet.h"

void PeaShooterBullet::Update()
{
	double currentTime = glfwGetTime();
	if (lastTime == 0) lastTime = currentTime;
	position.x += speed * (currentTime - lastTime);
	lastTime = currentTime;

}

void PeaShooterBullet::Draw()
{
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glTranslatef(position.x, position.y, position.z);
	glColor3f(0.43f, 0.83f, 0.23f); // 绿色
	GLUquadric* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricNormals(quad, GLU_SMOOTH);
	gluSphere(quad, 0.2f, 16, 16); // 绘制一个半径为0.1的球体
	gluDeleteQuadric(quad);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
