#include "ComponentMesh.h"
#include "Application.h"
#include "ModuleFBXLoader.h"
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
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, go_mesh->id_indices);
		glVertexPointer(3, GL_FLOAT, 0, &go_mesh->vertices[0]);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (go_mesh->texCoords != nullptr)
			glTexCoordPointer(2, GL_FLOAT, 0, &go_mesh->texCoords[0]);

		glDrawElements(GL_TRIANGLES, go_mesh->num_indices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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