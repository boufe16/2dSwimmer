#pragma once
#include <string> 
#include <iostream> 
#include <sstream> 
#include <fstream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
class Shader
{
public:
	Shader();
	Shader(std::string vertexShaderName, std::string fragmentShaderName);
	void use(); 
	unsigned int ID;
	~Shader();
	void setFloat(std::string name, float fl);
	void setInt(std::string name, int index); 
	void setMat4(std::string name, glm::mat4 &mat);
	void setVec3f(std::string name, glm::vec3 &vec);
};

