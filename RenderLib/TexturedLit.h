#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Transforms.h"
#include "PointLight.h"
#include "Texture.h"
#include "GeometryHelper.h"
#include "Drawable.h"

class TexturedLit : public Drawable
{
	SOF::ShaderProgram *shader;
	SOF::Geometry *geom;
	SOF::Texture* texture;
	SOF::Texture* normalMap;

	void CreateGeometry( const GeometryHelper &geometry );
public:
	TexturedLit( const GeometryHelper &geometry, const std::string& textureName="default", const std::string& normalMapName="default" ) 
	{ 
		CreateGeometry(geometry); 
		// load the textures
		if ( textureName == "default")
			texture = new SOF::Texture("textures/default_diffuse.png");
		else
			texture = new SOF::Texture(textureName);

		if ( normalMapName == "default")
			normalMap = new SOF::Texture("textures/default_normal.png");
		else
			normalMap = new SOF::Texture(normalMapName);

	}
	virtual void Draw( const Transforms &renderTransform, const PointLight& light ) override;
};