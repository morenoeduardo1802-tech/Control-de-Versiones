/*
Práctica 8: Iluminación 2 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture octaedroTexture;

Model Blackhawk_M;
Model CarroF_M;
Model LlantaDer_M;
Model LlantaIzq_M;
Model Cofre_M;
Model Lamp_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearOctaedro()
{
	unsigned int octaedro_indices[] =
	{
		0, 1, 2,    // Cara 1
		3, 4, 5,    // Cara 2
		6, 7, 8,    // Cara 3
		9, 10, 11,  // Cara 4
		12, 13, 14, // Cara 5
		15, 16, 17, // Cara 6
		18, 19, 20, // Cara 7
		21, 22, 23  // Cara 8
	};

	GLfloat octaedro_vertices[] =
	{
		// Cara 1: front
		//	x      y      z		S	   T		Nx	  Ny    Nz
		-0.5f, -0.5f, 0.5f,     0.0f, 0.4f,     0.0f, 1.0f, -1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,     0.0f, 1.0f, -1.0f,
		0.0f, 0.5f, 0.0f,		0.33f, 0.2f,    0.0f, 1.0f, -1.0f,

		// Cara 2: right 
		//x      y      z		S	   T		 Nx	    Ny    Nz
		0.5f, -0.5f, 0.5f,      0.66f, 0.0f,     -1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.66f, 0.399f,   -1.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,		0.33f, 0.2f,     -1.0f, 1.0f, 0.0f,
		// Cara 3: back
		//	x      y      z		 S	    T		 Nx	  Ny    Nz
		0.5f, -0.5f, -0.5f,     0.66f, 0.4f,     0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.33f, 0.6f,     0.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f,		0.33f, 0.2f,     0.0f, 1.0f, 1.0f,
		// Cara 4 left
		//	x      y      z		 S	    T		 Nx	  Ny    Nz
		-0.5f, -0.5f, -0.5f,    0.33f, 0.59f,    1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.01f, 0.4f,     1.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,		0.33f, 0.2f,     1.0f, 1.0f, 0.0f,

		// Cara 5:  front
		//x      y      z		 S	     T		 Nx	     Ny    Nz
		-0.5f, -0.5f, 0.5f,     0.995f, 1.0f,    0.0f, -1.0f, -1.0f,
		0.5f, -0.5f, 0.5f,		0.99f, 0.6f,     0.0f, -1.0f, -1.0f,
		0.0f, -1.5f, 0.0f,		0.67f, 0.8f,     0.0f, -1.0f, -1.0f,

		// Cara 6: right
		//x      y      z		S	     T		 Nx	     Ny    Nz
		0.5f, -0.5f, 0.5f,     0.3325f, 0.6f,   -1.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,	   0.3325f, 1.0f,   -1.0f, -1.0f, 0.0f,
		0.0f, -1.5f, 0.0f,	   0.66f, 0.8f,     -1.0f, -1.0f, 0.0f,

		// Cara 7:  back
		//x      y      z		 S	    T		 Nx	    Ny    Nz
		0.5f, -0.5f, -0.5f,		0.66f, 0.4f,     0.0f, -1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.33f, 0.6f,     0.0f, -1.0f, 1.0f,
		0.0f, -1.5f,   0.0f,	0.66f, 0.8f,     0.0f, 1.0f, 1.0f,

		// Cara 8: left
		//x      y      z		S	    T		  Nx	  Ny    Nz
		-0.5f, -0.5f, -0.5f,    0.99f, 0.6f,      1.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.665f, 0.4f,	  1.0f, -1.0f, 0.0f,
		0.0f, -1.5f, 0.0f,		0.665f, 0.8f,     1.0f, -1.0f, 0.0f,

	};

	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	meshList.push_back(octaedro);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearOctaedro();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	octaedroTexture = Texture("Textures/octaedro.tga");
	octaedroTexture.LoadTextureA();

	Lamp_M = Model();
	Lamp_M.LoadModel("Models/Lamp_Text.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	CarroF_M = Model();
	CarroF_M.LoadModel("Models/carroSnoopy.obj");
	LlantaIzq_M = Model();
	LlantaIzq_M.LoadModel("Models/llantaizq.obj");
	LlantaDer_M = Model();
	LlantaDer_M.LoadModel("Models/llantader.obj");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/cofre1.obj");
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
// --- Luz puntual de la lámpara ---
	pointLights[0] = PointLight(
		1.0f, 1.0f, 0.8f,   // color (blanco cálido)
		0.3f, 1.0f,         // intensidades
		0.0f, 0.0f, 0.0f,  // posición (encima del modelo Lamp_M)
		0.3f, 0.2f, 0.1f    // atenuación
	);
	pointLightCount++;

	// --- Luz puntual del octaedro ---
	pointLights[1] = PointLight(
		1.0f, 1.0f, 0.0f,   // color (blanco cálido)
		0.3f, 1.0f,         // intensidades
		0.0f, 0.0f, 0.0f,  // posición (encima del modelo Lamp_M)
		0.3f, 0.2f, 0.1f    // atenuación
	);
	pointLightCount++;


	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz cofre
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 
	// --- Luz roja carro---
	spotLights[2] = SpotLight(
		1.0f, 0.0f, 0.0f,   // color (rojo)
		0.5f, 0.8f,         // intensidades ambiental y difusa
		0.0f, 0.0f, 0.0f,   // posición inicial (se actualizará cada frame)
		0.0f, -1.0f, 0.0f,  // dirección (hacia abajo)
		1.0f, 0.0f, 0.0f,   // atenuación (alcance)
		15.0f               // ángulo del haz
	);
	spotLightCount++;

	//luz fija azul
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 carPosition;
	glm::vec3 lightDirection;
	glm::vec3 heliPosition;
	glm::vec3 heliDirection;
	glm::vec3 LampPosition;
	glm::vec3 OctaPosition;
	bool azulActiva = false;
	bool rojaActiva = false;
	// --- Luz del cofre ---
	glm::vec3 worldLightPos;
	glm::vec3 worldLightDir;
	// --- Luz del cofre ---
// Posición y dirección locales (en el espacio del cofre)
	glm::vec4 localLightPos;// w=1 → posición
	glm::vec4 localLightDir;// w=0 → dirección
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		//Se envía al shader el arreglo de luces puntuales, agregar el if para intercambiar arreglos

// --- Actualizar luces del auto ---
		carPosition = glm::vec3(mainWindow.getposicionCarro() - 2.5f, -0.15f, 0.0f);
		lightDirection = glm::vec3(-1.0f, -0.3f, 0.0f);

		heliPosition = glm::vec3(mainWindow.getposicionCarro() + 3.0f, -0.15f, 0.0f);
		heliDirection = glm::vec3(1.0f, -0.3f, 0.0f);

		// Bandera para seleccionar qué luz encender
		azulActiva = (mainWindow.getlucespuntuales() < 0.5f);
		rojaActiva = !azulActiva;

		// --- Luz azul ---
		if (azulActiva) {
			spotLights[3].SetDiffuseIntensity(1.0f);  // Encender 
			spotLights[3].SetAmbientIntensity(0.3f);  //  luz ambiental
			spotLights[3].SetFlash(carPosition, lightDirection);
		}
		else {
			spotLights[3].SetDiffuseIntensity(0.0f);  // Apagar
			spotLights[3].SetAmbientIntensity(0.0f);
		}

		// --- Luz roja ---
		if (rojaActiva) {
			spotLights[2].SetDiffuseIntensity(1.0f);
			spotLights[2].SetAmbientIntensity(0.2f);
			spotLights[2].SetFlash(heliPosition, heliDirection);
		}
		else {
			spotLights[2].SetDiffuseIntensity(0.0f);
			spotLights[2].SetAmbientIntensity(0.0f);
		}



		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

	//Muro 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

	//Carro base (X) y (C)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(mainWindow.getposicionCarro(), -0.95f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarroF_M.RenderModel();

		//LLanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.6f, 0.3f, -0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getanguloLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDer_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.5f, 0.3f, -0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getanguloLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDer_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.6f, 0.3f, 0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getanguloLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaIzq_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.5f, 0.3f, 0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getanguloLlantas()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaIzq_M.RenderModel();

		//Cofre (V) y (B)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.85f, 0.97f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		// --- Luz del cofre (desde adentro iluminando hacia arriba) ---
		// Coordenadas locales dentro del cofre
		localLightPos = glm::vec4(-0.7f, -0.2f, 0.0f, 1.0f);   // Un poco encima del fondo interno
		localLightDir = glm::vec4(-1.0f, -0.05f, 0.0f, 0.0f);   // Apunta hacia arriba y ligeramente adelante

		// Transformarlas al espacio del mundo con la misma rotación del cofre
		worldLightPos = glm::vec3(model * localLightPos);
		worldLightDir = glm::normalize(glm::vec3(model * localLightDir));

		// Configurar la luz
		spotLights[1].SetFlash(worldLightPos, worldLightDir);
		
		// --- Control de encendido ---
		// Obtener ángulo actual del cofre
		float apertura = mainWindow.getanguloCofre();

		// Si el cofre está cerrado, la luz se apaga (intensidad 0)
		if (apertura >= 0.0f) { // para "cerrado"
			spotLights[1].SetDiffuseIntensity(0.0f);
			spotLights[1].SetAmbientIntensity(0.0f);
		}
		// Si está abierto, encender la luz con intensidad máxima
		else {
			spotLights[1].SetDiffuseIntensity(2.5f);
			spotLights[1].SetAmbientIntensity(0.3f);
		}

		//Octaedro por código
		model = glm::mat4(1.0);
		OctaPosition = (glm::vec3(-1.5f, 4.5f, -2.0f));
		model = glm::translate(model, OctaPosition);
		pointLights[1].SetPos(OctaPosition);
		// --- Control de encendido / apagado ---
		if (mainWindow.getprendida() < 0.1f) {
			pointLights[1].SetDiffuseIntensity(1.0f);
			pointLights[1].SetAmbientIntensity(0.3f);
		}
		else {
			pointLights[1].SetDiffuseIntensity(0.0f);
			pointLights[1].SetAmbientIntensity(0.0f);
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		octaedroTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Lampara
		model = glm::mat4(1.0);
		LampPosition = (glm::vec3(-6.5f, -1.0f, -2.0f));
		model = glm::translate(model,LampPosition);
				// --- Luz  ---
		LampPosition.y += 4.0f;
		pointLights[0].SetPos(LampPosition);
		// --- Control de encendido / apagado ---
		if (mainWindow.getlampara() < 0.1f) {
			pointLights[0].SetDiffuseIntensity(1.0f);
			pointLights[0].SetAmbientIntensity(0.3f);
		}
		else {
			pointLights[0].SetDiffuseIntensity(0.0f);
			pointLights[0].SetAmbientIntensity(0.0f);
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
