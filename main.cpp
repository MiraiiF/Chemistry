#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void callback_redimensionamento(GLFWwindow* window, int width, int height);

int main(void){
    GLFWwindow* janela;

    if (!glfwInit()) {
		printf("GLFW nono\n");
		return -1;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int largura = 1280, altura = 720;
	janela = glfwCreateWindow(largura, altura, "Optics", NULL, NULL);
	if (!janela) {
		printf("janela nao foi criada...\n");
		glfwTerminate();
		return -1;
	}

    glfwMakeContextCurrent(janela);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("no glad...");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, largura, altura);
    glClearColor(0.1, 0.0, 0.1, 1.0);
	glfwSetFramebufferSizeCallback(janela, callback_redimensionamento); 

	while (!glfwWindowShouldClose(janela))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(janela);
		glfwPollEvents();
	}

	glfwTerminate();
}


void callback_redimensionamento(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  