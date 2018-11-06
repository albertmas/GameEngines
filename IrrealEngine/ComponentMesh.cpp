#include "ComponentMesh.h"
#include "Application.h"
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
	if (comp_active)
	{
		glColor3f(1.0, 1.0, 1.0);
		glEnableClientState(GL_VERTEX_ARRAY);
		if (go_mesh->texture != 0)
			glBindTexture(GL_TEXTURE_2D, go_mesh->texture);
		else
			glColor3f(go_mesh->color.x, go_mesh->color.y, go_mesh->color.z);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go_mesh->id_indices);
		glVertexPointer(3, GL_FLOAT, 0, &go_mesh->vertices[0]);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if (go_mesh->texCoords != nullptr)
			glTexCoordPointer(2, GL_FLOAT, 0, &go_mesh->texCoords[0]);
		glDrawElements(GL_TRIANGLES, go_mesh->num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		if (go_mesh->texture != 0)
			glBindTexture(GL_TEXTURE_2D, 0);
		else
			glColor3f(1.0, 1.0, 1.0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	return true;
}

void ComponentMesh::SetMesh(FBXMesh* mesh)
{
	go_mesh = mesh;
}

void ComponentMesh::SetInspectorInfo()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID(1);
		ImGui::Checkbox("Active", &comp_active);
		ImGui::PopID();

		ImGui::Text("Triangles: %i", go_mesh->num_triangles);
		ImGui::Text("Vertices: %i", go_mesh->num_vertices);
	}
}