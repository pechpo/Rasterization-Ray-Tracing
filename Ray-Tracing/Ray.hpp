#include <glm/glm.hpp>

// 光线追踪相关类
// 射线类

namespace RayTracing
{
	static const float FLOAT_INF = 100000000.0f;
	static const float FLOAT_EPS = 1e-5;			// 用于浮点数比较以忽略浮点误差
	static const glm::vec3 NULL_POINT(FLOAT_INF, FLOAT_INF, FLOAT_INF);
	class Ray
	{
	public:
		Ray(glm::vec3 src, glm::vec3 dest);			// 用起点和终点构造
		glm::vec3 pointAtT(float t) const;			// 起点加上t倍方向向量的坐标
		glm::vec3 getVertex() const { return _vertex; }
		glm::vec3 getDirection() const { return _direction; }
	private:
		glm::vec3 _vertex;
		glm::vec3 _direction;
	};
}