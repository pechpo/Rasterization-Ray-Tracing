#pragma once
#include <glm/glm.hpp>
#include <functional>

// 着色部分，定义材质、光线
// 材质结构

struct Material
{
	// 所有颜色使用函数根据该点在材质上的位置进行计算

	std::function<glm::vec3(const glm::vec3& pos)> ambient; // 环境色
	std::function<glm::vec3(const glm::vec3& pos)> diffuse; // 漫反射色
	std::function<glm::vec3(const glm::vec3& pos)> specular;// 镜面反射色
	std::function<float(const glm::vec3& pos)> shininess;	// 亮度

	float kShade;			// 局部着色比例
	float kReflect;			// 反射比例
	float kRefract;			// 折射比例
	float refractiveIndex;	// 折射率
};