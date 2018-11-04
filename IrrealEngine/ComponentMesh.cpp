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

void ComponentMesh::Update()
{
	if (comp_active)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go_mesh->id_indices);
		glVertexPointer(3, GL_FLOAT, 0, &go_mesh->vertices[0]);
		glDrawElements(GL_TRIANGLES, go_mesh->num_indices, GL_UNSIGNED_INT, NULL);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void ComponentMesh::SetMesh(FBXMesh* mesh)
{
	go_mesh = mesh;
}

void ComponentMesh::SetInspectorInfo()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active", &comp_active);

		ImGui::Text("Triangles: %i", go_mesh->num_triangles);
		ImGui::Text("Vertices: %i", go_mesh->num_vertices);
	}
}