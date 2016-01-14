#include "stdafx.h"
#include "MGLUtil.h"

#include "MGLExceptions.h"

/*************************************/
/*********** MGL Namespace ***********/
/*************************************/

void MGL::EnableWireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
}

void MGL::DisableWireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void MGL::SetTextureParameters(const GLuint texture, const GLboolean repeat, const GLboolean linear) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint MGL::LoadTextureFromFile(const std::string& fileName, const GLboolean flipY) {
	GLuint tex = 0;

	try {
		GLint w, h, c;

		stbi_set_flip_vertically_on_load(flipY);

		GLubyte* image = stbi_load(fileName.c_str(), &w, &h, &c, STBI_rgb);

		MGLException_Null::Test(image);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(image);
		MGLException_IsZero::Test(tex);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << "LoadTextureFromFile " << fileName.c_str() << std::endl;
		return 0;
	}
	
	return tex;
}


int MGL::GetWindowInfo(GLFWwindow* window, MGLenum info, GLint attribute) {
	if (!window)
		return 0;

	GLint wanted = 0;
	GLint unw = 0;

	switch (info) {
	case MGL_WINDOWINFO_WIDTH:
		glfwGetWindowSize(window, &wanted, &unw);
		break;
	case MGL_WINDOWINFO_HEIGHT:
		glfwGetWindowSize(window, &unw, &wanted);
		break;
	case MGL_WINDOWINFO_XPOS:
		glfwGetWindowPos(window, &wanted, &unw);
		break;
	case MGL_WINDOWINFO_YPOS:
		glfwGetWindowPos(window, &unw, &wanted);
		break;
	case MGL_WINDOWINFO_ATTRIBUTE:
		wanted = glfwGetWindowAttrib(window, attribute);
		break;
	case MGL_WINDOWINFO_M_XPOS: {
		GLdouble want, unwd;
		glfwGetCursorPos(window, &want, &unwd);
		wanted = (GLint)want;
	}
								break;
	case MGL_WINDOWINFO_M_YPOS: {
		GLdouble want, unwd;
		glfwGetCursorPos(window, &unwd, &want);
		wanted = (GLint)want;
	}
								break;
	}
	return wanted;
}

void MGL::PrintMat4(const glm::mat4& matrix) {
	double val[16] = { 0.0 };

	const float *pSource = (const float*)glm::value_ptr(matrix);
	for (int i = 0; i < 16; ++i)
		val[i] = pSource[i];

	std::cout << val[0] << "\t" << val[1] << "\t" << val[2] << "\t" << val[3] << std::endl;
	std::cout << val[4] << "\t" << val[5] << "\t" << val[6] << "\t" << val[7] << std::endl;
	std::cout << val[8] << "\t" << val[9] << "\t" << val[10] << "\t" << val[11] << std::endl;
	std::cout << val[12] << "\t" << val[13] << "\t" << val[14] << "\t" << val[15] << std::endl;
}

/********************************/
/*********** MGLTimer ***********/
/********************************/

MGLTimer::MGLTimer() {
	// init times to now
	Start();
	End();
}

void MGLTimer::Start() {
	start = (GLfloat)glfwGetTime();
}

void MGLTimer::End() {
	end = (GLfloat)glfwGetTime();
}

GLfloat MGLTimer::Time() {
	return end - start;
}