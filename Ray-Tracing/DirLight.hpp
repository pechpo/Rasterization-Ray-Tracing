#include <glm/glm.hpp>
#include "Light.hpp"
#include "Material.hpp"

// 方向光类，用Phong模型局部着色

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
	glm::vec3 _ambient;		// 环境光
	glm::vec3 _diffuse;		// 漫反射光
	glm::vec3 _specular;	// 镜面反射光
	glm::vec3 _direction;	// 光照方向
};