///////////////////////////////////////////////////////////////////////
//
// P3D Course
// (c) 2016 by João Madeiras Pereira
// TEMPLATE: Whitted Ray Tracing NFF scenes and drawing points with Modern OpenGL
//
// You should develop your rayTracing( Ray ray, int depth, float RefrIndex) which returns a color and
// to develop your load_NFF function
//
///////////////////////////////////////////////////////////////////////

// Group 9

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <ctime>

#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"

#include "utils.h"
#include "scene.h"
#include "colors.h"
#include "primitives.h"
#include "ray.h"
#include "configutil.h"

#define CAPTION "ray tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1

// Points defined by 2 attributes: positions which are stored in vertices array and colors which are stored in colors array
float *colors;
float *vertices;

int size_vertices;
int size_colors;

GLfloat m[16];  //projection matrix initialized by ortho function

GLuint VaoId;
GLuint VboId[2];

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

Scene* scene = NULL;
int RES_X, RES_Y;

std::clock_t start_time, end_time;

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
int draw_mode = 1;

int WindowHandle = 0;

///////////////////////////////////////////////////////////////////////  RAY-TRACE SCENE

Color rayTracing(Ray ray, int depth, float RefrIndex)
{
	Collision col = scene->castRay(ray);
	if (col.object == nullptr) {
		return *(scene->background);
	}

	Color color = { 0, 0, 0 };
	
	for (Light* light : scene->lights) {
		int n_samples = (!ANTI_ALIASING) ? SAMPLING_RATE : 1;
		Color lightC = { 0,0,0 };

		for (int i = 0; i < n_samples; i++) {
			Vector3 sampleDirection = normalize(subVector(light->getLightSample(), col.point));
			if (internalProduct(sampleDirection, col.normal) <= 0) {
				// The Light is behind our object.
				continue;
			}

			Ray shadow_feeler = createRay(sampleDirection, addVector(col.point, vector3MultScalar(sampleDirection, EPSILON)));
			Collision shadow = scene->castRay(shadow_feeler);

			// If the shadow feeler did not collide or if the collision was further away than the light source.
			//Blinn Phong model
			if (shadow.object == nullptr || vector3Length(subVector(shadow_feeler.origin, shadow.point)) > vector3Length(subVector(light->pos, col.point))) {
				Vector3 halfway = normalize(addVector(sampleDirection, vector3MultScalar(ray.versor, -1)));

				float diffuse_c = clamp(col.object->_matProps->diffuseComp * internalProduct(col.normal, sampleDirection), 0, 1);
				Color diffuse = colorTimesConstant(col.object->_matProps->color, diffuse_c);
				lightC = addColors(lightC, diffuse);

				if (col.object->_matProps->specularComp > 0) {
					float specular_c = col.object->_matProps->specularComp * pow(internalProduct(col.normal, halfway), col.object->_matProps->shine);
					Color specular = colorTimesConstant(light->color, specular_c);
					lightC = addColors(lightC, specular);
				}
			}
		}

		// Average the colors from all samples and add it to the color result.
		lightC = colorTimesConstant(lightC, 1.0f / n_samples);
		color = addColors(color, lightC);
	}

	// Calculate Reflections.
	if (col.object->_matProps->specularComp > 0 && depth < DEPTH_TRACE_LIMIT) {
		Vector3 versor = normalize(subVector(ray.versor, vector3MultScalar(col.normal, 2 * internalProduct(ray.versor, col.normal))));
		Ray reflector = createRay(versor, addVector(col.point, vector3MultScalar(versor, EPSILON)));

		Color reflected = colorTimesConstant(rayTracing(reflector, depth + 1, RefrIndex), col.object->_matProps->specularComp);
		color = addColors(color, reflected);
	}

	// Calculate Refractions using Snell-Decarte law.
	if (col.object->_matProps->t > 0 && depth < DEPTH_TRACE_LIMIT) {
		Ray refractor;

		float origin_ior = RefrIndex;
		float dest_ior = col.inside ? 1.0f : col.object->_matProps->idxOfRefraction;
		Vector3 v = vector3MultScalar(ray.versor, -1);
		float ior = origin_ior / dest_ior;
		Vector3 vt = subVector(vector3MultScalar(col.normal, internalProduct(v, col.normal)), v);
		float sin_theta_t = ior * vector3Length(vt);
		float cos_theta_t = sqrt(1 - sin_theta_t * sin_theta_t);
		Vector3 t = normalize(vt);

		Vector3 versor = normalize(addVector(vector3MultScalar(t, sin_theta_t), vector3MultScalar(col.normal, -cos_theta_t)));
		refractor = createRay(versor, addVector(col.point, vector3MultScalar(versor, EPSILON)));

		Color refracted = colorTimesConstant(rayTracing(refractor, depth + 1, dest_ior), col.object->_matProps->t);
		color = addColors(color, refracted);
	}

	color = { clamp(color.r, 0, 1), clamp(color.g, 0, 1), clamp(color.b, 0, 1) };
	return color;
}

