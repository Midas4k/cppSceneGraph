#include "stdafx.h"
#include "texturedlit.h"
#include "spheregeom.h"
#include <cmath>

void TexturedLit::CreateGeometry( const GeometryHelper &geometry )
{
	// load the shader program
	shader = new SOF::ShaderProgram("shaders/v_texturedlit.glsl", "shaders/f_texturedlit.glsl");
	// make the SOF::Geometry object
	geom = new SOF::Geometry(geometry.GetNumVertices());
	// add the position attribute
	geom->AddAttribute(geometry.GetPositionVector(), "vertexPos");
	// add the normal attribute
	geom->AddAttribute(geometry.GetNormalVector(), "vertexNormal");
	// add the UV coordinate attribute
	geom->AddAttribute(geometry.GetUVVector(), "vertexUV");
	// add the tangent attribute
	geom->AddAttribute(geometry.GetTangentVector(), "vertexTangent");
	// finish up
	geom->Finalize(shader);
}

void TexturedLit::Draw(const Transforms &trans, const PointLight& light )
{
	// calculate the mvp matrix;
	glm::mat4 vp = trans.GetProj() * trans.GetView();
	// enable the shader, set uniforms
	shader->Use();
	shader->SetUniformMat4("viewProj", vp);
	shader->SetUniformMat4("model", trans.GetModel());
	shader->SetUniformVec3("lightK", glm::vec3(light.GetAmbientLevel(), light.GetDiffuseLevel(), light.GetSpecularLevel()));
	shader->SetUniformFloat("alpha", light.GetSpecularPower());
	shader->SetUniformMat4("lightPos", light.GetPosition());
	shader->SetUniformMat4("ambientColor", light.GetColor());
	shader->SetUniformMat4("lightColor", light.GetColor());
	glm::vec4 attenuation = glm::vec4(1,1,1,1) / (light.GetAttenuationRadius() * light.GetAttenuationRadius());
	shader->SetUniformVec4("attenuation", attenuation);

	glm::vec3 camPos = -glm::vec3(trans.GetView()[3]) * glm::mat3(trans.GetView());
	shader->SetUniformVec3("cameraPos", camPos);
	// hook up the textures 
	shader->AssignTextureSampler("textureSampler", texture);
	shader->AssignTextureSampler("normalSampler", normalMap);
	// draw the triangles
	geom->DrawPrimitives();
}
