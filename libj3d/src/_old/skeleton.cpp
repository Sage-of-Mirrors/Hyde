#include "skeleton.hpp"
#include "shape.hpp"
#include "jutnametab.hpp"
#include "util.hpp"

#include <bstream.h>
#include <tiny_gltf.h>
#include <ufbx.h>

#include <algorithm>

const float INT16_RAD_ANGLE_RATIO = 32768.0f / glm::pi<float>();

/* SJoint */

void SJoint::WriteHierarchyRecursive(bStream::CStream& stream) {
    //stream.writeUInt16(static_cast<uint16_t>(EHierarchyNodeType::Joint));
    //stream.writeUInt16(JointIndex);

    //if (AttachedShapes.size() != 0) {
    //    // Sort shapes by material name, ascending alphabetically
    //    std::sort(
    //        AttachedShapes.begin(),
    //        AttachedShapes.end(),
    //        [](const std::shared_ptr<CShape> a, const std::shared_ptr<CShape> b) {
    //            return a->GetMaterialName() < b->GetMaterialName();
    //        }
    //    );

    //    for (const std::shared_ptr<CShape> s : AttachedShapes) {
    //        stream.writeUInt16(static_cast<uint16_t>(EHierarchyNodeType::Down));
    //        stream.writeUInt16(0);

    //        stream.writeUInt16(static_cast<uint16_t>(EHierarchyNodeType::Material));
    //        stream.writeUInt16(s->GetMaterialIndex());

    //        stream.writeUInt16(static_cast<uint16_t>(EHierarchyNodeType::Down));
    //        stream.writeUInt16(0);

    //        stream.writeUInt16(static_cast<uint16_t>(EHierarchyNodeType::Shape));
    //        stream.writeUInt16(s->GetIndex());
    //    }
    //}

    //if (Children.size() != 0) {
    //    stream.writeUInt16(static_cast<uint16_t>(EHierarchyNodeType::Down));
    //    stream.writeUInt16(0);

    //    for (auto j : Children) {
    //        j->WriteHierarchyRecursive(stream);
    //    }

    //    stream.writeUInt16(static_cast<uint16_t>(EHierarchyNodeType::Up));
    //    stream.writeUInt16(0);
    //}

    //// We wrote 2 "down"s for each attached shape, so write as many "up"s.
    //for (uint32_t i = 0; i < AttachedShapes.size() * 2; i++) {
    //    stream.writeUInt16(static_cast<uint16_t>(EHierarchyNodeType::Up));
    //    stream.writeUInt16(0);
    //}
}

/* CSkeletonData */

CSkeletonData::CSkeletonData() : mRootJoint(nullptr) {

}

CSkeletonData::~CSkeletonData() {
    mRootJoint = nullptr;
    mJoints.clear();
}

void CSkeletonData::BuildHierarchyRecursive(const ufbx_node* current, std::shared_ptr<SJoint> parent) {
    //auto jnt = std::make_shared<SJoint>();
    //jnt->Name = current->name.data;
    //jnt->JointIndex = mJoints.size();
    //
    //jnt->Parent = parent;
    //if (parent != nullptr) {
    //    parent->Children.push_back(jnt);
    //}

    //const ufbx_vec3& trans = current->local_transform.translation;
    //jnt->Translation = glm::vec3(trans.x, trans.y, trans.z);

    //const ufbx_quat& rot = current->local_transform.rotation;
    //jnt->Rotation = glm::quat(rot.w, rot.x, rot.y, rot.z);

    //const ufbx_vec3& scale = current->local_transform.scale;
    //jnt->Scale = glm::vec3(scale.x, scale.y, scale.z);

    //mJoints.push_back(jnt);

    //for (const ufbx_node* child : current->children) {
    //    BuildHierarchyRecursive(child, jnt);
    //}
}

void CSkeletonData::BuildSkeleton(ufbx_scene* scene) {
    //ufbx_node* rootJointNode = nullptr;

    //for (const auto& node : scene->root_node->children) {
    //    if (node->attrib_type != UFBX_ELEMENT_EMPTY) {
    //        continue;
    //    }

    //    if (node->children.count != 0 && node->children[0]->attrib_type == UFBX_ELEMENT_BONE) {
    //        rootJointNode = node->children[0];
    //        break;
    //    }
    //}

    //if (rootJointNode == nullptr) {
    //    mRootJoint = std::make_shared<SJoint>();
    //    mRootJoint->Name = "root";

    //    mJoints.push_back(mRootJoint);
    //    return;
    //}

    //BuildHierarchyRecursive(rootJointNode, nullptr);
    //mRootJoint = mJoints[0];
}