Ray calculatePrimaryRay(float x, float y, Camera* cam) {
	float df = cam->GetHither();
	float w = cam->GetPlaneWidth();
	float h = cam->GetPlaneHeight();

	Vector3 rx = cam->GetRefX();
	Vector3 ry = cam->GetRefY();
	Vector3 rz = cam->GetRefZ();

	float cx = w * (x / cam->GetResX() - 0.5);
	float cy = h * (y / cam->GetResY() - 0.5);
	float cz = -df;

	Vector3 p = cam->GetPointInFocalPlane({ cx, cy, cz });
	Vector3 ls = cam->GetSample();

	float tx = p.x - ls.x;
	float ty = p.y - ls.y;
	float tz = - cam->focal_distance;

	Vector3 vx = vector3MultScalar(rx, tx);
	Vector3 vy = vector3MultScalar(ry, ty);
	Vector3 vz = vector3MultScalar(rz, tz);

	Vector3 ox = vector3MultScalar(rx, ls.x);
	Vector3 oy = vector3MultScalar(ry, ls.y);

	Vector3 o = addVector(cam->GetFrom(), addVector(ox, oy));

	Vector3 d = addVector(vx, addVector(vy, vz));

	Ray ray = createRay(normalize(d), o);	
	return ray;
}

Color calculatePixelColor(int x,int y,Camera* cam) {

	Color color = { 0,0,0 };

	cam->GenerateSamples(SAMPLING_RATE);
	switch (SAMPLE_MODE) {  //0 for regular sampling, 1 for Jittering sampling, 2 for Stochastic sampling
	case 0: //0 for regular sampling
		for (int q = 0; q < SAMPLING_RATE; q++)
		{
			for (int p = 0; p < SAMPLING_RATE; p++)
			{
				float newx = x + (float) (q + 0.5) / SAMPLING_RATE;
				float newy = y + (float) (p + 0.5) / SAMPLING_RATE;
				Ray ray = calculatePrimaryRay(newx, newy, cam);
				color = addColors(color, rayTracing(ray, 1, 1.0));
			}
		}
		return colorTimesConstant(color, 1.0f / (SAMPLING_RATE*SAMPLING_RATE));
		break;
	case 1:	//1 for Jittering sampling	
		for (int q = 0; q < SAMPLING_RATE; q++)
		{
			for (int p = 0; p < SAMPLING_RATE; p++)
			{
				float minq = x + (float)q / SAMPLING_RATE;
				float maxq = x + (float)(q + 1) / SAMPLING_RATE;
				float minp = y + (float)p / SAMPLING_RATE;
				float maxp = y + (float)(p + 1) / SAMPLING_RATE;
				float newx = generateRandom(minq, maxq );
				float newy = generateRandom(minp, maxp);
				Ray ray = calculatePrimaryRay(newx, newy, cam);
				color = addColors(color,rayTracing(ray, 1, 1.0));
			}
		}
		return colorTimesConstant(color, 1.0f / (SAMPLING_RATE*SAMPLING_RATE));
	case 2: //2 for Stochastic sampling
		for (int q = 0; q < SAMPLING_RATE * SAMPLING_RATE; q++)
		{
			float newx = x + generateRandom(0, 1);
			float newy = y + generateRandom(0, 1);
			Ray ray = calculatePrimaryRay(newx, newy, cam);
			color = addColors(color, rayTracing(ray, 1, 1.0));
		}
		return colorTimesConstant(color, 1.0f / (SAMPLING_RATE*SAMPLING_RATE));
	}
}

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}
/////////////////////////////////////////////////////////////////////// SHADERs
const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec2 in_Position;\n"
	"in vec3 in_Color;\n"
	"uniform mat4 Matrix;\n"
	"out vec4 color;\n"

	"void main(void)\n"
	"{\n"
	"	vec4 position = vec4(in_Position, 0.0, 1.0);\n"
	"	color = vec4(in_Color, 1.0);\n"
	"	gl_Position = Matrix * position;\n"

	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = color;\n"
	"}\n"
};

void createShaderProgram()
{
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
	glBindAttribLocation(ProgramId, COLOR_ATTRIB, "in_Color");

	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}
