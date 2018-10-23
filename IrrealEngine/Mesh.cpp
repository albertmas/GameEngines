#include "Mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::DrawCube() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void Mesh::DrawIndexCube() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_unique_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::DrawMesh() const
{

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, data.id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.id_index);
	glDrawElements(GL_TRIANGLES, data.num_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::DefineVerticesAndIndicesForACube(float3 _position, float size)
{
	position = _position;

	glGenBuffers(1, (GLuint*) &(id_unique_vertices));
	glGenBuffers(1, (GLuint*) &(id_indices));

	type = CUBE_MESH;
	num_unique_vertices = 8;
	unique_vertices = new float3[num_unique_vertices];
	{
		//Front bottom left
		unique_vertices[0].x = position.x;
		unique_vertices[0].y = position.y;
		unique_vertices[0].z = position.z;

		//Front bottom Right
		unique_vertices[1].x = position.x + size;
		unique_vertices[1].y = position.y;
		unique_vertices[1].z = position.z;

		//Front top left
		unique_vertices[2].x = position.x;
		unique_vertices[2].y = position.y + size;
		unique_vertices[2].z = position.z;

		//Front top Rigth
		unique_vertices[3].x = position.x + size;
		unique_vertices[3].y = position.y + size;
		unique_vertices[3].z = position.z;

		//Back bottom left
		unique_vertices[4].x = position.x;
		unique_vertices[4].y = position.y;
		unique_vertices[4].z = position.z + size;

		//Back bottom Rigth
		unique_vertices[5].x = position.x + size;
		unique_vertices[5].y = position.y;
		unique_vertices[5].z = position.z + size;

		//Back top left
		unique_vertices[6].x = position.x;
		unique_vertices[6].y = position.y + size;
		unique_vertices[6].z = position.z + size;

		//Back top Right
		unique_vertices[7].x = position.x + size;
		unique_vertices[7].y = position.y + size;
		unique_vertices[7].z = position.z + size;

	}

	glBindBuffer(GL_ARRAY_BUFFER, id_unique_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_unique_vertices * 3, unique_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	num_indices = 36;
	vertices_indices = new uint[num_indices];

	{
		// Every two blocks we get 1 face of the cube
		vertices_indices[0] = 0;
		vertices_indices[1] = 1;
		vertices_indices[2] = 2;

		vertices_indices[3] = 2;
		vertices_indices[4] = 1;
		vertices_indices[5] = 3,

		//
		vertices_indices[6] = 1;
		vertices_indices[7] = 5;
		vertices_indices[8] = 3;

		vertices_indices[9] = 3;
		vertices_indices[10] = 5;
		vertices_indices[11] = 7;
		//
		vertices_indices[12] = 5;
		vertices_indices[13] = 4;
		vertices_indices[14] = 7;
		
		vertices_indices[15] = 7;
		vertices_indices[16] = 4;
		vertices_indices[17] = 6;
		//
		vertices_indices[18] = 4;
		vertices_indices[19] = 0;
		vertices_indices[20] = 6;

		vertices_indices[21] = 6;
		vertices_indices[22] = 0;
		vertices_indices[23] = 2;
		//
		vertices_indices[24] = 2;
		vertices_indices[25] = 3;
		vertices_indices[26] = 6;

		vertices_indices[27] = 6;
		vertices_indices[28] = 3;
		vertices_indices[29] = 7;
		//
		vertices_indices[30] = 4;
		vertices_indices[31] = 5;
		vertices_indices[32] = 0;

		vertices_indices[33] = 0;
		vertices_indices[34] = 5;
		vertices_indices[35] = 1;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*num_indices, vertices_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LOG("VERTEX+INDICES CUBE LOADED BUFFER_ID: %d", id_unique_vertices);
}


void Mesh::DefineVerticesForACube(float3 _position, float size)
{
	position = _position;

	glGenBuffers(1, (GLuint*) &(id_vertices));

	type = CUBE_MESH;
	num_vertices = 36;

	vertices = new float3[num_vertices];
	{
		//--------------->FACE1<----------------// front
		//Front bottom left
		vertices[0].x = position.x;
		vertices[0].y = position.y;
		vertices[0].z = position.z;
		//Front bottom Right
		vertices[1].x = position.x + size;
		vertices[1].y = position.y;
		vertices[1].z = position.z;
		//Front top left
		vertices[2].x = position.x;
		vertices[2].y = position.y + size;
		vertices[2].z = position.z;

		//Front top left
		vertices[3].x = position.x;
		vertices[3].y = position.y + size;
		vertices[3].z = position.z;
		//Front bottom Right
		vertices[4].x = position.x + size;
		vertices[4].y = position.y;
		vertices[4].z = position.z;
		//Front top Rigth
		vertices[5].x = position.x + size;
		vertices[5].y = position.y + size;
		vertices[5].z = position.z;

		//------------>FACE2<--------------// right
		//Front bottom Right
		vertices[6].x = position.x + size;
		vertices[6].y = position.y;
		vertices[6].z = position.z;
		//Back bottom Rigth
		vertices[7].x = position.x + size;
		vertices[7].y = position.y;
		vertices[7].z = position.z + size;
		//Front top Rigth
		vertices[8].x = position.x + size;
		vertices[8].y = position.y + size;
		vertices[8].z = position.z;

		//Front top Rigth
		vertices[9].x = position.x + size;
		vertices[9].y = position.y + size;
		vertices[9].z = position.z;
		//Back bottom Rigth
		vertices[10].x = position.x + size;
		vertices[10].y = position.y;
		vertices[10].z = position.z + size;
		//Back top Right
		vertices[11].x = position.x + size;
		vertices[11].y = position.y + size;
		vertices[11].z = position.z + size;

		//---------->FACE3<-------------// back
		//Back bottom Rigth
		vertices[12].x = position.x + size;
		vertices[12].y = position.y;
		vertices[12].z = position.z + size;
		//Back bottom left
		vertices[13].x = position.x;
		vertices[13].y = position.y;
		vertices[13].z = position.z + size;
		//Back top Right
		vertices[14].x = position.x + size;
		vertices[14].y = position.y + size;
		vertices[14].z = position.z + size;

		//Back top Right
		vertices[15].x = position.x + size;
		vertices[15].y = position.y + size;
		vertices[15].z = position.z + size;
		//Back bottom left
		vertices[16].x = position.x;
		vertices[16].y = position.y;
		vertices[16].z = position.z + size;
		//Back top left
		vertices[17].x = position.x;
		vertices[17].y = position.y + size;
		vertices[17].z = position.z + size;

		//------------>FACE4<---------------// left
		//Back bottom left
		vertices[18].x = position.x;
		vertices[18].y = position.y;
		vertices[18].z = position.z + size;
		//Front bottom left
		vertices[19].x = position.x;
		vertices[19].y = position.y;
		vertices[19].z = position.z;
		//Back top left
		vertices[20].x = position.x;
		vertices[20].y = position.y + size;
		vertices[20].z = position.z + size;


		//Back top left
		vertices[21].x = position.x;
		vertices[21].y = position.y + size;
		vertices[21].z = position.z + size;
		//Front bottom left
		vertices[22].x = position.x;
		vertices[22].y = position.y;
		vertices[22].z = position.z;
		//Front top left
		vertices[23].x = position.x;
		vertices[23].y = position.y + size;
		vertices[23].z = position.z;

		//------------>FACE5<------------// top
		//Front top left
		vertices[24].x = position.x;
		vertices[24].y = position.y + size;
		vertices[24].z = position.z;
		//Front top Rigth
		vertices[25].x = position.x + size;
		vertices[25].y = position.y + size;
		vertices[25].z = position.z;
		//Back top left
		vertices[26].x = position.x;
		vertices[26].y = position.y + size;
		vertices[26].z = position.z + size;

		//Back top left
		vertices[27].x = position.x;
		vertices[27].y = position.y + size;
		vertices[27].z = position.z + size;
		//Front top Rigth
		vertices[28].x = position.x + size;
		vertices[28].y = position.y + size;
		vertices[28].z = position.z;
		//Back top Right
		vertices[29].x = position.x + size;
		vertices[29].y = position.y + size;
		vertices[29].z = position.z + size;

		//----------->FACE6<------------// bottom
		//Back bottom left
		vertices[30].x = position.x;
		vertices[30].y = position.y;
		vertices[30].z = position.z + size;
		//Back bottom Rigth
		vertices[31].x = position.x + size;
		vertices[31].y = position.y;
		vertices[31].z = position.z + size;
		//Front bottom left
		vertices[32].x = position.x;
		vertices[32].y = position.y;
		vertices[32].z = position.z;

		//Front bottom left
		vertices[33].x = position.x;
		vertices[33].y = position.y;
		vertices[33].z = position.z;
		//Back bottom Rigth
		vertices[34].x = position.x + size;
		vertices[34].y = position.y;
		vertices[34].z = position.z + size;
		//Front bottom Right
		vertices[35].x = position.x + size;
		vertices[35].y = position.y;
		vertices[35].z = position.z;


	}

	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LOG("VERTEX CUBE LOADED BUFFER_ID: %d", id_vertices);
}

void Mesh::DrawSphere() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, sphere_vertices.data());
	glDrawElements(GL_TRIANGLES, (uint)sphere_indices.size(), GL_UNSIGNED_INT, sphere_indices.data());
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Mesh::DefineVerticesForASphere(float3 _position, float radius, uint secCount, uint stCount)
{
	type = SPHERE_MESH;
	position = _position;
	float x, y, z, xz;                              // vertex position
	float sectorStep = 2 * PI / secCount;
	float stackStep = PI / stCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;				  // starting from pi/2 to -pi/2
		xz = radius * cosf(stackAngle);						 // r * cos(u)
		y = position.y + (radius * sinf(stackAngle));            // r * sin(u) + initial pos variation
															  // add (sectorCount+1) vertices per stack
															  // the first and last vertices have same position and normal, but different tex coods
		for (int j = 0; j <= secCount; ++j)
		{
			sectorAngle = j * sectorStep;
			// vertex position
			z = (position.z + xz) * cosf(sectorAngle);             // r * cos(u) * cos(v) + initial pos variation
			x = (position.x + xz) * sinf(sectorAngle);             // r * cos(u) * sin(v) + initial pos variation
			sphere_vertices.push_back(x);
			sphere_vertices.push_back(y);
			sphere_vertices.push_back(z);
		}
	}
	// indices
	uint k1, k2;
	for (int i = 0; i < stCount; ++i)
	{
		k1 = i * (secCount + 1);     // beginning of current stack
		k2 = k1 + secCount + 1;      // beginning of next stack
		for (int j = 0; j < secCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				sphere_indices.push_back(k1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k1 + 1);
			}
			if (i != (stCount - 1))
			{
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}
		}
	}
}