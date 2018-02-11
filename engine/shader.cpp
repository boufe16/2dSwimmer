#include "shader.h"


Shader::Shader() {
}
Shader::Shader(std::string vertexShaderName, std::string fragmentShaderName)
{
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//VERTEX SHADER 
	file.open(vertexShaderName);
	if (!file){//check errors 
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}
	//we need to compile the shader from source code (code string) ;
	std::stringstream shaderSrcCode; 
	shaderSrcCode << file.rdbuf(); 
	file.close();
	std::string tmpCode = shaderSrcCode.str(); 
	const char * vertexShaderCode = tmpCode.c_str();

	unsigned int vertexShader; 
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader); 

	//err handling
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "VERTEX Shader Compilation Error" << std::endl
			<< infoLog << std::endl;
	}

	//cleanup
	shaderSrcCode.str(std::string()); //clean stream
	shaderSrcCode.clear();//clear flags 

	//FRAGMENT SHADER
	file.open(fragmentShaderName);
	if (!file) {
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}
	shaderSrcCode << file.rdbuf();
	file.close(); 
	tmpCode = shaderSrcCode.str();
	const char * fragmentShaderCode = tmpCode.c_str();

	unsigned int fragmentShader; 
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "FRAGMENT SHADER compilation Error \n" 
			<< infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog); 
		std::cout << "Shader Program Link Error \n" 
			<<infoLog<<std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setInt(std::string name, int index) {
	use();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), index);
}
void Shader::setFloat(std::string name, float fl) {
	use();
	glUniform1f(glGetUniformLocation(ID, name.c_str()), fl);
}
void Shader::setVec3f(std::string name, glm::vec3 &vec) {
	use();
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
}
void Shader::setMat4(std::string name, glm::mat4 &mat) {
	use();
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

Shader::~Shader()
{
}
