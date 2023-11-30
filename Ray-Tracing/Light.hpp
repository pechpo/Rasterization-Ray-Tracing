#pragma once
#include <glm/glm.hpp>
#include "Material.hpp"

// ���ճ�����

class Light
{
public:
	virtual glm::vec3 calLight(
		const Material& material,  //�������䵽�Ĳ���
		const glm::vec3& fragPos,
		const glm::vec3& norm,
		const glm::vec3& viewDir) const = 0;
private:
};