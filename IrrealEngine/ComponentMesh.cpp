#include "ComponentMesh.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTexture.h"
#include "ModuleTextureLoader.h"
#include "ModuleSceneLoader.h"
#include "ModuleRenderer3D.h"


ComponentMesh::ComponentMesh(GameObject* gameobject)
{
	my_go = gameobject;
	type = MESH;
}

ComponentMesh::~ComponentMesh()
{
}


bool ComponentMesh::Update()
{
	if (active)
	{
		glColor3f(1.0, 1.0, 1.0);
		glEnableClientState(GL_VERTEX_ARRAY);
		if (mesh_material)
		{
			if (mesh_material->texture->id != 0)
				glBindTexture(GL_TEXTURE_2D, mesh_material->texture->id);
			else
				glColor3f(mesh_material->texture->color.x, mesh_material->texture->color.y, mesh_material->texture->color.z);
		}
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

		if (App->renderer3D->BB)
			App->renderer3D->DrawBB(go_mesh->bounding_box, { 1, 0, 0 });

		if (App->renderer3D->GetNormals())
		{
			for (int j = 0; j < go_mesh->num_normals; j++) {
				glBegin(GL_LINES);
				glLineWidth(2.0f);
				glVertex3f(go_mesh->vertices[j].x, go_mesh->vertices[j].y, go_mesh->vertices[j].z);
				glVertex3f(go_mesh->vertices[j].x - go_mesh->normals[j].x, go_mesh->vertices[j].y - go_mesh->normals[j].y, go_mesh->vertices[j].z - go_mesh->normals[j].z);
				glLineWidth(1.0f);
				glEnd();
			}
		}
	}

	return true;
}

void ComponentMesh::SetMesh(FBXMesh* mesh)
{
	go_mesh = mesh;
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
		ImGui::PushID("MeshActive");
		ImGui::Checkbox("Active", &active);
		ImGui::Spacing();
		ImGui::PopID();

		ImGui::Text("Triangles: %i", go_mesh->num_triangles);
		ImGui::Text("Vertices: %i", go_mesh->num_vertices);
	}
}

bool ComponentMesh::Save(Document& document, FileWriteStream& fws) const
{
	Document::AllocatorType& allocator = document.GetAllocator();
	// Save stuff
	Writer<FileWriteStream> writer(fws);
	return true;
}

bool ComponentMesh::Load(Document& document)
{
	return true;
}