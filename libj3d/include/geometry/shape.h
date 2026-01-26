#pragma once

#include "types.h"

struct ufbx_mesh;
struct ufbx_mesh_part;

namespace j3d
{
	class VertexData;

	class Shape
	{
	public:
		Shape(ufbx_mesh* srcMesh, ufbx_mesh_part& partInfo, VertexData& vertexData);

	private:
		struct VertexIndexData
		{
			uint16_t m_position{ 0 };
			uint16_t m_normal{ 0 };
			uint16_t m_color[2]{ 0, 0 };
			uint16_t m_texCoord[8]{ 0, 0, 0, 0, 0, 0, 0, 0 };
			uint8_t m_envelope{ 0 };

			bool operator==(const VertexIndexData& other)
			{
				return m_position == other.m_position && m_normal == other.m_normal && m_color[0] == other.m_color[0]
					&& m_color[1] == other.m_color[1] && m_texCoord[0] == other.m_texCoord[0] && m_texCoord[1] == other.m_texCoord[1]
					&& m_texCoord[2] == other.m_texCoord[2] && m_texCoord[3] == other.m_texCoord[3] && m_texCoord[4] == other.m_texCoord[4]
					&& m_texCoord[5] == other.m_texCoord[5] && m_texCoord[6] == other.m_texCoord[6] && m_texCoord[7] == other.m_texCoord[7]
					&& m_envelope == other.m_envelope;
			}
		};

		struct VertexEnvelope
		{
			static constexpr uint32_t kMaxWeights = 4;

			uint32_t m_weightCount{ 0 };
			uint32_t m_jointIndices[kMaxWeights]{ 0, 0, 0, 0 };
			float m_weights[kMaxWeights]{ 0.0f, 0.0f, 0.0f, 0.0f };

			bool operator==(const VertexEnvelope& other)
			{
				return m_jointIndices[0] == other.m_jointIndices[0] && m_jointIndices[1] == other.m_jointIndices[1]
					&& m_jointIndices[2] == other.m_jointIndices[2] && m_jointIndices[3] == other.m_jointIndices[3]
					&& m_weights[0] == other.m_weights[0] && m_weights[1] == other.m_weights[1]
					&& m_weights[2] == other.m_weights[2] && m_weights[3] == other.m_weights[3];
			}
		};

		VertexIndexData CreateVertex(ufbx_mesh* mesh, uint32_t index, VertexData& vertexData);
		void CreateVertexLists(ufbx_mesh* mesh, ufbx_mesh_part& partInfo, VertexData& vertexData);
		void DecideJointIndex();
		void CreateTristrips();

		std::string m_materialName;
		uint32_t m_jointIndex;

		std::vector<VertexIndexData> m_vertices;
		std::vector<VertexEnvelope> m_vertexEnvelopes;
		std::vector<size_t> m_indices;

		bool m_hasNormals{false};
		bool m_hasColor[2]{false, false};
		bool m_hasTexCoord[8]{false, false, false, false, false, false, false, false};
		bool m_hasSkinning{false};
	};
} // namespace j3d
