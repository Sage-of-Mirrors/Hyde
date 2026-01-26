#include "model.h"
#include "util/enums.h"

#include <bstream.h>
#include <ufbx.h>

j3d::Model::Model(ufbx_scene* fbxScene) : m_matrixType(2)
{
	LoadGeometry(fbxScene);
}

int32_t j3d::Model::Export()
{
	constexpr size_t kFileSizeOffset{ 8 };

	bStream::CFileStream strm("D:\\SZS Tools\\J3DConv\\test.bmd", bStream::Big, bStream::Out);

	WriteHeader(strm);

	WriteSection(strm, std::bind_front(&Model::WriteScenegraph, this));
	WriteSection(strm, std::bind_front(&Model::WriteVertexData, this));
	WriteSection(strm, std::bind_front(&Model::WriteEnvelopeData, this));
	WriteSection(strm, std::bind_front(&Model::WriteSkinningData, this));
	WriteSection(strm, std::bind_front(&Model::WriteGeometry, this));
	WriteSection(strm, std::bind_front(&Model::WriteMaterials, this));
	WriteSection(strm, std::bind_front(&Model::WriteTextures, this));

	size_t modelSize = strm.tell();
	strm.seek(kFileSizeOffset);
	strm.writeUInt32(static_cast<uint32_t>(modelSize));

	return 0;
}

void j3d::Model::LoadGeometry(ufbx_scene* fbxScene)
{
	if (fbxScene->meshes.count <= 0)
	{
		std::cout << "Failed to load shape data: FBX scene had no meshes!" << std::endl;
		return;
	}

	for (ufbx_mesh* mesh : fbxScene->meshes)
	{
		for (ufbx_mesh_part& part : mesh->material_parts)
		{
			m_shapes.emplace_back(mesh, part, m_vertexData);
		}
	}
}

void j3d::Model::WriteSection(bStream::CFileStream& stream, std::function<void(bStream::CFileStream&)> writeFunc)
{
	constexpr size_t kSectionSizeOffset{ 4 };

	size_t startingPos = stream.tell();
	writeFunc(stream);
	size_t endingPos = stream.tell();

	stream.seek(startingPos + kSectionSizeOffset);
	stream.writeUInt32(static_cast<uint32_t>(endingPos - startingPos));
	stream.seek(endingPos);
}

void j3d::Model::WriteHeader(bStream::CFileStream& stream)
{
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::J3D2));
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::BMD3));
	stream.writeUInt32(0); // Placeholder for file size.
	stream.writeUInt32(8); // Section count (8 for BMD, 9 for BDL).

	// This is technically a section, but it's the same across all files.
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::SVR3));
	stream.writeUInt32(UINT32_MAX);
	stream.writeUInt32(UINT32_MAX);
	stream.writeUInt32(UINT32_MAX);
}

void j3d::Model::WriteScenegraph(bStream::CFileStream& stream)
{
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::INF1));
	stream.writeUInt32(0);                             // Placeholder for section size.
	stream.writeUInt16(m_matrixType);                  // Flags (but really only holds the matrix type)
	stream.writeUInt16(UINT16_MAX);                    // Padding
	stream.writeUInt32(0);                             // Matrix group count
	stream.writeUInt32(m_vertexData.GetVertexCount()); // Vertex count
	stream.writeUInt32(0x18);                          // Offset to hierarchy data (but it's always 0x18, so...)
}

void j3d::Model::WriteVertexData(bStream::CFileStream& stream)
{
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::VTX1));
	stream.writeUInt32(0);
	stream.writeUInt32(0);
	stream.writeUInt32(0);
}

void j3d::Model::WriteEnvelopeData(bStream::CFileStream& stream)
{
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::EVP1));
	stream.writeUInt32(0);
	stream.writeUInt32(0);
	stream.writeUInt32(0);
}

void j3d::Model::WriteSkinningData(bStream::CFileStream& stream)
{
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::DRW1));
	stream.writeUInt32(0);
	stream.writeUInt32(0);
	stream.writeUInt32(0);
}

void j3d::Model::WriteGeometry(bStream::CFileStream& stream)
{
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::SHP1));
	stream.writeUInt32(0);
	stream.writeUInt32(0);
	stream.writeUInt32(0);
}

void j3d::Model::WriteMaterials(bStream::CFileStream& stream)
{
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::MAT3));
	stream.writeUInt32(0);
	stream.writeUInt32(0);
	stream.writeUInt32(0);
}

void j3d::Model::WriteTextures(bStream::CFileStream& stream)
{
	stream.writeUInt32(static_cast<uint32_t>(EFourCC::TEX1));
	stream.writeUInt32(0);
	stream.writeUInt32(0);
	stream.writeUInt32(0);
}
