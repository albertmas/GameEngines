#include "ComponentMesh.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTexture.h"
#include "ComponentTransform.h"
#include "ModuleMeshLoader.h"
#include "ModuleTextureLoader.h"
#include "ModuleSceneLoader.h"
#include "ModuleRenderer3D.h"
#include "ModuleImGui.h"

#include "mmgr/mmgr.h"


ComponentMesh::ComponentMesh(GameObject* gameobject)
{
	my_go = gameobject;
	type = MESH;
	UUID = pcg32_random_r(&App->rng);
}

ComponentMesh::~ComponentMesh()
{
}


bool ComponentMesh::Update()
{
	if (active && go_mesh->hasTriFaces)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go_mesh->id_indices);
		glVertexPointer(3, GL_FLOAT, 0, &go_mesh->vertices[0]);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if (go_mesh->texCoords != nullptr)
			glTexCoordPointer(2, GL_FLOAT, 0, &go_mesh->texCoords[0]);

		glDrawElements(GL_TRIANGLES, go_mesh->num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (mesh_material)
		{
			if (mesh_material->texture->id != 0)
				glBindTexture(GL_TEXTURE_2D, 0);
			else
				glColor3f(1.0, 1.0, 1.0);
		}
		
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);


		if (App->renderer3D->GetNormals())
		{
			for (int j = 0; j < go_mesh->num_normals; j++)
			{
				glBegin(GL_LINES);
				glColor3f(0.5f, 0.5f, 0.5f);
				glLineWidth(2.0f);
				glVertex3f(go_mesh->vertices[j], go_mesh->vertices[j + 1], go_mesh->vertices[j + 2]);
				glVertex3f(go_mesh->vertices[j] - go_mesh->normals[j] * 3, go_mesh->vertices[j + 1] - go_mesh->normals[j + 1] * 3, go_mesh->vertices[j + 2] - go_mesh->normals[j + 2] * 3);
				glLineWidth(1.0f);
				glColor3f(1, 1, 1);
				glEnd();
			}
		}
	}

	return true;
}

void ComponentMesh::SetMesh(FBXMesh* mesh)
{
	go_mesh = mesh;

	my_go->CalcGlobalTransform();
}

void ComponentMesh::SetCompTexture(ComponentTexture* compTex)
{
	mesh_material = compTex;
}

void ComponentMesh::SetInspectorInfo()
{
	ImGui::Spacing();
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (go_mesh != nullptr)
		{
			ImGui::PushID("MeshActive");
			ImGui::Checkbox("Active", &active);
			ImGui::Spacing();
			ImGui::PopID();

			ImGui::Text("Triangles: %i", go_mesh->num_triangles);
			ImGui::Text("Vertices: %i", go_mesh->num_vertices);
		}
	}
}

Value ComponentMesh::Save(Document::AllocatorType& allocator) const
{
	Value CompArray(kObjectType);
	CompArray.AddMember("Type", type, allocator);
	CompArray.AddMember("Active", active, allocator);
	CompArray.AddMember("UUID", UUID, allocator);
	Value name(go_mesh->meshName.c_str(), go_mesh->meshName.size(), allocator);
	CompArray.AddMember("Mesh", name, allocator);

	return CompArray;
}

bool ComponentMesh::Load(Document& document)
{
	return true;
}