#include "shape.hpp"
#include "vertex.hpp"

#include <tiny_gltf.h>
#include <bstream.h>
#include <tri_stripper.h>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>
#include <ufbx.h>

#include <algorithm>

const std::vector<std::string> VERTEX_ATTRIBUTE_NAMES = {
    "POSITION",
    "NORMAL",
    "TANGENT",
    "COLOR_0",
    "COLOR_1",
    "TEXCOORD_0",
    "TEXCOORD_1",
    "TEXCOORD_2",
    "TEXCOORD_3",
    "TEXCOORD_4",
    "TEXCOORD_5",
    "TEXCOORD_6",
    "TEXCOORD_7",
};

/* SPrimitive */

SPrimitive::~SPrimitive() {
    mVertices.clear();
}

/* CShape */

CShape::CShape() {

}

CShape::~CShape() {
    mPrimitives.clear();
}

void CShape::CalculateBoundingVolume(const std::vector<glm::vec4>& positions) {
    for (const glm::vec4& p : positions) {
        if (p.x > mBounds.BoundingBoxMax.x) {
            mBounds.BoundingBoxMax.x = p.x;
        }
        if (p.x < mBounds.BoundingBoxMin.x) {
            mBounds.BoundingBoxMin.x = p.x;
        }

        if (p.y > mBounds.BoundingBoxMax.y) {
            mBounds.BoundingBoxMax.y = p.y;
        }
        if (p.y < mBounds.BoundingBoxMin.y) {
            mBounds.BoundingBoxMin.y = p.y;
        }

        if (p.z > mBounds.BoundingBoxMax.z) {
            mBounds.BoundingBoxMax.z = p.z;
        }
        if (p.z < mBounds.BoundingBoxMin.z) {
            mBounds.BoundingBoxMin.z = p.z;
        }
    }
}

bool CShape::GetAttributeEnabled(EGXAttribute attr) const
{
    switch (attr)
    {
    case EGXAttribute::Position:
        return mHasPosition;
        break;
    case EGXAttribute::Normal:
        return mHasNormals;
        break;
    case EGXAttribute::Color0:
        return mHasColor0;
        break;
    case EGXAttribute::Color1:
        return mHasColor1;
        break;
    case EGXAttribute::TexCoord0:
        return mHasTexCoord0;
        break;
    case EGXAttribute::TexCoord1:
        return mHasTexCoord1;
        break;
    case EGXAttribute::TexCoord2:
        return mHasTexCoord2;
        break;
    case EGXAttribute::TexCoord3:
        return mHasTexCoord3;
        break;
    case EGXAttribute::TexCoord4:
        return mHasTexCoord4;
        break;
    case EGXAttribute::TexCoord5:
        return mHasTexCoord5;
        break;
    case EGXAttribute::TexCoord6:
        return mHasTexCoord6;
        break;
    case EGXAttribute::TexCoord7:
        return mHasTexCoord7;
        break;
    case EGXAttribute::PositionMatrixArray:
        return mHasPosMtx;
        break;
    default:
        return false;
    }
}

void CShape::SetAttributeEnabled(EGXAttribute attr)
{
    switch (attr)
    {
    case EGXAttribute::Position:
        mHasPosition = true;
        break;
    case EGXAttribute::Normal:
        mHasNormals = true;
        break;
    case EGXAttribute::Color0:
        mHasColor0 = true;
        break;
    case EGXAttribute::Color1:
        mHasColor1 = true;
        break;
    case EGXAttribute::TexCoord0:
        mHasTexCoord0 = true;
        break;
    case EGXAttribute::TexCoord1:
        mHasTexCoord1 = true;
        break;
    case EGXAttribute::TexCoord2:
        mHasTexCoord2 = true;
        break;
    case EGXAttribute::TexCoord3:
        mHasTexCoord3 = true;
        break;
    case EGXAttribute::TexCoord4:
        mHasTexCoord4 = true;
        break;
    case EGXAttribute::TexCoord5:
        mHasTexCoord5 = true;
        break;
    case EGXAttribute::TexCoord6:
        mHasTexCoord6 = true;
        break;
    case EGXAttribute::TexCoord7:
        mHasTexCoord7 = true;
        break;
    case EGXAttribute::PositionMatrixArray:
        mHasPosMtx = true;
        break;
    default:
        return;
    }
}

