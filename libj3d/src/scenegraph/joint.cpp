#include "scenegraph/joint.h"

j3d::Joint::Joint(ufbx_node* srcNode, Joint* parent)
	: m_name(srcNode->name.data), m_clusterIndex(0), m_translation(srcNode->local_transform.translation),
	  m_scale(srcNode->local_transform.scale), m_parent(parent)
{
	m_rotation = ufbx_quat_to_euler(srcNode->local_transform.rotation, srcNode->rotation_order);

	for (ufbx_node* child : srcNode->children)
	{
		m_children.emplace_back(child, this);
	}
}
