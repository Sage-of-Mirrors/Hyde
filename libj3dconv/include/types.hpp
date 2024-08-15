#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/vec3.hpp>

#include <cstdint>
#include <vector>
#include <memory>

namespace bStream {
    class CStream;
    class CMemoryStream;
}

namespace tinygltf {
    class Model;
    class Node;
}

template<typename T>
using shared_vector = std::vector<std::shared_ptr<T>>;
