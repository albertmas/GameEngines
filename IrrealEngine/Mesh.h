#ifndef _MESH_
#define _MESH

#include "Globals.h"
#include "Primitive.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Open_GL.h"

enum MESH_TYPE { //All geometric forms will be included here
	CUBE_MESH = 0,
	SPHERE_MESH
	
};

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	void DrawCube()const;
	void DrawIndexCube() const;
	void DrawSphere() const;

	void DefineVerticesForACube(float3 _position, float size);
	void DefineVerticesAndIndicesForACube(float3 _position, float size);
	void DefineVerticesForASphere(float3 _position, float radius,uint secCount, uint stCount);


private:

	float3 color;
	MESH_TYPE type;

	float3 position = float3(0.f, 0.f, 0.f);
	Quat rotation = Quat::identity;
	float3 scale = float3(1.f, 1.f, 1.f);

	float3* vertices;
	float3* unique_vertices;
	uint* vertices_indices;

	uint num_vertices;
	uint num_unique_vertices;
	uint num_indices;

	uint id_vertices;
	uint id_unique_vertices;
	uint id_indices;

	//Sphere 

	std::vector<float> sphere_vertices;
	std::vector<uint> sphere_indices;



};






#endif