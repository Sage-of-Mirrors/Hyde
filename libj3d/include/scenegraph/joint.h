#pragma once

#include "types.h"

#include <ufbx.h>

namespace j3d
{
	class Joint
	{
	public:
		Joint(ufbx_node* srcNode, Joint* parent = nullptr);

		const std::string& GetName() const { return m_name; }
		uint32_t GetClusterIndex() const { return m_clusterIndex; }
		const Joint* GetParent() const { return m_parent; }
		const std::vector<Joint>& GetChildren() const { return m_children; }

		void SetClusterIndex(uint32_t idx) { m_clusterIndex = idx; }

	private:
		std::string m_name;
		uint32_t m_clusterIndex;

		ufbx_vec3 m_translation;
		ufbx_vec3 m_scale;
		ufbx_vec3 m_rotation;

		Joint* m_parent;
		std::vector<Joint> m_children;
	};
}
