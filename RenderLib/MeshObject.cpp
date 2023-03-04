#include "stdafx.h"
#include "MeshObject.h"

class BasicGeometryHelper : public GeometryHelper
{
	const std::vector<glm::vec3> &verts;
	const std::vector<glm::vec3> &normals;
	const std::vector<glm::vec3> &tangents;
	const std::vector<glm::vec2> &uvs;
public:
	BasicGeometryHelper(const std::vector<glm::vec3>& verts,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec3>& tangents,
		const std::vector<glm::vec2>& uvs) : verts(verts), normals(normals), tangents(tangents), uvs(uvs) {}

	const std::vector<glm::vec3>& GetPositionVector() const { return verts; }
	const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
	const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
	const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
	int GetNumVertices() const { return verts.size(); }
};

MeshObject::MeshObject(const std::string& meshFile)
{
	Load(meshFile);
}

void MeshObject::Draw(const Transforms& trans, const PointLight& light)
{
	for (auto o : subObjects)
	{
		o->Draw(trans, light);
	}
}

void MeshObject::Load(const std::string& fileName)
{
	std::vector<std::string> diffuseMapNames;
	std::vector<std::string> normalMapNames;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
		aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
		aiProcess_SortByPType | // ?
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_PreTransformVertices | //-- fixes the transformation issue.
		aiProcess_SplitByBoneCount |
		aiProcess_FlipUVs);

	if (scene == nullptr)
	{
		std::cerr << "failed to load " << fileName << std::endl;
		return;
	}

	int numMeshes = scene->mNumMeshes;

	// process materials
	// extract the path 
	std::string basePath; 
	size_t found;
	found = fileName.find_last_of("/\\");
	if (found != std::string::npos)
		basePath = fileName.substr(0, found+1);

	for (int i = 0; i < scene->mNumMaterials; i++)
	{

			aiMaterial* mat = scene->mMaterials[i];
			aiString path;
			aiReturn texFound = mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			if (texFound == AI_SUCCESS)
				diffuseMapNames.push_back(basePath + path.C_Str());
			else
				diffuseMapNames.push_back("textures/grid.jpg");
			texFound = mat->GetTexture(aiTextureType_NORMALS, 0, &path);
			if (texFound == AI_SUCCESS)
				normalMapNames.push_back(basePath + path.C_Str());
			else
				normalMapNames.push_back("textures/grid.jpg");

	}
	// process meshes
	for (int i = 0; i < numMeshes; i++)
	{
		std::vector<glm::vec3> vPos;
		std::vector<glm::vec3> vNorm;
		std::vector<glm::vec3> vTangent;
		std::vector<glm::vec2> vUV;
		std::vector<int> indices;

		int iMaterial;

		aiMesh* mesh = scene->mMeshes[i];
		iMaterial = mesh->mMaterialIndex;

		int numVerts = mesh->mNumVertices;
		for (int i = 0; i < numVerts; i++)
		{
			vPos.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
			vNorm.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
			if (mesh->mTangents == nullptr)
				vTangent.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
			else
				vTangent.push_back(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
			if (mesh->mTextureCoords[0] == nullptr)
				vUV.push_back(glm::vec2(0.0f, 0.0f));
			else
				vUV.push_back(glm::vec3(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, mesh->mTextureCoords[0][i].z));
		}

		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
		// TODO: use this data to construct TexturedLit object
		std::vector<glm::vec3> pos;
		std::vector<glm::vec3> norm;
		std::vector<glm::vec3> tangent;
		std::vector<glm::vec2> uv;

		for (int i : indices)
		{
			pos.push_back(vPos[i]);
			norm.push_back(vNorm[i]);
			tangent.push_back(vTangent[i]);
			uv.push_back(vUV[i]);
		}
		BasicGeometryHelper geom(pos, norm, tangent, uv);
		TexturedLit* obj = new TexturedLit(geom, diffuseMapNames[iMaterial], normalMapNames[iMaterial]);
		subObjects.push_back(obj);
	}
}
