#pragma once
#include "ShaderProgram.h"
#include "Transforms.h"
#include "Drawable.h"
#include "CubeGeom.h"
#include "CubeMap.h"

class Skybox : public Drawable
{
    SOF::ShaderProgram* shader;
    SOF::Cubemap* cubeMapTexture;
    CubeGeometry* geom;
    SOF::Geometry* geometry;
public:
    Skybox(SOF::Cubemap* cubeMapTexture) : cubeMapTexture(cubeMapTexture)
    {
        geom = new CubeGeometry();
        geometry = new SOF::Geometry(geom->GetNumVertices());
        geometry->AddAttribute(geom->GetPositionVector(), "vertexPos");

        shader = new SOF::ShaderProgram("shaders/vs_skybox.glsl", "shaders/fs_skybox.glsl");
        geometry->Finalize(shader);
    }

    virtual void Draw(const Transforms& renderTransform, const PointLight& light) override
    {
        shader->Use();
        // figure out the camera position - need to set this as a uniform
        glm::vec3 camPos = -glm::vec3(renderTransform.GetView()[3]) * glm::mat3(renderTransform.GetView());
        // view/projection matrix
        glm::mat4 viewProj = renderTransform.GetProj() * renderTransform.GetView();
        shader->SetUniformVec3("cameraPos", camPos);
        shader->SetUniformMat4("viewProj", viewProj);
        shader->AssignCubemapSampler("cubeMap", cubeMapTexture);
        // switch off z buffer writes
        glDepthMask(GL_FALSE);
        // kick the primitives
        geometry->DrawPrimitives();
        // turn the z buffer writes back on 
        glDepthMask(GL_TRUE);
    }
};