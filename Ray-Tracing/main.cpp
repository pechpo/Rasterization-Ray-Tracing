/*
例题5.3.3-1 main.cpp
实现了一个光线追踪场景，场景中有一个球体和一个黑白棋盘格。

开发工具：
Microsoft Visual Studio 2019

OpenGL库版本：
GLFW-3.2.1
GLAD-0.1.29

开源库版本：
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


// 着色器源代码
std::string vertexShader, fragmentShader;
// 写入着色器源代码
void writeShader(std::string& vertexShader, std::string& fragmentShader);

// 初始化OpenGL所需要的函数及变量
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
void resizeGL(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 变换矩阵
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
// 观察点位置及观察方向
glm::vec3 viewPos = glm::vec3(0.0f, 2.0f, 3.0f);
glm::vec3 viewFront = glm::vec3(0, 0, -1);  //把屏幕想象成一个放在观察点前的像素阵列
glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 本程序使用的场景
RayTracing::Scene scene;

int main()
{
	// 初始化OpenGL
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

	// 读取着色器
	// 着色器对屏幕上单个像素着色
	writeShader(vertexShader, fragmentShader);
	Shader shader(vertexShader.c_str(), fragmentShader.c_str());

	// 将点设为着色对象
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

	// 设置光线、平面、球体的参数，并把它们加入场景

	// 设置光线参数并把平面加入场景
	scene.addLight(new DirLight(
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.6f, 0.6f, 0.6f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f, -1.0f, -1.0f)
	));

	// 设置平面参数

	// 设置平面材质
	Material planeMaterial;
	// 设置着色和反射的比例
	planeMaterial.kShade = 0.7f;
	planeMaterial.kReflect = 0.3f;
	planeMaterial.kRefract = 0.0f;
	// 设置棋盘格的材质颜色计算函数，函数根据这个格子的x,z坐标返回是黑是白
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
	// 设置平面位置、法向量、材质
	RayTracing::Plane* plane= new RayTracing::Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	plane->setMaterial(planeMaterial);
	// 把平面加入场景
	scene.addEntity(plane);

	// 设置墙体

	// 设置墙体材质
	Material wallMaterial;
	// 设置着色和反射的比例
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
	// 设置墙体位置、法向量、材质
	RayTracing::Plane* wall = new RayTracing::Plane(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	wall->setMaterial(wallMaterial);
	// 把墙体加入场景
	scene.addEntity(wall);

	// 设置球体参数

	// 设置球体材质
	Material ballMaterial;
	// 设置着色和反射的比例
	ballMaterial.kShade = 0.8f;
	ballMaterial.kReflect = 0.2f;
	ballMaterial.kRefract = 0.0f;
	ballMaterial.refractiveIndex = 1.5f;

	// 设置球体材质颜色计算函数，所有函数返回一个常量
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
	// 设置球体位置、半径、材质
	auto ball = new RayTracing::Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
	ball->setMaterial(ballMaterial);
	// 把球体加入场景
	scene.addEntity(ball);

	// 设置球体2参数

	// 设置球体材质
	Material ballMaterial2;
	// 设置着色和反射的比例
	ballMaterial2.kShade = 0.2f;
	ballMaterial2.kReflect = 0.2f;
	ballMaterial2.kRefract = 0.6f;
	ballMaterial2.refractiveIndex = 1.5f;

	// 设置球体材质颜色计算函数，所有函数返回一个常量
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
	// 设置球体位置、半径、材质
	auto ball2 = new RayTracing::Sphere(glm::vec3(-2.5f, 1.0f, 0.0f), 1.0f);
	ball2->setMaterial(ballMaterial2);
	// 把球体加入场景
	scene.addEntity(ball2);

	// 设置球体3参数

	// 设置球体材质
	Material ballMaterial3;
	// 设置着色和反射的比例
	ballMaterial3.kShade = 0.99f;
	ballMaterial3.kReflect = 0.01f;
	ballMaterial3.kRefract = 0.0f;
	ballMaterial3.refractiveIndex = 1.5f;

	// 设置球体材质颜色计算函数，所有函数返回一个常量
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
	// 设置球体位置、半径、材质
	auto ball3 = new RayTracing::Sphere(glm::vec3(2.5f, 1.0f, 0.0f), 1.0f);
	ball3->setMaterial(ballMaterial3);
	// 把球体加入场景
	scene.addEntity(ball3);


	while (!glfwWindowShouldClose(window))
	{
		// 处理输入信息并初始化缓冲、变换矩阵
		processInput(window);

		glClearColor(0.3, 0.3, 0.3, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		model = glm::mat4(1.0f);
		view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
		projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

		// 初始化观察右向量
		glm::vec3 viewRight = glm::normalize(glm::cross(viewFront, viewUp));

		// 将点设置为绘制对象
		shader.use();
		glBindVertexArray(VAO);

		// 枚举屏幕上每一个像素
		for (unsigned int i = 0; i < SCR_WIDTH; i++)
		{
			for (unsigned int j = 0; j < SCR_HEIGHT; j++)
			{
				// 将像素坐标分量映射到[-1, 1]
				glm::vec3 pos(float(i) * 2 / SCR_WIDTH - 1.0f, float(j) * 2/ SCR_HEIGHT - 1.0f, 0.0f);
				shader.setVec2("screenPos", pos.x, pos.y);

				// 计算像素在世界坐标中的位置
				glm::vec3 globalPos = viewPos + viewFront + pos.x * viewRight * (float(SCR_WIDTH) / SCR_HEIGHT) + pos.y * viewUp;

				// 计算出光线并进行光线追踪
				RayTracing::Ray ray(viewPos, globalPos);
				glm::vec3 color = scene.traceRay(ray);

				// 绘制该处的像素
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

