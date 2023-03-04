#include "stdafx.h"
#include "RenderLib.h"
#include "spline.h"
#include "Physics.h"
#include <random>

std::vector<glm::vec3> splinePoints;

// callback for animating on a spline
class SplineAnimationCallback : public UpdateCallback
{
	Spline* spline;
	TransformNode* trans;
	float rate;
	float t = 0;
public:
	SplineAnimationCallback(TransformNode* trans, Spline* spline, float rate) : trans(trans), spline(spline), rate(rate), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		if (t > spline->GetNumPoints())
			t -= spline->GetNumPoints();
		// work out the new transform here
		glm::vec3 position = spline->GetPoint(t);
		glm::vec3 tangent = spline->GetTangent(t);
		glm::mat4 lookAt = glm::lookAt(position, position + tangent, glm::vec3(0, 1, 0));
		trans->SetTransform(glm::inverse(lookAt));
	}
};

class MoveLightCallback : public UpdateCallback
{
	LightNode* node;
	float t;
public:
	MoveLightCallback(LightNode* node) : node(node), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		float x, y, z;
		x = 50.0f * cos(t);
		y = 20.0f;
		z = 50.0f * sin(t);
		node->light.position = glm::vec3(x, y, z);
	}
};


//callback for animating a rotation about the y axis - use this for the solar system exercise
class YRotatorCallback : public UpdateCallback
{
	TransformNode* trans;
	float rate;
	float t;
public:
	YRotatorCallback( TransformNode *trans, float rate) : trans(trans), rate(rate), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		trans->SetTransform(glm::rotate(glm::mat4(), t * rate, glm::vec3(0, 1, 0)));
	}

};

class TransparentCallback : public UpdateCallback {
	Emissive* geom;
	float alpha;
	bool flip;
public:
	TransparentCallback(Emissive *geom, float rate) : geom(geom), alpha(1.0f), flip(false){}
	virtual void operator()(float dt) {
		if (!flip) {
			alpha -= dt;
			if (alpha <= 0.0f)flip = true;
		}
		else {
			alpha += dt;
			if (alpha >= 1.0f)flip = false;
		}
		geom->ChangeOpacity(alpha);
	}
};

