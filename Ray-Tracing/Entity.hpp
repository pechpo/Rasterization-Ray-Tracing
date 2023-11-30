#include <glm/glm.hpp>
#include "Ray.hpp"
#include "Material.hpp"

// 实体部分
// 定义了抽象实体以及平面、三角形、球体三种实体

namespace RayTracing
{
	// 抽象实体类
	class Entity
	{
	public:
		virtual float rayCollision(const Ray& ray) const = 0;		// 接收射线，返回射线与该实体的交的参数方程解t，无解返回-1
		virtual glm::vec3 calNormal(const glm::vec3& p) const = 0;	// 根据实体上一点计算法向量
		virtual bool rayInEntity(const Ray& ray) const = 0;			// 判断该射线是否从实体内部射出
		void setMaterial(const Material& m) { _material = m; }
		const Material& getMaterial() const { return _material; }
	protected:
		Material _material;	// 实体的材质
	};

	// 平面类
	class Plane : public Entity
	{
	public:
		Plane(const glm::vec3& aPoint, const glm::vec3& normal);
		bool onPlane(const glm::vec3& p) const;
		glm::vec3 getNormal() const { return _normal; }
		glm::vec3 getAPoint() const { return _aPoint; }

		float rayCollision(const Ray& ray) const;
		glm::vec3 calNormal(const glm::vec3& p) const;
		bool rayInEntity(const Ray& ray) const { return false; }
	private:
		glm::vec3 _normal;
		glm::vec3 _aPoint;
	};

	// 三角形类
	class Triangle : public Entity
	{
	public:
		Triangle(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
		bool inTriangle(const glm::vec3& p) const;
		Plane getPlane() const;
		glm::vec3 getNormal() const;
		void getVertice(glm::vec3& A, glm::vec3& B, glm::vec3 C) const;

		float rayCollision(const Ray& ray) const;
		glm::vec3 calNormal(const glm::vec3& p) const;
		bool rayInEntity(const Ray& ray) const { return false; }
	private:
		glm::vec3 _vertice[3];
	};

	// 球体类
	class Sphere : public Entity
	{
	public:
		Sphere(const glm::vec3& center, float radius);
		bool inSphere(const glm::vec3& p) const;
		glm::vec3 getCenter() const { return _center; }
		float getRadius() const { return _radius; }

		float rayCollision(const Ray& ray) const;
		glm::vec3 calNormal(const glm::vec3& p) const;
		bool rayInEntity(const Ray& ray) const;
	private:
		glm::vec3 _center;
		float _radius;
	};
}