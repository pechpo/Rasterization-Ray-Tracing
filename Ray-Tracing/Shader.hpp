#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// 着色器部分
// 用于读写着色器的类

class Shader
{
public:
	unsigned int getID() { return _ID; }
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);  // 编译并连接顶点着色器和片元着色器
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMatrix4(const std::string& name, const glm::mat4& value) const;
private:
	unsigned int _ID;
};