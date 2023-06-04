#include "Libraries/Camera.h"
#include "Libraries/Shader.h"
#include "Libraries/texture.h"
#include "Libraries/Desenhos.h"

bool firstcursor = true;
float lastx = 640, lasty = 360;
GLfloat yaw = 180, pitch = 0;
int largura = 1280, altura = 720;


void callback_redimensionamento(GLFWwindow* window, int width, int height);
void callback_cursor(GLFWwindow* janela, double xpos, double ypos);
void callback_CloseWindow(GLFWwindow* janela);
void constrain(float* value, float lim_up, float lim_down);
void up_down(GLFWwindow* janela, float* var, float val, int key_up, int key_down, float lim_up, float lim_down);
void camera_movement(GLFWwindow* janela, float velocity, Camera viewer, glm::vec3 *position);

int main(void){
    GLFWwindow* janela;

    if (!glfwInit()) {
		printf("GLFW nono\n");
		return -1;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	glfwSetCursorPosCallback(janela, callback_cursor);
	glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader Base("../../Shaders/BaseShader.vert", "../../Shaders/BaseShader.frag");

	unsigned int VAOMirror, VBOMirror;
	glGenVertexArrays(1, &VAOMirror);
	glGenBuffers(1, &VBOMirror);
	glBindVertexArray(VAOMirror);

	float MirrorArray[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBOMirror);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MirrorArray), MirrorArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (void*)0);	
	glBindVertexArray(0);

	unsigned int VAOatom, VBOatom, EBOatom;
	glGenVertexArrays(1, &VAOatom);
	glGenBuffers(1, &VBOatom);
	glGenBuffers(1, &EBOatom);
	glBindVertexArray(VAOatom);

	std::vector<unsigned int> atomIndices;
	std::vector<float> atomBuffer = CriaEsfera(20, 20, 3.0f, &atomIndices);

	glBindBuffer(GL_ARRAY_BUFFER, VBOatom);
	glBufferData(GL_ARRAY_BUFFER, atomBuffer.size() * sizeof(float), atomBuffer.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOatom);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, atomIndices.size() * sizeof(float), atomIndices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(janela))
	{
		callback_CloseWindow(janela);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)(largura)/(GLfloat)(altura), 0.01f, 50.0f);
		Base.use();
		Base.setmat4("proj", projection);
		Camera viewer = Camera(pos, yaw, pitch);
		Base.setmat4("view", viewer.view);
		camera_movement(janela, 0.005, viewer, &pos);
		glBindVertexArray(VAOatom);
		glDrawElements(GL_TRIANGLES, (unsigned int)atomIndices.size(), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(janela);
		glfwPollEvents();
	}

	glfwTerminate();
}


void callback_redimensionamento(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
	largura = width;
	altura = height;
}  

void callback_cursor(GLFWwindow* janela, double xpos, double ypos){
	if (firstcursor) {
		lastx = xpos; lasty = ypos;
		firstcursor = false;
	}
	float deltax = xpos - lastx; float deltay = ypos - lasty;
	lastx = xpos; lasty = ypos;

	const float sensitivity = 0.05f;
	deltax *= sensitivity;
	deltay *= sensitivity;

	yaw -= deltax; pitch -= deltay;
	constrain(&pitch, 89.999f, -89.999f);
}

void constrain(float* value, float lim_up, float lim_down) {
	if (*value > lim_up) {
		*value = lim_up;
	}
	if (*value < lim_down) {
		*value = lim_down;
	}
}

void up_down(GLFWwindow* janela, float* var, float val, int key_up, int key_down, float lim_up, float lim_down) {
	if (glfwGetKey(janela, key_up) == GLFW_PRESS) {
		*var += val;
	}
	else if (glfwGetKey(janela, key_down) == GLFW_PRESS) {
		*var -= val;
	}
	constrain(var, lim_up, lim_down);
}

void callback_CloseWindow(GLFWwindow* janela) {
	if (glfwGetKey(janela, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(janela,true);
	}
}

void camera_movement(GLFWwindow* janela, float velocity, Camera viewer, glm::vec3 *position){
	if(glfwGetKey(janela, GLFW_KEY_D) == GLFW_PRESS){
		*position -= velocity * viewer.left;
	}
	else if(glfwGetKey(janela, GLFW_KEY_A) == GLFW_PRESS){
		*position += velocity * viewer.left;
	}

	if(glfwGetKey(janela, GLFW_KEY_W) == GLFW_PRESS){
		*position += velocity * viewer.front;
	}
	else if(glfwGetKey(janela, GLFW_KEY_S) == GLFW_PRESS){
		*position -= velocity * viewer.front;
	}

	if(glfwGetKey(janela, GLFW_KEY_Q) == GLFW_PRESS){
		*position += velocity * viewer.up;
	}
	else if(glfwGetKey(janela, GLFW_KEY_E) == GLFW_PRESS){
		*position -= velocity * viewer.up;
	}
}