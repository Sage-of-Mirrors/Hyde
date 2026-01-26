#pragma once

#include "types.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct ufbx_vec2;
struct ufbx_vec3;
struct ufbx_vec4;

namespace j3d
{
	class VertexData
	{
	public:
		uint16_t AddPosition(const ufbx_vec3& pos);
		uint16_t AddNormal(const ufbx_vec3& nrm);
		uint16_t AddColor(const ufbx_vec4& col, uint32_t idx);
		uint16_t AddTexCoord(const ufbx_vec2& txc, uint32_t idx);

		uint32_t GetVertexCount() const { return m_positions.size(); }

	private:
		std::vector<ufbx_vec3> m_positions;
		std::vector<ufbx_vec3> m_normals;
		std::vector<ufbx_vec4> m_colors[2];
		std::vector<ufbx_vec2> m_texCoords[8];
	};
} // namespace j3d
