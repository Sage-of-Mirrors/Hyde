#include "j3dconv.hpp"
#include "object.hpp"

#include <bstream.h>
#include <tiny_gltf.h>

int main() {
    tinygltf::Model model;
    libj3dconv::LoadGltf(&model, "D:\\SZS Tools\\J3DConv\\link.glb");

    CConverterObject t;
    t.Load(&model);

    bStream::CFileStream f("D:\\SZS Tools\\J3DConv\\test.bmd", bStream::Big, bStream::Out);
    t.WriteBMD(f);
}
