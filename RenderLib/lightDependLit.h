#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Transforms.h"
#include "PointLight.h"
#include "Texture.h"
#include "GeometryHelper.h"
#include "Drawable.h"

class lightDependLit : public Drawable
{
	SOF::ShaderProgram *shader;
	SOF::Geometry *geom;
	SOF::Texture* texture1;
	SOF::Texture* texture2;
	SOF::Texture* normalMap;

	void CreateGeometry( const GeometryHelper &geometry );
public:
	lightDependLit( const GeometryHelper &geometry, const std::string& textureName1="default", const std::string& textureName2 = "default", const std::string& normalMapName="default" )
	{ 
		CreateGeometry(geometry); 
		// load the textures
		if ( textureName1 == "default")
			texture1 = new SOF::Texture("textures/default_diffuse.png");
		else
			texture1 = new SOF::Texture(textureName1);

		if (textureName2 == "default")
			texture2 = new SOF::Texture("textures/default_diffuse.png");
		else
			texture2 = new SOF::Texture(textureName2);

		if ( normalMapName == "default")
			normalMap = new SOF::Texture("textures/default_normal.png");
		else
			normalMap = new SOF::Texture(normalMapName);

	}
	virtual void Draw( const Transforms &renderTransform, const PointLight& light ) override;
};