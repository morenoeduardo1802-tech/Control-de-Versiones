//Pr�ctica 2: �ndices, mesh, proyecciones, transformaciones geom�tricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crear�an ac�
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* fShaderAzul = "shaders/shaderazul.frag";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* fShaderVerde = "shaders/shaderverde.frag";
static const char* vShaderVerde2 = "shaders/shaderverde2.vert";
static const char* fShaderVerde2 = "shaders/shaderverde2.frag";
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* fShaderRojo = "shaders/shaderrojo.frag";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* fShaderCafe = "shaders/shadercafe.frag";

float angulo = 0.0f;

//color caf� en RGB : 0.478, 0.255, 0.067

//Pir�mide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//V�rtices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}


void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		//X			Y			Z			R		G		B
		// Letra E (x de -0.4 a 0.4, desplazada a x=-1.5)
		// Barra vertical: tri�ngulo 1
		-1.9f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 0
		-1.7f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 1
		-1.7f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 2
		// Barra vertical: tri�ngulo 2
		-1.9f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 3
		-1.7f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 4
		-1.9f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 5
		// Barra superior: tri�ngulo 1
		-1.7f,  0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 6
		-1.1f,  0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 7
		-1.1f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 8
		// Barra superior: tri�ngulo 2
		-1.7f,  0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 9
		-1.1f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 10
		-1.7f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 11
		// Barra media: tri�ngulo 1
		-1.7f, -0.2f, 0.0f, 0.0f, 0.0f, 1.0f,  // 12
		-1.1f, -0.2f, 0.0f, 0.0f, 0.0f, 1.0f,  // 13
		-1.1f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 14
		// Barra media: tri�ngulo 2
		-1.7f, -0.2f, 0.0f, 0.0f, 0.0f, 1.0f,  // 15
		-1.1f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 16
		-1.7f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 17
		// Barra inferior: tri�ngulo 1
		-1.7f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 18
		-1.1f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 19
		-1.1f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 20
		// Barra inferior: tri�ngulo 2
		-1.7f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  // 21
		-1.1f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 22
		-1.7f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f,  // 23
		// Letra J (x de -0.4 a 0.4, desplazada a x=-0.5)
		// Barra vertical: tri�ngulo 1
		-0.3f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 24
		-0.1f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 25
		-0.1f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 26
		// Barra vertical: tri�ngulo 2
		-0.3f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 27
		-0.1f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 28
		-0.3f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 29
		// Base horizontal: tri�ngulo 1
		-0.9f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 30
		-0.3f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 31
		-0.3f, -0.6f, 0.0f, 1.0f, 0.0f, 0.0f,  // 32
		// Base horizontal: tri�ngulo 2
		-0.9f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,  // 33
		-0.3f, -0.6f, 0.0f, 1.0f, 0.0f, 0.0f,  // 34
		-0.9f, -0.6f, 0.0f, 1.0f, 0.0f, 0.0f,  // 35
		// Curva inferior: tri�ngulo 1
		-0.9f, -0.6f, 0.0f, 1.0f, 0.0f, 0.0f,  // 36
		-0.7f, -0.6f, 0.0f, 1.0f, 0.0f, 0.0f,  // 37
		-0.7f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // 38
		// Curva inferior: tri�ngulo 2
		-0.9f, -0.6f, 0.0f, 1.0f, 0.0f, 0.0f,  // 39
		-0.7f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // 40
		-0.9f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // 41
		// Letra M (x de -0.4 a 0.4, desplazada a x=0.5)
		// Barra vertical izquierda: tri�ngulo 1
		0.1f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 42
		0.3f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 43
		0.3f,  0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 44
		// Barra vertical izquierda: tri�ngulo 2
		0.1f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 45
		0.3f,  0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 46
		0.1f,  0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 47
		// Diagonal izquierda: tri�ngulo 1
	   // 0.3f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f,   // 48
	   // 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 49
	   // 0.3f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 50
		// Diagonal izquierda: tri�ngulo 2
		0.3f,  0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 51
		0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // 52
		0.4f,  0.2f, 0.0f, 1.0f, 0.0f, 1.0f,   // 53
		/*   // Diagonal derecha: tri�ngulo 1
		   0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 54
		   0.7f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f,   // 55
		   0.7f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 56*/
		   // Diagonal derecha: tri�ngulo 2
		0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // 57
		0.7f,  0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 58
		0.6f,  0.2f, 0.0f, 1.0f, 0.0f, 1.0f,   // 59
		// Barra vertical derecha: tri�ngulo 1
		0.7f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 60
		0.9f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 61
		0.9f,  0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 62
		// Barra vertical derecha: tri�ngulo 2
		0.7f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 63
		0.9f,  0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 64
		0.7f,  0.8f, 0.0f, 1.0f, 0.0f, 1.0f,   // 65
		// Letra R (x de -0.4 a 0.4, desplazada a x=1.5)
		// Barra vertical: tri�ngulo 1
		1.1f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 66
		1.3f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 67
		1.3f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 68
		// Barra vertical: tri�ngulo 2
		1.1f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 69
		1.3f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 70
		1.1f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 71
		// Bucle superior: tri�ngulo 1
		1.3f,  0.2f, 0.0f, 0.0f, 1.0f, 0.0f,   // 72
		1.9f,  0.2f, 0.0f, 0.0f, 1.0f, 0.0f,   // 73
		1.9f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 74
		// Bucle superior: tri�ngulo 2
		1.3f,  0.2f, 0.0f, 0.0f, 1.0f, 0.0f,   // 75
		1.9f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 76
		1.3f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 77
		// Pata diagonal: tri�ngulo 1
		1.3f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f,   // 78
		1.9f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 79
		1.5f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f,   // 80
		// Pata diagonal: tri�ngulo 2
		1.5f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f,   // 81
		1.9f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,   // 82
		1.7f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f    // 83

	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 504);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);


	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.5f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.5f,	0.0f,

	};

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);


	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);



	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,	0.255f,	0.067f,

	};

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar �ndices: objetos: cubo y  pir�mide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader(); //shader azul
	shader3->CreateFromFiles(vShaderAzul, fShaderAzul);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader(); //shader verde
	shader4->CreateFromFiles(vShaderVerde, fShaderVerde);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader(); //shader rojo
	shader5->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader(); //shader cafe
	shader6->CreateFromFiles(vShaderCafe, fShaderCafe);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader(); //shader verde2
	shader7->CreateFromFiles(vShaderVerde2, fShaderVerde2);
	shaderList.push_back(*shader7);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //�ndice 0 en MeshList
	CrearCubo();//�ndice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, �ndices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensi�n 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensi�n 4x4 en la cual se almacena la multiplicaci�n de las transformaciones geom�tricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

	/*
		//Para las letras hay que usar el segundo set de shaders con �ndice 1 en ShaderList
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensi�n 4x4 que servir� como matriz de modelo para almacenar las transformaciones geom�tricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se env�an al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
	*/
		
	
		//Para el cubo y la pir�mide se usa el primer set de shaders con �ndice 0 en ShaderList
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensi�n 4x4 que servir� como matriz de modelo para almacenar las transformaciones geom�tricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.0125f, -0.38f, -3.0f));
		model = glm::scale(model, glm::vec3(0.8f, 1.12f, 1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		angulo += 0.01;

		//Cubo 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.765f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.35f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubo 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.18f, -0.08f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.35f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMesh();

		//Cubo 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, -0.08f, -2.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.35f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshList[1]->RenderMesh();

		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;

		//Cubo 5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.64f, -0.815f, -2.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.25f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubo 6
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.615f, -0.815f, -2.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.25f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//Para la pir�mide se usa el primer set de shaders con �ndice 0 en ShaderList
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;

		//Piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.0125f, 0.58f, -3.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.8f, 0.8f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.01;
		//Piramide 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.645f, -0.439f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Piramide 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.62f, -0.439f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.5f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

	
		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslaci�n
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACI�N //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/