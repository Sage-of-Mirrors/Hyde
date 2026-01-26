#include "geometry/shape.h"
#include "geometry/vertexdata.h"

#include <algorithm>
#include <tri_stripper.h>
#include <ufbx.h>

j3d::Shape::Shape(ufbx_mesh* srcMesh, ufbx_mesh_part& partInfo, VertexData& vertexData)
	: m_materialName(srcMesh->materials[partInfo.index]->name.data), m_jointIndex(0),
	  m_hasNormals(srcMesh->vertex_normal.exists),
	  m_hasColor(srcMesh->color_sets.count >= 1, srcMesh->color_sets.count >= 2),
	  m_hasTexCoord(srcMesh->uv_sets.count >= 1, srcMesh->uv_sets.count >= 2,
		  srcMesh->uv_sets.count >= 3, srcMesh->uv_sets.count >= 4,
		  srcMesh->uv_sets.count >= 5, srcMesh->uv_sets.count >= 6,
		  srcMesh->uv_sets.count >= 7, srcMesh->uv_sets.count >= 8),
	  m_hasSkinning(srcMesh->skin_deformers.count >= 1)
{
	CreateVertexLists(srcMesh, partInfo, vertexData);
	DecideJointIndex();
	CreateTristrips();
}

j3d::Shape::VertexIndexData j3d::Shape::CreateVertex(ufbx_mesh* mesh, uint32_t index, VertexData& vertexData)
{
	VertexIndexData vertex;

	// Position always exists.
	vertex.m_position = vertexData.AddPosition(mesh->vertex_position[index]);
	
	if (m_hasNormals)
	{
		vertex.m_normal = vertexData.AddNormal(mesh->vertex_normal[index]);
	}
	if (m_hasColor[0])
	{
		vertex.m_color[0] = vertexData.AddColor(mesh->color_sets[0].vertex_color[index], 0);
	}
	if (m_hasColor[1])
	{
		vertex.m_color[1] = vertexData.AddColor(mesh->color_sets[1].vertex_color[index], 1);
	}
	for (uint32_t i = 0; i < 8; i++)
	{
		if (m_hasTexCoord[i])
		{
			vertex.m_texCoord[i] = vertexData.AddTexCoord(mesh->uv_sets[i].vertex_uv[index], i);
		}
	}
	if (m_hasSkinning)
	{
		VertexEnvelope envelope;

		const ufbx_skin_deformer* skinDeformer = mesh->skin_deformers[0];
		const ufbx_skin_vertex& skinVertex = skinDeformer->vertices[mesh->vertex_indices[index]];

		for (uint32_t i = 0; i < std::min(skinVertex.num_weights, VertexEnvelope::kMaxWeights); i++)
		{
			const ufbx_skin_weight& skinWeight = skinDeformer->weights[skinVertex.weight_begin + i];

			envelope.m_weightCount++;
			envelope.m_jointIndices[i] = skinWeight.cluster_index;
			envelope.m_weights[i] = static_cast<float>(skinWeight.weight);
		}

		auto itr = std::find(m_vertexEnvelopes.begin(), m_vertexEnvelopes.end(), envelope);
		if (itr != m_vertexEnvelopes.end())
		{
			vertex.m_envelope = static_cast<uint8_t>(itr - m_vertexEnvelopes.begin());
		}
		else
		{
			vertex.m_envelope = static_cast<uint8_t>(m_vertexEnvelopes.size());
			m_vertexEnvelopes.push_back(envelope);
		}
	}

	return vertex;
}

void j3d::Shape::CreateVertexLists(ufbx_mesh* mesh, ufbx_mesh_part& partInfo, VertexData& vertexData)
{
	for (uint32_t& faceIdx : partInfo.face_indices)
	{
		ufbx_face& face = mesh->faces[faceIdx];
		for (uint32_t i = 0; i < face.num_indices; i++)
		{
			VertexIndexData newVertex = CreateVertex(mesh, face.index_begin + i, vertexData);

			auto itr = std::find(m_vertices.begin(), m_vertices.end(), newVertex);
			if (itr != m_vertices.end())
			{
				m_indices.push_back(itr - m_vertices.begin());
				continue;
			}

			m_indices.push_back(m_vertices.size());
			m_vertices.push_back(newVertex);
		}
	}
}

void j3d::Shape::DecideJointIndex()
{
	for (const auto& env : m_vertexEnvelopes)
	{
		// There's at least 1 vertex with multiple joints influencing it.
		// This shape should belong to the root joint.
		if (env.m_weightCount > 1)
		{
			m_jointIndex = 0;
			break;
		}
		// We already set the joint index earlier, but we found a vertex
		// bound to a different joint... this shape should belong to the root joint then.
		if (m_jointIndex != 0 && env.m_jointIndices[0] != m_jointIndex)
		{
			m_jointIndex = 0;
			break;
		}

		m_jointIndex = env.m_jointIndices[0];
	}
}

void j3d::Shape::CreateTristrips()
{
	triangle_stripper::tri_stripper stripper(m_indices);
	stripper.SetCacheSize(0);

	triangle_stripper::primitive_vector strippedPrimitives;
	stripper.Strip(&strippedPrimitives);

	std::sort(strippedPrimitives.begin(), strippedPrimitives.end(), [](const auto& a, const auto& b) { return a.Indices.size() > b.Indices.size(); });
}
