#include "stdafx.h"
#include "week5app.h"

Node* SolarSystem(Physics *phys);

class CamColl : public CameraCollisionCallback
{
	Physics* phys;
public:
	CamColl(Physics* phys) : phys(phys) {}
	glm::vec3 operator()(const glm::vec3& pos, const glm::vec3& newPos, float rad) const override
	{
		std::vector<ContactInfo> contacts = phys->CollideWithWorld(newPos, rad);
		glm::vec3 finalPos = newPos;
		for (auto c : contacts)
		{
			finalPos += c.normal * c.depth;
		}
		return finalPos;
	}
	virtual ~CamColl() {}
};


void Week5App::Init()
{
	// setup the camera
	eye = glm::vec3(0.0f, 30.0f, -50.0f);
	lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0); // y is up!

	physics = new Physics();

	

	camera = new FlyCam(eye, lookAt);

	sceneGraphRoot = SolarSystem(physics);

	dynamicObjectNode = new GroupNode();
	sceneGraphRoot->AddChild(dynamicObjectNode);

	projectileNode = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.2f, 0.2f, 0.2f)));
	SphereGeometry geom(20);
	TexturedLit* mesh = new TexturedLit(geom, "textures/grid.jpg");
	projectileNode->AddChild(new GeometryNode(mesh));

	// render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Week5App::Draw()
{
	// camera
	glm::mat4 view;
	glm::mat4 proj;

	eye = camera->GetPos();
	lookAt = camera->GetLookAt();
	up = camera->GetUp();
	view = glm::lookAt(eye, lookAt, up);
	proj = glm::perspective(glm::radians(60.0f), (float)SOF::Window::GetWidth() / (float)SOF::Window::GetHeight(), 0.1f, 1000.0f);

	
	// Render the scene graph
	RenderVisitor rv(view, proj);
	rv.Traverse(sceneGraphRoot);
}

void Week5App::Update(float deltaT)
{
	// Upadate the scene graph
	UpdateVisitor uv(deltaT);
	uv.Traverse(sceneGraphRoot);
	physics->Update(deltaT);

	if (SOF::Window::IsKeyDown(GLFW_KEY_W))
		camera->Move(10.0f*deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_S))
		camera->Move(-10.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_A))
		camera->Strafe(-10.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_D))
		camera->Strafe(10.0f * deltaT);

	if (SOF::Window::IsKeyDown(GLFW_KEY_J))
		camera->Pan(40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_L))
		camera->Pan(-40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_I))
		camera->LookUpDown(40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_K))
		camera->LookUpDown(-40.0f * deltaT);

	if (SOF::Window::IsKeyDown(GLFW_KEY_SPACE))
		Shoot();

}

void Week5App::Shoot() {
	
	TransformNode* node = new TransformNode(glm::mat4());
	dynamicObjectNode->AddChild(node);
	node->AddChild(projectileNode);
	float speed = 20.0f;
	glm::vec3 vel = glm::normalize(camera->GetLookAt() - camera->GetPos()) * speed;
	Projectile* proj = new Projectile(camera->GetPos(), vel, node);
	physics->AddProjectile(proj); 
	
}


