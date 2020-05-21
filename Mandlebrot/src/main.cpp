#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define HEIGHT 1000
#define WIDTH  1000
#define HEIGHT_F 1000.0f
#define WIDTH_F  1000.0f

#define SCROLL_SENSITVITY 0.5

glm::dvec2 center(0.0);
double range = 4.0;

bool update = false;


void framebuffer_resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void clear_error()
{
	while (GL_NO_ERROR != glGetError());
}

void print_any_error()
{
	unsigned int err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL Error " << err << std::endl;
	}
}

float map(float value, float min1, float max1, float min2, float max2)
{
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

double map(double value, double min1, double max1, double min2, double max2)
{
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void mouse_scroll(GLFWwindow* window, double xoff, double yoff)
{
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);

	if (yoff > 0.0) { // Zoomed in
		double off = range / 2.0;
		double real = map(xpos, 0.0, (double)WIDTH_F, center.x - off, center.x + off);
		double img = map(ypos, 0.0, (double)HEIGHT_F, center.y + off, center.y - off);
		center.x = real;
		center.y = img;
		range *= SCROLL_SENSITVITY;
		update = true;
	} else if (yoff < 0.0) { // Zoomed out

		if (range >= 4.0)
			return;

		double off = range / 2.0;
		double real = map(xpos, 0.0, (double)WIDTH_F, center.x - off, center.x + off);
		double img = map(ypos, 0.0, (double)HEIGHT_F, center.y + off, center.y - off);
		center.x = real;
		center.y = img;

		range /= SCROLL_SENSITVITY;
		update = true;
	}
}

void mouse_button_cb(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)   return;

    static double xpos_pressed = 0.0;
    static double ypos_pressed = 0.0;

    if (action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xpos_pressed, &ypos_pressed);
    } else {
        double xpos_released;
        double ypos_released;
        glfwGetCursorPos(window, &xpos_released, &ypos_released);
        // this means that the action was release, calculate and update the translation
        double translatedx = (2.0 * (xpos_released - xpos_pressed) / (double)WIDTH_F);
        double translatedy = (2.0 * (ypos_released - ypos_pressed) / (double)HEIGHT_F);
        center -= glm::dvec2(translatedx * range, -translatedy * range);
        update = true;
    }
}

int main()
{
	if (!glfwInit()) {
		std::cout << "glfwInitialization Failed!" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandlebrot", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD!" << std::endl;
		glfwTerminate();
		return -2;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize);
    glfwSetMouseButtonCallback(window, mouse_button_cb);
	//glfwSetCursorPosCallback(window, mouse_moved);
	glfwSetScrollCallback(window, mouse_scroll);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float verticies[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,

		 1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);

	Shader* shader = new Shader("./Mandlebrot/src/mandle.vs", "./Mandlebrot/src/mandle.fs");
	shader->Bind();
	shader->UploadUniform2d("resolution", WIDTH_F, HEIGHT_F);
	shader->UploadUniform2d("center", center.x, center.y);
	shader->UploadUniform1d("range", range);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.2f, 0.18f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Object draw call
		glBindVertexArray(vao);
		shader->Bind();

		if (update) {
			shader->UploadUniform2d("center", center.x, center.y);
			shader->UploadUniform1d("range", range);
			update = false;
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}
