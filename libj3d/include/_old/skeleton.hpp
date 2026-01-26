#pragma once

#include "types.hpp"
#include "util.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <string>

struct ufbx_scene;
struct ufbx_node;
class CShape;

enum class EHierarchyNodeType {
    End = 0,
    Down = 1,
    Up = 2,
    Joint = 0x10,
    Material = 0x11,
    Shape = 0x12
};

struct SJoint {
    std::string Name = "";

    // J3D properties
    uint16_t MatrixType = 1;
    bool bDoNotInheritParentScale = true;

    glm::vec3 Translation = glm::zero<glm::vec3>();
    glm::quat Rotation = glm::identity<glm::quat>();
    glm::vec3 Scale = glm::one<glm::vec3>();

    Util::UConvBoundingVolume Bounds;

    // Hierarchy properties
    std::weak_ptr<SJoint> Parent;
    std::vector<SJoint> Children;

    // Utility properties
    uint32_t OriginalNodeIndex = UINT32_MAX;
    uint32_t JointIndex = UINT32_MAX;
    std::vector<CShape*> AttachedShapes;

    void WriteHierarchyRecursive(bStream::CStream& stream);
};

class CSkeletonData {
    std::vector<SJoint> mJoints;
    std::shared_ptr<SJoint> mRootJoint;

    void BuildHierarchyRecursive(const ufbx_node* current, std::shared_ptr<SJoint> parent);

public:
    CSkeletonData();
    ~CSkeletonData();

    void BuildSkeleton(tinygltf::Model* model);
    void BuildSkeleton(ufbx_scene* scene);

    void WriteINF1(bStream::CStream& stream, uint32_t vertexCount);
    void WriteJNT1(bStream::CStream& stream);

    void AttachShapesToSkeleton(std::vector<CShape*>& shapes);
};