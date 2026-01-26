#include "geometry/vertexdata.h"

#include <algorithm>
#include <ufbx.h>

namespace
{
	bool CompareVec2(const ufbx_vec2& lhs, const ufbx_vec2& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	bool CompareVec3(const ufbx_vec3& lhs, const ufbx_vec3& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}

	bool CompareVec4(const ufbx_vec4& lhs, const ufbx_vec4& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
	}
} // namespace

uint16_t j3d::VertexData::AddPosition(const ufbx_vec3& pos)
{
	auto itr = std::find_if(m_positions.begin(), m_positions.end(),
		[pos](const ufbx_vec3& other) { return CompareVec3(pos, other); });

	if (itr != m_positions.end())
	{
		return static_cast<uint16_t>(itr - m_positions.begin());
	}

	uint16_t newIndex = static_cast<uint16_t>(m_positions.size());
	m_positions.push_back(pos);

	return newIndex;
}

uint16_t j3d::VertexData::AddNormal(const ufbx_vec3& nrm)
{
	auto itr = std::find_if(m_normals.begin(), m_normals.end(),
		[nrm](const ufbx_vec3& other) { return CompareVec3(nrm, other); });

	if (itr != m_normals.end())
	{
		return static_cast<uint16_t>(itr - m_normals.begin());
	}

	uint16_t newIndex = static_cast<uint16_t>(m_normals.size());
	m_normals.push_back(nrm);

	return newIndex;
}

uint16_t j3d::VertexData::AddColor(const ufbx_vec4& col, uint32_t idx)
{
	auto itr = std::find_if(m_colors[idx].begin(), m_colors[idx].end(),
		[col](const ufbx_vec4& other) { return CompareVec4(col, other); });

	if (itr != m_colors[idx].end())
	{
		return static_cast<uint16_t>(itr - m_colors[idx].begin());
	}

	uint16_t newIndex = static_cast<uint16_t>(m_colors[idx].size());
	m_colors[idx].push_back(col);

	return newIndex;
}

uint16_t j3d::VertexData::AddTexCoord(const ufbx_vec2& txc, uint32_t idx)
{
	auto itr = std::find_if(m_texCoords[idx].begin(), m_texCoords[idx].end(),
		[txc](const ufbx_vec2& other) { return CompareVec2(txc, other); });

	if (itr != m_texCoords[idx].end())
	{
		return static_cast<uint16_t>(itr - m_texCoords[idx].begin());
	}

	uint16_t newIndex = static_cast<uint16_t>(m_texCoords[idx].size());
	m_texCoords[idx].push_back(txc);

	return newIndex;
}
