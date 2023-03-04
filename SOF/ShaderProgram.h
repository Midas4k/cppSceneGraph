#pragma once
#include "stdafx.h"
#include "texture.h"
#include "cubemap.h"

namespace SOF
{
	class ShaderProgram
	{
		// Encapsulates a shader program (vertex + fragment shader).
		// Shaders loaded from file and compiled at runtime.
		// provides an API for setting uniforms and attributes.
		GLuint LoadShaderFromFile(const char *fileName, GLenum shaderType ); // helper function - loads and compiles shader from file
		GLuint LoadShaderFromSource(const std::string &source, GLenum shaderType); // helper function - loads and compiles shader from source code
		GLuint vsObject; // vertex shader object
		GLuint fsObject; // fragment shader object
		GLuint programObject; // shader program object
		int nextTextureStage;
		std::map<std::string, int> textureStages;
		void BuildProgram();
		void AssignSampler(const std::string &uniform, GLint sampler, GLint object);
	public:
		ShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource); // load from source
		ShaderProgram(const char *vertexShaderFile, const char *fragmentShaderFile); // load from files
		void SetUniformMat4(const char *name, const glm::mat4 &value) const;
		void SetUniformFloat(const char *name, float value) const;
		void SetUniformInt(const char *name, int value) const;
		void SetUniformVec2(const char *name, const glm::vec2 &value) const;
		void SetUniformVec3(const char *name, const glm::vec3 &value) const;
		void SetUniformVec4(const char *name, const glm::vec4 &value) const;
		void SetStaticVertexAttribVec4(const char *name, const glm::vec4 &value) const;
		void SetStaticVertexAttribVec3(const char *name, const glm::vec3 &value) const;
		void SetStaticVertexAttribVec2(const char *name, const glm::vec2 &value) const;
		GLint GetAttribute(const std::string& name) const;
		GLint GetUniform(const std::string& name) const;
		void Use() const; // set this shader program as the current program
		void AssignTextureSampler(const std::string &uniform, const Texture *t);
		void AssignTextureSampler(const std::string &uniform, const Texture *t, int num );
		void AssignCubemapSampler(const std::string &uniform, const Cubemap* t);
	};
}