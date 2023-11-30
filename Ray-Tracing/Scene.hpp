#include <glm/glm.hpp>
#include "Entity.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include <vector>

// ����׷�ٲ���
// ���峡���࣬��������Խ��й���׷��

namespace RayTracing
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		void addEntity(Entity* entity);		// �򳡾��м���ʵ��
		void addLight(Light* light);		// �򳡾��м������
		glm::vec3 traceRay(const Ray& ray, unsigned int recursionTime = 0);			// ����׷��������
		std::pair<const glm::vec3&, const Entity*> getIntersection(const Ray& ray);	// ��ȡ�����е�һ���������ཻ��ʵ��
		glm::vec3 shade(const Entity& entity, glm::vec3 fragPos, const Ray& ray);	// �����й�ʹ�þֲ�ģ����ɫ

		static const unsigned int MAX_RECURSION_TIME; // ����׷�ٺ������ݹ����
	private:
		std::vector<Entity*> _entitys;
		std::vector<Light*> _lights;
	};
}
