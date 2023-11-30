#pragma once
#include <glm/glm.hpp>
#include "Material.hpp"

// 光照抽象类

class Light
{
public:
	virtual glm::vec3 calLight(
		const Material& material,  //光线照射到的材质
		const glm::vec3& fragPos,
		const glm::vec3& norm,
		const glm::vec3& viewDir) const = 0;
private:
};