#include "LoadOBJ.h"

// 新增纹理加载函数
GLuint loadTexture(const char* filename) {
	HBITMAP hBmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (!hBmp) {
		MessageBox(NULL, filename, "Error", MB_OK);
		return 0;
	}

	BITMAP bm;
	GetObject(hBmp, sizeof(bm), &bm);

	// 通道格式判断 
	GLenum format = GL_BGR_EXT;
	if (bm.bmBitsPixel == 32) format = GL_BGRA_EXT;

	// 创建纹理
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// 设置纹理参数 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 对齐设置
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// 加载纹理数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bm.bmWidth, bm.bmHeight,
		0, format, GL_UNSIGNED_BYTE, bm.bmBits);

	// 生成mipmaps（关键补充） 
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bm.bmWidth, bm.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bm.bmBits);

	DeleteObject(hBmp);
	return textureID;
}


ObjLoader::ObjLoader()
{
	m_position = Vector3();
	m_rotation = Vector3();
	m_scale = Vector3(1,1,1);
}

ObjLoader::ObjLoader(const Vector3& pos, const Vector3& rot, const Vector3& sca)
{
	m_position = pos;
	m_rotation = rot;
	m_scale = sca;
}

bool ObjLoader::load(const char* path) {
	std::ifstream file(path);
	if (!file.is_open()) return false;

	std::vector<GLfloat> tempVertices, tempNormals, tempUVs;
	std::string line;

	// 清空旧数据
	vertices.clear();

	while (getline(file, line)) {
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "usemtl") {
			iss >> currentMaterial;
			// 当材质变化时记录分组
			if (currentMaterial != prevMaterial) {
				if (!prevMaterial.empty()) {
					materialGroups.push_back({
						findMaterialIndex(prevMaterial),
						currentStart,
						vertices.size()
						});
				}
				prevMaterial = currentMaterial;
				currentStart = vertices.size();
			}
		}

		else if (type == "v") {  // 几何顶点
			GLfloat x, y, z;
			iss >> x >> y >> z;
			tempVertices.insert(tempVertices.end(), { x,y,z });
		}
		else if (type == "vt") { // 纹理坐标
			GLfloat u, v;
			iss >> u >> v;
			tempUVs.insert(tempUVs.end(), { u, v });
		}
		else if (type == "vn") { // 法线向量
			GLfloat nx, ny, nz;
			iss >> nx >> ny >> nz;
			tempNormals.insert(tempNormals.end(), { nx, ny, nz });
		}
		else if (type == "f") {  // 面数据
			for (int i = 0; i < 3; ++i) {
				std::string vert;
				iss >> vert;
				std::replace(vert.begin(), vert.end(), '/', ' ');
				std::istringstream viss(vert);

				unsigned int vi, ti, ni;
				viss >> vi >> ti >> ni;

				// 构建顶点数据并附加材质ID
				// 修改面处理部分的顶点创建逻辑
				Vertex_Old vertex{
					tempVertices[(vi - 1) * 3],
					tempVertices[(vi - 1) * 3 + 1],
					tempVertices[(vi - 1) * 3 + 2],
					tempNormals[(ni - 1) * 3],
					tempNormals[(ni - 1) * 3 + 1],
					tempNormals[(ni - 1) * 3 + 2],
					tempUVs[(ti - 1) * 2],
					tempUVs[(ti - 1) * 2 + 1],
					findMaterialIndex(currentMaterial) // 确保使用当前材质
				};
				vertices.push_back(vertex);
			}
		}
	}
	if (!prevMaterial.empty()) {
		materialGroups.push_back({
			findMaterialIndex(prevMaterial),
			currentStart,
			vertices.size()
			});
	}
	return true;
}

int ObjLoader::findMaterialIndex(const std::string& name) {
	for (size_t i = 0; i < materials.size(); ++i) {
		if (materials[i].name == name) {
			materials[i].cachedIndex = i; // 缓存索引
			return i;
		}
	}
	return -1;
}

bool ObjLoader::loadMTL(const char* mtlPath) {
	std::ifstream file(mtlPath);
	if (!file.is_open()) return false;

	Material_Old currentMat;
	std::string line;

	while (getline(file, line)) {
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "newmtl") {
			if (!currentMat.name.empty()) {
				materials.push_back(currentMat);
			}
			currentMat = Material_Old(); // 重置材质
			iss >> currentMat.name;
		}
		else if (prefix == "Ka") { // 环境光
			iss >> currentMat.Ka[0] >> currentMat.Ka[1] >> currentMat.Ka[2];
		}
		else if (prefix == "Kd") { // 漫反射
			iss >> currentMat.Kd[0] >> currentMat.Kd[1] >> currentMat.Kd[2];
		}
		else if (prefix == "Ks") { // 高光
			iss >> currentMat.Ks[0] >> currentMat.Ks[1] >> currentMat.Ks[2];
		}
		else if (prefix == "Ns") { // 高光指数
			iss >> currentMat.Ns;
		}
		else if (prefix == "map_Kd") { // 纹理路径（支持带空格路径）
			std::string pathPart;
			while (iss >> pathPart) {
				if (currentMat.map_Kd.empty()) {
					currentMat.map_Kd = pathPart;
				}
				else {
					currentMat.map_Kd += " " + pathPart;
				}
			}
		}
	}
	if (!currentMat.name.empty()) {
		materials.push_back(currentMat);
	}
	return true;
}

void ObjLoader::draw() 
{
	glPushMatrix();
	
	glTranslatef(m_position.x, m_position.y, m_position.z);//处理位移

	//处理旋转
	if (m_rotation.x)
	{
		glRotatef(m_rotation.x, 1, 0, 0);
	}
	if (m_rotation.y)
	{
		glRotatef(m_rotation.y, 0, 1, 0);
	}
	if (m_rotation.z)
	{
		glRotatef(m_rotation.z, 0, 0, 1);
	}

	//处理缩放
	glScalef(m_scale.x,m_scale.y , m_scale.z);


	for (const auto& group : materialGroups) {
		if (group.materialID < 0 ||
			group.materialID >= materials.size()) continue;

		const auto& mat = materials[group.materialID];

		// 设置材质属性
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.Ks);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.Ns);

		// 绑定纹理
		if (mat.textureID != 0) {
			glBindTexture(GL_TEXTURE_2D, mat.textureID);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// 绘制该材质对应的三角形
		glBegin(GL_TRIANGLES);
		for (size_t i = group.start; i < group.end; ++i) {
			const auto& v = vertices[i];
			glNormal3f(v.nx, v.ny, v.nz);
			glTexCoord2f(v.u, v.v);
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}

	glPopMatrix();
}

