/*
����5.3.3-1 main.cpp
ʵ����һ������׷�ٳ�������������һ�������һ���ڰ����̸�

�������ߣ�
Microsoft Visual Studio 2019

OpenGL��汾��
GLFW-3.2.1
GLAD-0.1.29

��Դ��汾��
OpenGL Mathematics(glm)-0.9.9.5 (https://github.com/g-truc/glm/releases/tag/0.9.9.5)

*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <functional>
#include <cmath>
#include <vector>
#include <iostream>

#include "Scene.hpp"
#include "Shader.hpp"
#include "DirLight.hpp"


// ��ɫ��Դ����
std::string vertexShader, fragmentShader;
// д����ɫ��Դ����
void writeShader(std::string& vertexShader, std::string& fragmentShader);

// ��ʼ��OpenGL����Ҫ�ĺ���������
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
void resizeGL(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// �任����
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
// �۲��λ�ü��۲췽��
glm::vec3 viewPos = glm::vec3(0.0f, 2.0f, 3.0f);
glm::vec3 viewFront = glm::vec3(0, 0, -1);  //����Ļ�����һ�����ڹ۲��ǰ����������
glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

// ������ʹ�õĳ���
RayTracing::Scene scene;

int main()
{
	// ��ʼ��OpenGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, u8"Ray Tracing", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeGL);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ��ȡ��ɫ��
	// ��ɫ������Ļ�ϵ���������ɫ
	writeShader(vertexShader, fragmentShader);
	Shader shader(vertexShader.c_str(), fragmentShader.c_str());

	// ������Ϊ��ɫ����
	float point[] = { 0, 0, 0 };

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ���ù��ߡ�ƽ�桢����Ĳ������������Ǽ��볡��

	// ���ù��߲�������ƽ����볡��
	scene.addLight(new DirLight(
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -1.0f, -1.0f)
	));

	// ����ƽ�����

	// ����ƽ�����
	Material planeMaterial;
	// ������ɫ�ͷ���ı���
	planeMaterial.kShade = 0.7f;
	planeMaterial.kReflect = 0.3f;
	planeMaterial.kRefract = 0.0f;
	// �������̸�Ĳ�����ɫ���㺯������������������ӵ�x,z���귵���Ǻ��ǰ�
	auto isBlack = [](const glm::vec3& pos)
	{
		return fmod(floor(pos.x) + floor(pos.z), 2) == 0;
	};
	planeMaterial.ambient = [=](const glm::vec3& pos)->glm::vec3
	{
		glm::vec3 color(1.0f, 1.0f, 1.0f);
		if (isBlack(pos))
		{
			return color;
		}
		else
		{
			return glm::vec3(1.0f, 1.0f, 1.0f) - color;
		}
	};
	planeMaterial.diffuse = [=](const glm::vec3& pos)->glm::vec3
	{
		glm::vec3 color(1.0f, 1.0f, 1.0f);
		if (isBlack(pos))
		{
			return color;
		}
		else
		{
			return glm::vec3(1.0f, 1.0f, 1.0f) - color;
		}
	};
	planeMaterial.specular = [=](const glm::vec3& pos)->glm::vec3
	{
		glm::vec3 color(1.0f, 1.0f, 1.0f);
		if (isBlack(pos))
		{
			return color;
		}
		else
		{
			return glm::vec3(1.0f, 1.0f, 1.0f) - color;
		}
	};
	planeMaterial.shininess = [](const glm::vec3& pos)->float
	{
		return 32.0f;
	};
	// ����ƽ��λ�á�������������
	RayTracing::Plane* plane= new RayTracing::Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	plane->setMaterial(planeMaterial);
	// ��ƽ����볡��
	scene.addEntity(plane);

	// ����ǽ��

	// ����ǽ�����
	Material wallMaterial;
	// ������ɫ�ͷ���ı���
	wallMaterial.kShade = 0.9f;
	wallMaterial.kReflect = 0.1f;
	wallMaterial.kRefract = 0.0f;
	wallMaterial.ambient = [=](const glm::vec3& pos)->glm::vec3
		{
			glm::vec3 color(1.0f, 0.0f, 0.0f);
			return color;
		};
	wallMaterial.diffuse = [=](const glm::vec3& pos)->glm::vec3
		{
			glm::vec3 color(0.5f, 0.5f, 0.5f);
			return color;
		};
	wallMaterial.specular = [=](const glm::vec3& pos)->glm::vec3
		{
			glm::vec3 color(0.5f, 0.5f, 0.5f);
			return color;
		};
	wallMaterial.shininess = [](const glm::vec3& pos)->float
		{
			return 32.0f;
		};
	// ����ǽ��λ�á�������������
	RayTracing::Plane* wall = new RayTracing::Plane(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wall->setMaterial(wallMaterial);
	// ��ǽ����볡��
	scene.addEntity(wall);

	// �����������

	// �����������
	Material ballMaterial;
	// ������ɫ�ͷ���ı���
	ballMaterial.kShade = 0.8f;
	ballMaterial.kReflect = 0.2f;
	ballMaterial.kRefract = 0.0f;
	ballMaterial.refractiveIndex = 1.5f;

	// �������������ɫ���㺯�������к�������һ������
	ballMaterial.ambient = [](const glm::vec3& pos)->glm::vec3
	{
		return { 1.0f, 1.0f, 1.0f };
	};
	ballMaterial.diffuse = [](const glm::vec3& pos)->glm::vec3
	{
		return { 1.0f, 1.0f, 1.0f };
	};
	ballMaterial.specular = [](const glm::vec3& pos)->glm::vec3
	{
		return  { 0.6f, 0.6f, 0.6f };
	};
	ballMaterial.shininess = [](const glm::vec3& pos)->float
	{
		return 32.0f;
	};
	// ��������λ�á��뾶������
	auto ball = new RayTracing::Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
	ball->setMaterial(ballMaterial);
	// ��������볡��
	scene.addEntity(ball);

	// ��������2����

	// �����������
	Material ballMaterial2;
	// ������ɫ�ͷ���ı���
	ballMaterial2.kShade = 0.2f;
	ballMaterial2.kReflect = 0.2f;
	ballMaterial2.kRefract = 0.6f;
	ballMaterial2.refractiveIndex = 1.5f;

	// �������������ɫ���㺯�������к�������һ������
	ballMaterial2.ambient = [](const glm::vec3& pos)->glm::vec3
		{
			return { 0.1f, 1.0f, 1.0f };
		};
	ballMaterial2.diffuse = [](const glm::vec3& pos)->glm::vec3
		{
			return { 0.1f, 1.0f, 1.0f };
		};
	ballMaterial2.specular = [](const glm::vec3& pos)->glm::vec3
		{
			return  { 0.6f, 0.6f, 0.6f };
		};
	ballMaterial2.shininess = [](const glm::vec3& pos)->float
		{
			return 50.0f;
		};
	// ��������λ�á��뾶������
	auto ball2 = new RayTracing::Sphere(glm::vec3(-2.5f, 1.0f, 0.0f), 1.0f);
	ball2->setMaterial(ballMaterial2);
	// ��������볡��
	scene.addEntity(ball2);

	// ��������3����

	// �����������
	Material ballMaterial3;
	// ������ɫ�ͷ���ı���
	ballMaterial3.kShade = 0.99f;
	ballMaterial3.kReflect = 0.01f;
	ballMaterial3.kRefract = 0.0f;
	ballMaterial3.refractiveIndex = 1.5f;

	// �������������ɫ���㺯�������к�������һ������
	ballMaterial3.ambient = [](const glm::vec3& pos)->glm::vec3
		{
			return { 0.1f, 1.0f, 1.0f };
		};
	ballMaterial3.diffuse = [](const glm::vec3& pos)->glm::vec3
		{
			return { 0.1f, 1.0f, 1.0f };
		};
	ballMaterial3.specular = [](const glm::vec3& pos)->glm::vec3
		{
			return  { 0.6f, 0.6f, 0.6f };
		};
	ballMaterial3.shininess = [](const glm::vec3& pos)->float
		{
			return 32.0f;
		};
	// ��������λ�á��뾶������
	auto ball3 = new RayTracing::Sphere(glm::vec3(2.5f, 1.0f, 0.0f), 1.0f);
	ball3->setMaterial(ballMaterial3);
	// ��������볡��
	scene.addEntity(ball3);


	while (!glfwWindowShouldClose(window))
	{
		// ����������Ϣ����ʼ�����塢�任����
		processInput(window);

		glClearColor(0.3, 0.3, 0.3, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		model = glm::mat4(1.0f);
		view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
		projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

		// ��ʼ���۲�������
		glm::vec3 viewRight = glm::normalize(glm::cross(viewFront, viewUp));

		// ��������Ϊ���ƶ���
		shader.use();
		glBindVertexArray(VAO);

		// ö����Ļ��ÿһ������
		for (unsigned int i = 0; i < SCR_WIDTH; i++)
		{
			for (unsigned int j = 0; j < SCR_HEIGHT; j++)
			{
				// �������������ӳ�䵽[-1, 1]
				glm::vec3 pos(float(i) * 2 / SCR_WIDTH - 1.0f, float(j) * 2/ SCR_HEIGHT - 1.0f, 0.0f);
				shader.setVec2("screenPos", pos.x, pos.y);

				// �������������������е�λ��
				glm::vec3 globalPos = viewPos + viewFront + pos.x * viewRight * (float(SCR_WIDTH) / SCR_HEIGHT) + pos.y * viewUp;

				// ��������߲����й���׷��
				RayTracing::Ray ray(viewPos, globalPos);
				glm::vec3 color = scene.traceRay(ray);

				// ���Ƹô�������
				shader.setVec3("vertexColor", color);
				glDrawArrays(GL_POINTS, 0, 1);
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void resizeGL(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


void writeShader(std::string& vertexShader, std::string& fragmentShader)
{
	vertexShader = std::string("\
#version 330 core\n\
\n\
uniform vec2 screenPos;\n\
\n\
void main()\n\
{\n\
    gl_Position = vec4(screenPos, -1.0, 1.0);\n\
}\n\
\n\
");

	fragmentShader = std::string("\
#version 330 core\n\
\n\
out vec4 FragColor;\n\
uniform vec3 vertexColor;\n\
\n\
void main()\n\
{\n\
	FragColor = vec4(vertexColor, 1.0);\n\
}\n\
");
}

