#include <glm/glm.hpp>
#include "Entity.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include <vector>

// 光线追踪部分
// 定义场景类，场景类可以进行光线追踪

namespace RayTracing
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		void addEntity(Entity* entity);		// 向场景中加入实体
		void addLight(Light* light);		// 向场景中加入光线
		glm::vec3 traceRay(const Ray& ray, unsigned int recursionTime = 0);			// 光线追踪主函数
		std::pair<const glm::vec3&, const Entity*> getIntersection(const Ray& ray);	// 获取场景中第一个与射线相交的实体
		glm::vec3 shade(const Entity& entity, glm::vec3 fragPos, const Ray& ray);	// 对所有光使用局部模型着色

		static const unsigned int MAX_RECURSION_TIME; // 光线追踪函数最大递归次数
	private:
		std::vector<Entity*> _entitys;
		std::vector<Light*> _lights;
	};
}
