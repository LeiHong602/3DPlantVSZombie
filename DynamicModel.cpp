#include "DynamicModel.h"

DynamicModel::DynamicModel(glm::vec3 pos, glm::vec3 rot, float rad, glm::vec3 scale, unsigned int& shader) :shader(shader), position(pos), rotationAxis(rot), rotationAngle(rad), scale(scale)
{
	//设置平移、旋转和缩放
	model = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), glm::radians(rad), rot) * glm::scale(glm::mat4(1.0f), scale);
}

DynamicModel::DynamicModel() :shader(0)
{
	//设置平移和缩放
	model = glm::scale(glm::mat4(1.0f), glm::vec3(1,1,1)) * glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
}

DynamicModel::~DynamicModel()
{
}


int DynamicModel::DrawDynamicModel()
{
	//glPushMatrix();
	glUseProgram(shader);//使用着色器shader

	////将纹理单元绑定到着色器采样器统一变量的关键操作
	//GLint locTex = glGetUniformLocation(shader, "texture_diffuse1");
	//if (locTex >= 0)
	//	glUniform1i(locTex, 0);

	//glUniform3f(glGetUniformLocation(shader, "lightPos"), 5.0f, 10.0f, 5.0f);
	//glUniform3f(glGetUniformLocation(shader, "viewPos"), 0.0f, 2.0f, 5.0f);
	//glUniform3f(glGetUniformLocation(shader, "lightColor"), 1.0f, 1.0f, 1.0f);

	//GLint locHas = glGetUniformLocation(shader, "hasDiffuseTexture");
	//if (locHas >= 0)
	//	glUniform1i(locHas, 1);

	//这三个都要设置，得把原本的摄像机改成glm的摄像机
	//glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &model[0][0]);

	if (myModel)
	{
		auto transforms = animator->GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
		{
			std::string name = "bones[" + std::to_string(i) + "]";
			int loc = glGetUniformLocation(shader, name.c_str());
			if (loc != -1)
				glUniformMatrix4fv(loc, 1, GL_FALSE, &transforms[i][0][0]);
			else
				std::cout << "[Warning] Uniform " << name << " not found!" << std::endl;
		}

		myModel->Draw(shader);
	}

	//glPopMatrix();
	//glutSwapBuffers();

	return 1;
}

int DynamicModel::DynamicModelUpdata()
{
	if (animator)
	{
		float dt = 0.001f;
		animator->UpdateAnimation(dt);
	}
	return 1;
}
