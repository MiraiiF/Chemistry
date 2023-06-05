#include "Libraries/Camera.h"
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

	janela = glfwCreateWindow(largura, altura, "Chemistry", NULL, NULL);
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
	Shader Atom("../../Shaders/Atom.vert", "../../Shaders/Atom.frag");

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

	unsigned int VAOcilindro[3], VBOcilindro[3], EBOcilindro;
	glGenVertexArrays(3, VAOcilindro);
	glGenBuffers(3, VBOcilindro);
	glGenBuffers(1, &EBOcilindro);
	glBindVertexArray(VAOcilindro[0]);

	int nSetores = 10;
	float cilinderRadius = 0.20f;
	float cilinderHeight = 5.0f;
	float HALF_CI_HEIGHT = 2.5f;
	std::vector<float> circleBuffer1 = CriaCirculo(cilinderRadius, nSetores, HALF_CI_HEIGHT);

	glBindBuffer(GL_ARRAY_BUFFER, VBOcilindro[0]);
	glBufferData(GL_ARRAY_BUFFER, circleBuffer1.size() * sizeof(float), circleBuffer1.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glBindVertexArray(VAOcilindro[1]);

	std::vector<float> circleBuffer2 = CriaCirculo(cilinderRadius, nSetores, -HALF_CI_HEIGHT);

	glBindBuffer(GL_ARRAY_BUFFER, VBOcilindro[1]);
	glBufferData(GL_ARRAY_BUFFER, circleBuffer2.size() * sizeof(float), circleBuffer2.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glBindVertexArray(VAOcilindro[2]);

	std::vector<unsigned int> cilindroIndices;
	std::vector<float> cilindroBuffer = Cria_CorpodeCilindro(nSetores, cilinderRadius, cilinderHeight, &cilindroIndices);

	glBindBuffer(GL_ARRAY_BUFFER, VBOcilindro[2]);
	glBufferData(GL_ARRAY_BUFFER, cilindroBuffer.size() * sizeof(float), cilindroBuffer.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOcilindro);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cilindroIndices.size() * sizeof(float), cilindroIndices.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec4 color;

	while (!glfwWindowShouldClose(janela))
	{
		callback_CloseWindow(janela);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)(largura)/(GLfloat)(altura), 0.01f, 50.0f);
		Atom.use();
		Atom.setmat4("proj", projection);
		Camera viewer = Camera(pos, yaw, pitch);
		Atom.setmat4("view", viewer.view);
		Atom.setmat4("model", model);
		float velocity = glfwGetTime()/32;
		camera_movement(janela, velocity, viewer, &pos);


		color = OxygenColor;
		Atom.setvec4("color", color);
		Desenha_Esfera(VAOatom, atomIndices);


		model = glm::translate(model, glm::vec3(-3.0f, -1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.5f));
		Atom.setmat4("model", model);
		color = BondColor;
		Atom.setvec4("color", color);
		Desenha_Cilindro(VAOcilindro[2], VAOcilindro[0], VAOcilindro[1], cilindroIndices);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, -1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, -0.5f));
		Atom.setmat4("model", model);
		Desenha_Cilindro(VAOcilindro[2], VAOcilindro[0], VAOcilindro[1], cilindroIndices);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.5f, -2.0f, -0.35f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		Atom.setmat4("model", model);
		color = HydrogenColor;
		Atom.setvec4("color", color);
		Desenha_Esfera(VAOatom, atomIndices);
		

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.5f, -2.0f, -0.35f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		Atom.setmat4("model", model);
		Desenha_Esfera(VAOatom, atomIndices);

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