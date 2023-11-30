#include <glm/glm.hpp>
#include "Light.hpp"
#include "Material.hpp"

// ������࣬��Phongģ�;ֲ���ɫ

class DirLight : public Light
{
public:
	DirLight(
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		glm::vec3 direction);
	glm::vec3 calLight(
		const Material& material,
		const glm::vec3& fragPos,
		const glm::vec3& norm,
		const glm::vec3& viewDir) const;
private:
	glm::vec3 _ambient;		// ������
	glm::vec3 _diffuse;		// �������
	glm::vec3 _specular;	// ���淴���
	glm::vec3 _direction;	// ���շ���
};