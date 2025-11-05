#pragma once
#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getanguloLlantas() { return angulollantas; }
	GLfloat getanguloCofre() { return angulocofre; }
	GLfloat getposicionCarro() { return posicioncarro; }
	GLfloat getposicionHawk() { return posicionhawk; }
	GLfloat getlucespuntuales() { return lucespuntuales; }
	GLfloat getprendida() { return luzprendida; }
	GLfloat getlampara() { return luzlampara; }
	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz; //angulocabeza, angulomandibula, angulopatatrader, angulopatatraizq, angulopatadelder,angulopatadelizq;
	GLfloat angulollantas, angulocofre, posicioncarro, posicionhawk;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat lucespuntuales;
	GLfloat luzlampara;
	GLfloat luzprendida;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

