#pragma once

#include "types.hpp"

#include "vertex.hpp"
#include "envelope.hpp"
#include "skeleton.hpp"
#include "shape.hpp"

struct ufbx_scene;

class CConverterObject
{
public:
    CConverterObject();
    ~CConverterObject();

    bool Load(ufbx_scene* scene);
    bool SaveBMD(bStream::CStream& stream);

private:
    CVertexData mVertexData;
    CSkeletonData mSkeletonData;
    CEnvelopeData mEnvelopeData;
    CShapeData mShapeData;

    void WriteTEX1(bStream::CStream& stream, tinygltf::Model* model);
};