/////////////////////////////////////////////////////////////////////// VAOs & VBOs
void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	glGenBuffers(2, VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

	/* Não é necessário fazer glBufferData, ou seja o envio dos pontos para a placa gráfica pois isso
	é feito na drawPoints em tempo de execução com GL_DYNAMIC_DRAW */

	glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VERTEX_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glEnableVertexAttribArray(COLOR_ATTRIB);
	glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, 0, 0, 0);

	// unbind the VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	//glDisableVertexAttribArray(COLOR_ATTRIB);
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(COLOR_ATTRIB);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void drawPoints()
{
	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, size_colors, colors, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(UniformId, 1, GL_FALSE, m);
	glDrawArrays(GL_POINTS, 0, RES_X*RES_Y);
	glFinish();

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

// Render function by primary ray casting from the eye towards the scene's objects

void renderScene()
{
	int index_pos = 0;
	int index_col = 0;
	Color color;
	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{
			// Prepare the lights to be rendered.
			for (Light* l : scene->lights) {
				l->generateSamples(SAMPLING_RATE);
			}
			// Prepare the camera samples
			scene->GetCamera()->GenerateSamples(SAMPLING_RATE);

			//YOUR 2 FUNTIONS:
			if (ANTI_ALIASING) {
				color = calculatePixelColor(x, y, scene->GetCamera());
			}
			else {
				Ray ray = calculatePrimaryRay(x + 0.5, y + 0.5, scene->GetCamera());
				color = rayTracing(ray, 1, 1.0);
			}

			vertices[index_pos++] = (float)x;
			vertices[index_pos++] = (float)y;
			colors[index_col++] = (float)color.r;
			colors[index_col++] = (float)color.g;
			colors[index_col++] = (float)color.b;

			if (draw_mode == 0) {  // desenhar o conteúdo da janela ponto a ponto
				drawPoints();
				index_pos = 0;
				index_col = 0;
			}
		}
		//printf("line %d", y);
		if (draw_mode == 1) {  // desenhar o conteúdo da janela linha a linha
			drawPoints();
			index_pos = 0;
			index_col = 0;
		}
	}

	if (draw_mode == 2) //preenchar o conteúdo da janela com uma imagem completa
		drawPoints();

	end_time = std::clock();
	printf("Done: Took %.3f seconds.", double(end_time - start_time) / CLOCKS_PER_SEC);
}

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void ortho(float left, float right, float bottom, float top,
	float nearp, float farp)
{
	m[0 * 4 + 0] = 2 / (right - left);
	m[0 * 4 + 1] = 0.0;
	m[0 * 4 + 2] = 0.0;
	m[0 * 4 + 3] = 0.0;
	m[1 * 4 + 0] = 0.0;
	m[1 * 4 + 1] = 2 / (top - bottom);
	m[1 * 4 + 2] = 0.0;
	m[1 * 4 + 3] = 0.0;
	m[2 * 4 + 0] = 0.0;
	m[2 * 4 + 1] = 0.0;
	m[2 * 4 + 2] = -2 / (farp - nearp);
	m[2 * 4 + 3] = 0.0;
	m[3 * 4 + 0] = -(right + left) / (right - left);
	m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	m[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
	m[3 * 4 + 3] = 1.0;
}

void reshape(int w, int h)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	ortho(0, (float)RES_X, 0, (float)RES_Y, -1.0, 1.0);
}

/////////////////////////////////////////////////////////////////////// SETUP
void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowPosition(640, 100);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glDisable(GL_DEPTH_TEST);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	loadConfig();

	//INSERT HERE YOUR CODE FOR PARSING NFF FILES
	scene = new Scene(GRID_OPTIMIZATION);
	srand(time(0));

	if (!(scene->load_nff(NFF_FILE.c_str()))) return 0;
	if (GRID_OPTIMIZATION) scene->initializeGrid();

	std::cout <<  "Drawing " << scene->primitives.size() << " objects.\n";

	RES_X = scene->GetCamera()->GetResX();
	RES_Y = scene->GetCamera()->GetResY();

	start_time = std::clock();

	if (draw_mode == 0) { // desenhar o conteúdo da janela ponto a ponto
		size_vertices = 2 * sizeof(float);
		size_colors = 3 * sizeof(float);
		printf("DRAWING MODE: POINT BY POINT\n");
	}
	else if (draw_mode == 1) { // desenhar o conteúdo da janela linha a linha
		size_vertices = 2 * RES_X * sizeof(float);
		size_colors = 3 * RES_X * sizeof(float);
		printf("DRAWING MODE: LINE BY LINE\n");
	}
	else if (draw_mode == 2) { // preencher o conteúdo da janela com uma imagem completa
		size_vertices = 2 * RES_X*RES_Y * sizeof(float);
		size_colors = 3 * RES_X*RES_Y * sizeof(float);
		printf("DRAWING MODE: FULL IMAGE\n");
	}
	else {
		printf("Draw mode not valid \n");
		exit(0);
	}
	printf("resx = %d  resy= %d.\n", RES_X, RES_Y);

	vertices = (float*)malloc(size_vertices);
	if (vertices == NULL) exit(1);

	colors = (float*)malloc(size_colors);
	if (colors == NULL) exit(1);

	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}
///////////////////////////////////////////////////////////////////////