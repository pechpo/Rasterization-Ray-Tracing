#pragma once
#include <glm/glm.hpp>
#include <functional>

// ��ɫ���֣�������ʡ�����
// ���ʽṹ

struct Material
{
	// ������ɫʹ�ú������ݸõ��ڲ����ϵ�λ�ý��м���

	std::function<glm::vec3(const glm::vec3& pos)> ambient; // ����ɫ
	std::function<glm::vec3(const glm::vec3& pos)> diffuse; // ������ɫ
	std::function<glm::vec3(const glm::vec3& pos)> specular;// ���淴��ɫ
	std::function<float(const glm::vec3& pos)> shininess;	// ����

	float kShade;			// �ֲ���ɫ����
	float kReflect;			// �������
	float kRefract;			// �������
	float refractiveIndex;	// ������
};