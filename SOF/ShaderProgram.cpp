#include "stdafx.h"
#include "ShaderProgram.h"

GLuint SOF::ShaderProgram::LoadShaderFromSource(const std::string &source, GLenum shaderType)
{
	// create shader
	GLuint object = glCreateShader(shaderType);

	const char* code = source.c_str();
	glShaderSource(object, 1, (const GLchar**)&code, NULL);

	//compile
	glCompileShader(object);

	//throw exception if compile error occurred
	GLint status;
	glGetShaderiv(object, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(object, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(object);
		std::cerr << msg << std::endl;
	}
	return object;
}
GLuint SOF::ShaderProgram::LoadShaderFromFile(const char *fileName, GLenum shaderType )
{
	// get the source code from the file, if it exists and is openable
	std::ifstream inFile;
	std::string shaderSource;
	try
	{
		inFile.open(fileName);
		std::stringstream buffer;
		buffer << inFile.rdbuf();
		shaderSource = buffer.str();
		inFile.close();
	}
	catch (...)
	{
		std::cerr << "failed to open shader file" << fileName << std::endl;
	}

	return LoadShaderFromSource(shaderSource, shaderType);

}

void SOF::ShaderProgram::BuildProgram()
{
	// create a new shader program object, attach the two shaders and link the program
	//create the program object
	programObject = glCreateProgram();
	if (programObject == 0)
		throw std::runtime_error("glCreateProgram failed");

	glAttachShader(programObject, vsObject);
	glAttachShader(programObject, fsObject);
	glLinkProgram(programObject);
	glDetachShader(programObject, fsObject);
	glDetachShader(programObject, vsObject);

	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		std::string msg("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(programObject, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteProgram(programObject); programObject = 0;
		std::cerr << msg << std::endl;
	}
}

// constructor - load from strings
SOF::ShaderProgram::ShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
	nextTextureStage = 0;
	// load and compile the vertex and fragment shaders
	vsObject = LoadShaderFromSource(vertexShaderSource, GL_VERTEX_SHADER);
	fsObject = LoadShaderFromSource(fragmentShaderSource, GL_FRAGMENT_SHADER);
	BuildProgram();
}
// constructor - load from files
SOF::ShaderProgram::ShaderProgram(const char *vertexShaderFile, const char *fragmentShaderFile)
{
	nextTextureStage = 0;
	// load and compile the vertex and fragment shaders
	vsObject = LoadShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER);
	fsObject = LoadShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER);
	BuildProgram();

}

void SOF::ShaderProgram::SetUniformMat4(const char *name, const glm::mat4 &value) const
{
	glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, glm::value_ptr(value) );
}

void SOF::ShaderProgram::SetUniformFloat(const char *name, float value) const
{
	glUniform1f(GetUniform(name), value);
}

void SOF::ShaderProgram::SetUniformInt(const char *name, int value) const
{
	glUniform1i(GetUniform(name), value);
}

void SOF::ShaderProgram::SetUniformVec2(const char *name, const glm::vec2 &value) const
{
	glUniform2fv(GetUniform(name), 1, glm::value_ptr(value));
}

void SOF::ShaderProgram::SetUniformVec3(const char *name, const glm::vec3 &value) const
{
	glUniform3fv(GetUniform(name), 1, glm::value_ptr(value));
}

void SOF::ShaderProgram::SetUniformVec4(const char *name, const glm::vec4 &value) const
{
	glUniform4fv(GetUniform(name), 1, glm::value_ptr(value));
}

void SOF::ShaderProgram::SetStaticVertexAttribVec4(const char *name, const glm::vec4 &value) const
{
	glVertexAttrib4fv(GetAttribute(name),glm::value_ptr(value) );
}

void SOF::ShaderProgram::SetStaticVertexAttribVec3(const char *name, const glm::vec3 &value) const
{
	glVertexAttrib3fv(GetAttribute(name), glm::value_ptr(value));
}

void SOF::ShaderProgram::SetStaticVertexAttribVec2(const char *name, const glm::vec2 &value) const
{
	glVertexAttrib2fv(GetAttribute(name), glm::value_ptr(value));
}

void SOF::ShaderProgram::Use() const
{
	// set as current program
	glUseProgram(programObject);
}

GLint SOF::ShaderProgram::GetAttribute(const std::string &name) const
{
	return glGetAttribLocation(programObject, name.c_str());
}

GLint SOF::ShaderProgram::GetUniform(const std::string &name) const
{
	return glGetUniformLocation(programObject, name.c_str());
}


void SOF::ShaderProgram::AssignSampler(const std::string &uniform, GLint sampler, GLint object)
{
	// we deal with this in the shader program for convenience. This class assigns
	// a texture stage to each texture sampler on first use, and then keeps track of it.
	if (textureStages.find(uniform) == textureStages.end())
	{
		// this is the first time we have seen this uniform, assign a texture stage
		textureStages[uniform] = nextTextureStage;
		++nextTextureStage;
	}
	glUniform1i(GetUniform(uniform), textureStages[uniform]);
	glActiveTexture(GL_TEXTURE0 + textureStages[uniform]);
	glBindTexture(sampler, object);

}
void SOF::ShaderProgram::AssignTextureSampler(const std::string &uniform, const SOF::Texture *t)
{
	AssignTextureSampler(uniform, t, 0);
}

void SOF::ShaderProgram::AssignCubemapSampler(const std::string &uniform, const SOF::Cubemap* t)
{
	AssignSampler(uniform, GL_TEXTURE_CUBE_MAP, t->GetObject());
}

void SOF::ShaderProgram::AssignTextureSampler(const std::string &uniform, const SOF::Texture *t, int num )
{
	AssignSampler(uniform, GL_TEXTURE_2D, t->GetObject(num));
}