#pragma once

#include "types.hpp"

#include "vertex.hpp"
#include "envelope.hpp"
#include "skeleton.hpp"
#include "shape.hpp"

class CConverterObject {
    std::vector<uint8_t*> mBuffers;
    std::vector<bStream::CMemoryStream> mBufferStreams;

    CVertexData mVertexData;
    CSkeletonData mSkeletonData;
    CEnvelopeData mEnvelopeData;
    CShapeData mShapeData;

    void WriteTEX1(bStream::CStream& stream, tinygltf::Model* model);

    void LoadBuffers(tinygltf::Model* model);

public:
    CConverterObject();
    ~CConverterObject();

    bool Load(tinygltf::Model* model);
    bool WriteBMD(bStream::CStream& stream);
};