void CSkeletonData::AttachShapesToSkeleton(std::vector<CShape*>& shapes) {
    //for (auto s : shapes) {
    //    mJoints[s->GetJointIndex()]->AttachedShapes.push_back(s);
    //}
}

void CSkeletonData::WriteINF1(bStream::CStream& stream, uint32_t vertexCount) {
    //size_t streamStartPos = stream.tell();

    //// Write header
    //stream.writeUInt32(0x494E4631); // FourCC ('INF1')
    //stream.writeUInt32(0);          // Placeholder for section size
    //stream.writeUInt16(0);          // "Misc flags"?
    //stream.writeUInt16(UINT16_MAX); // Padding

    //stream.writeUInt32(0);           // Matrix group count
    //stream.writeUInt32(vertexCount); // Vertex count
    //stream.writeUInt32(0x18);        // Offset to hierarchy data; always 0x18

    //// Write hierarchy
    //mRootJoint->WriteHierarchyRecursive(stream);

    //// End the hierarchy
    //stream.writeUInt32(0);

    //Util::PadStreamWithString(&stream, 32);

    //// Write section size
    //Util::WriteOffset(&stream, streamStartPos, 0x04);
}

void CSkeletonData::WriteJNT1(bStream::CStream& stream) {
    //JUTNameTab jointNameTable;
    //size_t streamStartPos = stream.tell();

    //// Header
    //stream.writeUInt32(0x4A4E5431);     // FourCC ('JNT1')
    //stream.writeUInt32(0);              // Placeholder for section size
    //stream.writeUInt16(mJoints.size()); // Number of joints
    //stream.writeUInt16(UINT16_MAX);     // Padding

    //// Offsets
    //stream.writeUInt32(0); // Placeholder for joint data offset
    //stream.writeUInt32(0); // Placeholder for instance table offset
    //stream.writeUInt32(0); // Placeholder for name table offset

    //// Write joint data offset
    //Util::WriteOffset(&stream, streamStartPos, 0x0C);
    //// Write joint data
    //for (const auto j : mJoints) {
    //    jointNameTable.AddName(j->Name);

    //    // Header
    //    stream.writeUInt16(j->MatrixType);
    //    stream.writeUInt8(j->bDoNotInheritParentScale);
    //    stream.writeUInt8(UINT8_MAX);

    //    // Scale
    //    stream.writeFloat(j->Scale.x);
    //    stream.writeFloat(j->Scale.y);
    //    stream.writeFloat(j->Scale.z);

    //    // Rotation
    //    glm::vec3 eulerAngles = glm::eulerAngles(j->Rotation);
    //    stream.writeInt16(static_cast<int16_t>(eulerAngles.x * INT16_RAD_ANGLE_RATIO));
    //    stream.writeInt16(static_cast<int16_t>(eulerAngles.y * INT16_RAD_ANGLE_RATIO));
    //    stream.writeInt16(static_cast<int16_t>(eulerAngles.z * INT16_RAD_ANGLE_RATIO));
    //    stream.writeUInt16(UINT16_MAX);

    //    // Translation
    //    stream.writeFloat(j->Translation.x);
    //    stream.writeFloat(j->Translation.y);
    //    stream.writeFloat(j->Translation.z);

    //    // Bounding sphere radius
    //    stream.writeFloat(j->Bounds.BoundingSphereRadius);

    //    // Bounding box min
    //    stream.writeFloat(j->Bounds.BoundingBoxMin.x);
    //    stream.writeFloat(j->Bounds.BoundingBoxMin.y);
    //    stream.writeFloat(j->Bounds.BoundingBoxMin.z);

    //    // Bounding box max
    //    stream.writeFloat(j->Bounds.BoundingBoxMax.x);
    //    stream.writeFloat(j->Bounds.BoundingBoxMax.y);
    //    stream.writeFloat(j->Bounds.BoundingBoxMax.z);
    //}

    //// Write instance table offset
    //Util::WriteOffset(&stream, streamStartPos, 0x10);
    //// Write instance table
    //for (uint16_t i = 0; i < mJoints.size(); i++) {
    //    stream.writeUInt16(i);
    //}

    //Util::PadStreamWithString(&stream, 4);

    //// Write name table offset
    //Util::WriteOffset(&stream, streamStartPos, 0x14);
    //// Write name table
    //jointNameTable.Serialize(&stream);

    //Util::PadStreamWithString(&stream, 32);

    //// Write section size
    //Util::WriteOffset(&stream, streamStartPos, 0x04);
}