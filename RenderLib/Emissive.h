#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"
#include "Geometry.h"
#include "SphereGeom.h"
#include "Transforms.h"
#include "Drawable.h"
#include "PointLight.h"

class Emissive : public Drawable
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;
	glm::vec4 color;
	float alphaValue;

	void CreateGeometry( const GeometryHelper &geometry)
	{
		shader = new SOF::ShaderProgram("shaders/v_emissive.glsl", "shaders/f_emissive.glsl");
		geom = new SOF::Geometry(geometry.GetNumVertices());
		geom->AddAttribute(geometry.GetPositionVector(), "vertexPos");
		geom->Finalize(shader);
	}
public:
	Emissive(const GeometryHelper &geometry, const glm::vec4& color ) : color(color) {CreateGeometry(geometry);}
	virtual void Draw(const Transforms &trans, const PointLight& light) override
	{
		glm::mat4 mvp;
		mvp = trans.GetProj() * trans.GetView() * trans.GetModel();
		shader->Use();
		shader->SetUniformMat4("mvp", mvp);
		shader->SetUniformVec4("color", color);
		shader->SetUniformFloat("alpha", alphaValue);
		geom->DrawPrimitives();
	}

	virtual void ChangeOpacity(float alpha) {
		alphaValue = alpha;
	}
};