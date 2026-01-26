#pragma once

#include "types.h"

#include "geometry/shape.h"
#include "geometry/vertexdata.h"

#include <functional>

struct ufbx_scene;
struct ufbx_skin_cluster;

namespace j3d
{
	class Model
	{
	public:
		Model(ufbx_scene* fbxScene);

		int32_t Export();

	private:
		static void WriteSection(bStream::CFileStream& stream, std::function<void(bStream::CFileStream&)> writeFunc);

		void LoadGeometry(ufbx_scene* fbxScene);

		void WriteHeader(bStream::CFileStream& stream);
		void WriteScenegraph(bStream::CFileStream& stream);
		void WriteVertexData(bStream::CFileStream& stream);
		void WriteEnvelopeData(bStream::CFileStream& stream);
		void WriteSkinningData(bStream::CFileStream& stream);
		void WriteGeometry(bStream::CFileStream& stream);
		void WriteMaterials(bStream::CFileStream& stream);
		void WriteTextures(bStream::CFileStream& stream);

		uint8_t m_matrixType;

		VertexData m_vertexData;
		std::vector<Shape> m_shapes;
		std::vector<ufbx_skin_cluster*> m_joints;
	};
} // namespace j3d