/* UConverterShapeData */

CShapeData::~CShapeData()
{
    for (auto shape : mShapes)
    {
        delete shape;
    }

    mShapes.clear();
}

void CShapeData::ProcessMeshes(ufbx_scene* scene)
{
    if (scene == nullptr || scene->meshes.count == 0)
    {
        return;
    }

    for (auto mesh : scene->meshes)
    {
        for (auto& matPart : mesh->material_parts)
        {
            mShapes.push_back(CreateShapeFromMeshPart(mesh, matPart));
        }
    }
}

CShape* CShapeData::CreateShapeFromMeshPart(const ufbx_mesh* mesh, const ufbx_mesh_part& partInfo)
{
    CShape* shape = new CShape();
    shape->SetIndex(static_cast<uint32_t>(mShapes.size()));
    shape->SetMaterialName(mesh->materials[partInfo.index]->name.data);

    if (mesh->vertex_position.exists)
    {
        shape->SetAttributeEnabled(EGXAttribute::Position);
    }
    if (mesh->vertex_normal.exists)
    {
        shape->SetAttributeEnabled(EGXAttribute::Normal);
    }
    if (mesh->color_sets.count >= 1)
    {
        shape->SetAttributeEnabled(EGXAttribute::Color0);
    }
    if (mesh->color_sets.count >= 2)
    {
        shape->SetAttributeEnabled(EGXAttribute::Color1);
    }

    return shape;
}

EGXAttribute GetVertexAttributeFromType(const std::string& type) {
    if (type == "POSITION") {
        return EGXAttribute::Position;
    }
    else if (type == "NORMAL") {
        return EGXAttribute::Normal;
    }
    else if (type == "TANGENT") {
        return EGXAttribute::NBT;
    }
    else if (type == "COLOR_0") {
        return EGXAttribute::Color0;
    }
    else if (type == "COLOR_1") {
        return EGXAttribute::Color1;
    }
    else if (type == "TEXCOORD_0") {
        return EGXAttribute::TexCoord0;
    }
    else if (type == "TEXCOORD_1") {
        return EGXAttribute::TexCoord1;
    }
    else if (type == "TEXCOORD_2") {
        return EGXAttribute::TexCoord2;
    }
    else if (type == "TEXCOORD_3") {
        return EGXAttribute::TexCoord3;
    }
    else if (type == "TEXCOORD_4") {
        return EGXAttribute::TexCoord4;
    }
    else if (type == "TEXCOORD_5") {
        return EGXAttribute::TexCoord5;
    }
    else if (type == "TEXCOORD_6") {
        return EGXAttribute::TexCoord6;
    }
    else if (type == "TEXCOORD_7") {
        return EGXAttribute::TexCoord7;
    }

    return EGXAttribute::Null;
}

