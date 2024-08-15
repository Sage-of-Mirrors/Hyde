#pragma once

#include "types.hpp"

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

class CShape;

struct SEnvelope {
    std::vector<uint16_t> JointIndices;
    std::vector<float> Weights;

    bool operator==(const SEnvelope& other) {
        return JointIndices == other.JointIndices && Weights == other.Weights;
    }

    bool operator!=(const SEnvelope& other) {
        return !operator==(other);
    }
};

class CEnvelopeData {
    // EVP1 data
    std::vector<SEnvelope> mEnvelopes;
    std::vector<glm::mat4> mInverseBindMatrices;

    // DRW1 data
    std::vector<uint16_t> mUnskinnedIndices;
    std::vector<uint16_t> mSkinnedIndices;

public:
    CEnvelopeData();
    ~CEnvelopeData();

    void ProcessEnvelopes(const shared_vector<CShape>& shapes);
    void ReadInverseBindMatrices(const tinygltf::Model* model, std::vector<bStream::CMemoryStream>& buffers);

    void WriteEVP1(bStream::CStream& stream);
    void WriteDRW1(bStream::CStream& stream);
};
