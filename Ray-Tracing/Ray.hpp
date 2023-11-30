#include <glm/glm.hpp>

// ����׷�������
// ������

namespace RayTracing
{
	static const float FLOAT_INF = 100000000.0f;
	static const float FLOAT_EPS = 1e-5;			// ���ڸ������Ƚ��Ժ��Ը������
	static const glm::vec3 NULL_POINT(FLOAT_INF, FLOAT_INF, FLOAT_INF);
	class Ray
	{
	public:
		Ray(glm::vec3 src, glm::vec3 dest);			// �������յ㹹��
		glm::vec3 pointAtT(float t) const;			// ������t����������������
		glm::vec3 getVertex() const { return _vertex; }
		glm::vec3 getDirection() const { return _direction; }
	private:
		glm::vec3 _vertex;
		glm::vec3 _direction;
	};
}