Node* SolarSystem(Physics* phys)
{
	//The below MeshObject is an example of importing a model into the scene
	MeshObject* mesh = new MeshObject("meshes/sheep/Statuette.fbx"); //As seen the object is a ".fbx" file which can be found by following the directory
	TransformNode* meshScaler = new TransformNode(glm::scale(glm::mat4(), glm::vec3(.5, .5, -.5)));
	GeometryNode* meshNode = new GeometryNode(mesh);

	//this creates the Quad that will later be used for the wall
	QuadGeometry quad(10);
	TexturedLit* wall = new TexturedLit(quad, "textures/grid.jpg");
	GeometryNode* geomNode = new GeometryNode(wall);


	//This is a custom object that has been created to look like saturns ring
	satRing ring(.1f, .3f, 16);
	TexturedLit* ringTL = new TexturedLit(ring, "textures/2k_saturn_ring_alpha.png");
	GeometryNode* ringGN = new GeometryNode(ringTL);

	// create a cubemap
	std::vector<std::string> cubeMapFiles;
	cubeMapFiles.push_back("textures/cubemaps/bkg1_top.png");//top
	cubeMapFiles.push_back("textures/cubemaps/bkg1_bot.png");//bot
	cubeMapFiles.push_back("textures/cubemaps/bkg1_right.png");//right
	cubeMapFiles.push_back("textures/cubemaps/bkg1_left.png");//left
	cubeMapFiles.push_back("textures/cubemaps/bkg1_front.png");//front
	cubeMapFiles.push_back("textures/cubemaps/bkg1_back.png");//back
	SOF::Cubemap* cubeMap = new SOF::Cubemap(cubeMapFiles);
	Skybox* sb = new Skybox(cubeMap);
	GeometryNode* sbNode = new GeometryNode(sb);

	// geometry and drawables
	SphereGeometry sphere(20);

	//In the below section the creation of the TexturedLits for the planets are created
	TexturedLit* mercury = new TexturedLit(sphere, "textures/2k_mercury.jpg");
	TexturedLit* venus = new TexturedLit(sphere, "textures/2k_venus_surface.jpg");
	//However the Earth object will be using a custom TexturedLit file that will allow for a custom shader to use the day/night and normal map for the planet
	lightDependLit* earth = new lightDependLit(sphere, "textures/2k_Earth_daymap.jpg", "textures/nightEarth.jpg", "textures/2k_earth_normal_map.png");
	TexturedLit* mars = new TexturedLit(sphere, "textures/2k_mars.jpg");
	TexturedLit* jupiter = new TexturedLit(sphere, "textures/2k_jupiter.jpg");
	TexturedLit* redNode = new TexturedLit(sphere, "textures/default_normal.png");
	TexturedLit* moon = new TexturedLit(sphere, "textures/2k_moon.jpg");
	TexturedLit* saturn = new TexturedLit(sphere, "textures/2k_saturn.jpg");
	Emissive* sun = new Emissive(sphere, glm::vec4(1.0f, 1.0f, 0.5f, 1.0f));
	
	//solar system spin rotators
	TransformNode* MercurySpinRotator = new TransformNode(glm::mat4());
	TransformNode* VenusSpinRotator = new TransformNode(glm::mat4());
	TransformNode* EarthSpinRotator = new TransformNode(glm::mat4());
	TransformNode* MarsSpinRotator = new TransformNode(glm::mat4());
	TransformNode* JupiterSpinRotator = new TransformNode(glm::mat4());
	TransformNode* SaturnSpinRotator = new TransformNode(glm::mat4());
	
	//solar system orbit rotators
	TransformNode* MercuryOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* VenusOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* EarthOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* MoonOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* MarsOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* JupiterOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* SaturnOrbitRotator = new TransformNode(glm::mat4());

	//this will be used to display the spline on the scene view
	GeometryNode* redSpline = new GeometryNode(redNode);

	// light node - use this as the root node
	LightNode* light = new LightNode(glm::vec3(0, 0, 0), glm::vec4(1, 1, 1, 1), .1, 1.0, 0.2, 10, 400);


	
	//In the below section I am creating the "YRotatorCallback" update callbacks for all of the planets
	MercuryOrbitRotator->SetUpdateCallback(new YRotatorCallback(MercuryOrbitRotator, 4.14f)); // speed of rotation around sun
	MercurySpinRotator->SetUpdateCallback(new YRotatorCallback(MercurySpinRotator, 0.0414f)); // speed of planet spin

	VenusOrbitRotator->SetUpdateCallback(new YRotatorCallback(VenusOrbitRotator, 1.6f));
	VenusSpinRotator->SetUpdateCallback(new YRotatorCallback(VenusSpinRotator, 0.084f));

	EarthOrbitRotator->SetUpdateCallback(new YRotatorCallback(EarthOrbitRotator,.01f));
	EarthSpinRotator->SetUpdateCallback(new YRotatorCallback(EarthSpinRotator, 1.0f));
	MoonOrbitRotator->SetUpdateCallback(new YRotatorCallback(MoonOrbitRotator, 3.0f));

	MarsOrbitRotator->SetUpdateCallback(new YRotatorCallback(MarsOrbitRotator, 0.5f));
	MarsSpinRotator->SetUpdateCallback(new YRotatorCallback(MarsSpinRotator, 0.48f));

	JupiterOrbitRotator->SetUpdateCallback(new YRotatorCallback(JupiterOrbitRotator, 0.08f));
	JupiterSpinRotator->SetUpdateCallback(new YRotatorCallback(JupiterSpinRotator, 0.192f));

	SaturnOrbitRotator->SetUpdateCallback(new YRotatorCallback(SaturnOrbitRotator, 0.02f));
	SaturnSpinRotator->SetUpdateCallback(new YRotatorCallback(SaturnSpinRotator, 0.2f));

	light->AddChild(sbNode); // this adds the previously created Skybox into the graph

	//this is a wall that will be used to demonstrate the physics collision within the scene
	glm::mat4 wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(0.0, 30.0, 0.0));
	wtrans = glm::rotate(wtrans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
	wtrans = glm::rotate(wtrans, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	wtrans = glm::scale(wtrans, glm::vec3(10.0f, 10.0f, 10.0f));
	TransformNode* wall0 = new TransformNode(wtrans);

	
	light->AddChild(wall0);
	wall0->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans)); //Here the QuadCollision is being added to the wall allowing for the balls to collide with it
	//Adding Sun into Scene Graph
	//Scale Node
	TransformNode *sunScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)));
	light->AddChild(sunScale);
	sunScale->SetUpdateCallback(new TransparentCallback(sun, 0.1f));
	//Adding Sun geometry into the scene graph
	light->GetChild(2)->AddChild(new GeometryNode(sun));

	
	//Implementing all the planet orbit rotators as children of light,
	//this allows for them to be called later to create the graph
	light->AddChild(MercuryOrbitRotator);
	light->AddChild(VenusOrbitRotator);
	light->AddChild(EarthOrbitRotator);
	light->AddChild(MarsOrbitRotator);
	light->AddChild(JupiterOrbitRotator);
	light->AddChild(SaturnOrbitRotator);
	

	//Below is the creation of the planets within the scene graph
	//Mercury
	MercuryOrbitRotator->AddChild(new TransformNode(glm::translate(glm::mat4(), glm::vec3(10, 0, 0))));// this moves Mercury away from the sun object
	MercuryOrbitRotator->GetChild(0)->AddChild(MercurySpinRotator);//The spin rotator is then added allowing for the planet to spin along its axis
	MercurySpinRotator->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))));//The scale allows for the objects to be custom sized
	MercurySpinRotator->GetChild(0)->AddChild(new GeometryNode(mercury));//Finally the GeometryNode is added to the graph
	//Below is the same as above just for the labelled planet
	
	//Venus 
	VenusOrbitRotator->AddChild(new TransformNode(glm::translate(glm::mat4(), glm::vec3(15, 0, 0))));
	VenusOrbitRotator->GetChild(0)->AddChild(VenusSpinRotator);
	VenusSpinRotator->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(1.5f, 1.5f, 1.5f))));
	VenusSpinRotator->GetChild(0)->AddChild(redSpline);

	//Earth
	EarthOrbitRotator->AddChild(new TransformNode(glm::translate(glm::mat4(), glm::vec3(25, 0, 0))));
	EarthOrbitRotator->GetChild(0)->AddChild(MoonOrbitRotator);
	EarthOrbitRotator->GetChild(0)->AddChild(EarthSpinRotator);
	MoonOrbitRotator->AddChild(new TransformNode(glm::translate(glm::mat4(), glm::vec3(5, 0, 0))));
	MoonOrbitRotator->GetChild(0)->AddChild(new GeometryNode(moon));
	EarthSpinRotator->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(2, -2, 2))));
	EarthSpinRotator->GetChild(0)->AddChild(new GeometryNode(earth));

	//Mars
	MarsOrbitRotator->AddChild(new TransformNode(glm::translate(glm::mat4(), glm::vec3(35, 0, 0))));
	MarsOrbitRotator->GetChild(0)->AddChild(MarsSpinRotator);
	MarsSpinRotator->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(1, 1, 1))));
	MarsSpinRotator->GetChild(0)->AddChild(new GeometryNode(mars));


	//Jupiter
	JupiterOrbitRotator->AddChild(new TransformNode(glm::translate(glm::mat4(), glm::vec3(50, 0, 0))));
	JupiterOrbitRotator->GetChild(0)->AddChild(JupiterSpinRotator);
	JupiterSpinRotator->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(5, 5, 5))));
	JupiterSpinRotator->GetChild(0)->AddChild(new GeometryNode(jupiter));

	//Saturn
	SaturnOrbitRotator->AddChild(new TransformNode(glm::translate(glm::mat4(), glm::vec3(70, 0, 0))));
	SaturnOrbitRotator->GetChild(0)->AddChild(SaturnSpinRotator);
	SaturnSpinRotator->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(5, 5, 5))));
	SaturnSpinRotator->GetChild(0)->AddChild(new GeometryNode(saturn));
	SaturnSpinRotator->GetChild(0)->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(2,2,2))));
	SaturnSpinRotator->GetChild(0)->GetChild(1)->AddChild(ringGN);

	//Here we create the splinePoints that we will place within the scene
	std::vector<glm::vec3> lineVerts;
	splinePoints.push_back(glm::vec3(-20, 1, 10));
	splinePoints.push_back(glm::vec3(20, 1, 30));
	splinePoints.push_back(glm::vec3(40, 1, 6));
	splinePoints.push_back(glm::vec3(24, 1, -12));
	splinePoints.push_back(glm::vec3(50, 1, -21));
	splinePoints.push_back(glm::vec3(10, 1, -60));
	splinePoints.push_back(glm::vec3(-18, 1, -40));
	splinePoints.push_back(glm::vec3(-18, 1, 18));


	for (glm::vec3 pos : splinePoints)
	{
		TransformNode* sphereTrans = new TransformNode(glm::translate(glm::mat4(), pos)); // for each spline point we are making a Node that has the previously determined position
		light->AddChild(sphereTrans);
		sphereTrans->AddChild(redSpline);
	}
	
	//This is the creation of the actual spline, for this we are using the Catmull Rom spline, an alternative would be a Bezier Spline
	CRSpline c(splinePoints, true, 0.5f); //We have set the tension of the curve to be 0.5f this creates a tight spline but still allows for a flow between the nodes
	for (int i = 0; i < 500; i++)
	{
		float u = 8.0f * i / 499.0f;
		lineVerts.push_back(c.GetPoint(u));
	}
	TransformNode *splineTN = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0, 0, 0)));
	light->AddChild(splineTN);
	PolyLine* p = new PolyLine(lineVerts, glm::vec4(1, 0, 0, 1));
	splineTN->AddChild(new GeometryNode(p));
	

	TransformNode* sphereTrans = new TransformNode(glm::mat4());
	light->AddChild(sphereTrans);
	sphereTrans->AddChild(meshScaler);
	meshScaler->AddChild(meshNode);
	sphereTrans->SetUpdateCallback(new SplineAnimationCallback(sphereTrans, new CRSpline(splinePoints, true, 0.5f), 1.0f));


	

	return light;
}