//uint16_t CShapeData::AddUniqueVertexAttribute(std::vector<glm::vec4>& attribute, const glm::vec4& value) {
//    auto itr = std::find(attribute.begin(), attribute.end(), value);
//    if (itr != attribute.end()) {
//        return itr - attribute.begin();
//    }
//
//    uint16_t newIdx = attribute.size();
//    attribute.push_back(value);
//
//    return newIdx;
//}
//
//struct SVertex_ {
//    glm::vec4 mPosition{ 0.0f, 0.0f, 0.0f, 0.0f };
//    glm::vec4 mNormal{ 0.0f, 0.0f, 0.0f, 0.0f };
//    glm::vec4 mColor{ 0.0f, 0.0f, 0.0f, 0.0f };
//    glm::vec4 mTexCoord{ 0.0f, 0.0f, 0.0f, 0.0f };
//};
//
//void CShapeData::SplitShapeByMaterial(ufbx_mesh* mesh, ufbx_mesh_part& matPart) {
//    std::shared_ptr<CShape> shape = std::make_shared<CShape>();
//    shape->SetIndex(static_cast<uint32_t>(mShapes.size()));
//    shape->SetMaterialName(mesh->materials[matPart.index]->name.data);
//
//    std::vector<uint32_t> splitIndices;
//
//    for (auto& faceIdx : matPart.face_indices) {
//        const ufbx_face& curFace = mesh->faces[faceIdx];
//
//        for (uint32_t i = 0; i < curFace.num_indices; i++) {
//            splitIndices.push_back(curFace.index_begin + i);
//        }
//    }
//
//    std::map<EGXAttribute, std::vector<glm::vec4>> shapeAttributes;
//    std::vector<SVertex_> shapeVertices;
//    std::vector<size_t> shapeIndices;
//
//    for (uint32_t i : splitIndices) {
//        SVertex_ vert;
//
//        {
//            const ufbx_vec3& fbxPos = mesh->vertex_position[i];
//            const glm::vec4 glmPos(fbxPos.x, fbxPos.y, fbxPos.z, 1.0f);
//
//            std::vector<glm::vec4>& posList = shapeAttributes[EGXAttribute::Position];
//            vert.mPosition = glmPos;
//        }
//
//        if (mesh->vertex_normal.exists) {
//            const ufbx_vec3& fbxNrm = mesh->vertex_normal[i];
//            const glm::vec4 glmNrm(fbxNrm.x, fbxNrm.y, fbxNrm.z, 1.0f);
//
//            std::vector<glm::vec4>& nrmList = shapeAttributes[EGXAttribute::Normal];
//            vert.mNormal = glmNrm;
//        }
//
//        if (mesh->vertex_color.exists) {
//            for (uint32_t col = 0; col < 2; col++) {
//                if (col >= mesh->color_sets.count) {
//                    break;
//                }
//
//                const ufbx_vec4& fbxCol = mesh->color_sets[col].vertex_color[i];
//                const glm::vec4 glmCol(fbxCol.x, fbxCol.y, fbxCol.z, fbxCol.w);
//
//                EGXAttribute colAttr = static_cast<EGXAttribute>(static_cast<uint32_t>(EGXAttribute::Color0) + col);
//                std::vector<glm::vec4>& colList = shapeAttributes[colAttr];
//                vert.mColor = glmCol;
//            }
//        }
//
//        if (mesh->vertex_uv.exists) {
//            for (uint32_t uv = 0; uv < 8; uv++) {
//                if (uv >= mesh->uv_sets.count) {
//                    break;
//                }
//
//                const ufbx_vec2& fbxUV = mesh->uv_sets[uv].vertex_uv[i];
//                const glm::vec4 glmUV(fbxUV.x, fbxUV.y, 0.0f, 1.0f);
//
//                EGXAttribute uvAttr = static_cast<EGXAttribute>(static_cast<uint32_t>(EGXAttribute::TexCoord0) + uv);
//                std::vector<glm::vec4>& uvList = shapeAttributes[uvAttr];
//                vert.mTexCoord = glmUV;
//            }
//        }
//
//        shapeVertices.push_back(vert);
//    }
//
//    ufbx_vertex_stream streams[1] = { { shapeVertices.data(), shapeVertices.size(), sizeof(SVertex_)} };
//    std::vector<uint32_t> indicesDedup(matPart.num_triangles * 3);
//
//    size_t numVertices = ufbx_generate_indices(streams, 1, indicesDedup.data(), indicesDedup.size(), nullptr, nullptr);
//
//    shapeIndices.assign(indicesDedup.begin(), indicesDedup.end());
//
//    triangle_stripper::tri_stripper stripper(shapeIndices);
//    stripper.SetCacheSize(0);
//
//    triangle_stripper::primitive_vector strippedPrimitives;
//    stripper.Strip(&strippedPrimitives);
//
//    shape->CalculateBoundingVolume(shapeAttributes.at(EGXAttribute::Position));
//    mShapes.push_back(shape);
//}
