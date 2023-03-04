#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"

namespace SOF
{
	class Geometry
	{
		typedef struct
		{
			int floatsPerVertex;
			int offset;
			std::string name;
			float *data;
		} Attribute;

		std::vector<Attribute> attributes;
		int vertexStride;
		int nVert;

		GLuint vao; // vertex array object
	public:
		Geometry( int nVert ) : nVert(nVert) {}
		template<class T> void AddAttribute(const std::vector<T>& data, const std::string &name)
		{
			// get the size of an element from the first element (all glm types should implement 'length')
			int floatsPerVertex = data[0].length();

			if (data.size() != nVert)
			{
				std::string msg = "Geometry::AddAttribute " + name + " has wrong number of elements";
				std::cerr << msg << std::endl;
			}

			Attribute thisAttribute;
			thisAttribute.floatsPerVertex = floatsPerVertex;
			if (attributes.empty())
				thisAttribute.offset = 0;
			else
			{
				Attribute lastElement = attributes[attributes.size() - 1];
				thisAttribute.offset = lastElement.offset + lastElement.floatsPerVertex;
			}
			thisAttribute.name = name;
			// create a temporary buffer and copy the data
			thisAttribute.data = new float[nVert * floatsPerVertex];
			for (int i = 0; i < nVert; i++)
			{
				std::memcpy(thisAttribute.data + i * floatsPerVertex, glm::value_ptr(data[i]), floatsPerVertex * sizeof(float));
			}
			attributes.push_back(thisAttribute);
		}

		void Finalize( const ShaderProgram *shader )
		{
			// calculate how much memory is needed for the buffer
			int numFloatsTotal = 0;
			for (Attribute a : attributes)
			{
				numFloatsTotal += a.floatsPerVertex;
			}
			// allocate buffer
			float *buffer = new float[numFloatsTotal * nVert];
			// run through the attributes copying data into the buffer
			float *curPtr = buffer;
			for (int i = 0; i < nVert; i++)
			{
				for (Attribute a : attributes)
				{
					for (int j = 0; j < a.floatsPerVertex; j++)
					{
						*curPtr++ = a.data[i*a.floatsPerVertex + j];
					}
				}
			}
			vertexStride = numFloatsTotal;
			// set up the vertex buffer object, vertex array object
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glGenVertexArrays(1, &vao);
			// bind the VAO
			glBindVertexArray(vao);
			// bind the VBO
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numFloatsTotal*nVert, buffer, GL_STATIC_DRAW);
			// hook up the attributes to the shader program
			for (Attribute a : attributes)
			{
				glEnableVertexAttribArray(shader->GetAttribute(a.name));
				glVertexAttribPointer(shader->GetAttribute(a.name), a.floatsPerVertex, GL_FLOAT, GL_FALSE, vertexStride * sizeof(GLfloat), (const GLvoid*)(a.offset * sizeof(GLfloat)));
			}
			// unbind the VBO
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// unbind the VAO
			glBindVertexArray(0);
			// free temporary memory
			for (Attribute a : attributes)
				delete[] a.data;
			delete[] buffer;
		}
		void DrawPrimitives() const
		{
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, nVert);
			glBindVertexArray(0);
		}	
		void DrawLines() const
		{
			glBindVertexArray(vao);
			glDrawArrays(GL_LINE_LOOP, 0, nVert);
			glBindVertexArray(0);
		}
		void DrawPoints(GLfloat size) const
		{
			glPointSize(size);
			DrawPoints();
			glPointSize(1);
		}

		void DrawPoints() const
		{
			glBindVertexArray(vao);
			glDrawArrays(GL_POINTS, 0, nVert);
			glBindVertexArray(0);
		}
		
	};
}