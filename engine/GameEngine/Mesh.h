//
//  Mesh.hpp
//  Assignment2
//
//  Created by David Lively on 2/1/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <functional>
#include <map>

#include "GameObject.h"
#include "Enums.h"
#include "Material.h"
#include "Vertex.h"

class Mesh : public GameObject
{
public:
	Mesh(const std::string& name) : GameObject(name)
	{

	}

	Mesh() : Mesh("unnamed-mesh")
	{

	}

	void Bind()
	{
		gl::BindVertexArray(m_vao);
	}

	void Unbind()
	{
		gl::BindVertexArray(0);
	}

	Material* Material = nullptr;

	BeginMode Type = BeginMode::Triangles;
	bool CullBackfaces = true;
	//SenderNotifyEvent OnPreDraw = nullptr;

	GLuint VertexBuffer() { return m_vertexBuffer; }
	bool IsDynamic = false;

	template<typename TCollection>
	void SetVertexData(const TCollection& vertices, const BufferUsageHint hint = BufferUsageHint::StaticDraw)
	{
		// NOTE: Associated VAO may have to be  bound for this - not sure. 
		gl::BindBuffer((GLenum)BufferTarget::ArrayBuffer, m_vertexBuffer);

		m_vertexCount = vertices.size();

		size_t size = m_vertexCount * sizeof(vertices[0]);

		gl::BufferData((GLenum)BufferTarget::ArrayBuffer, size, vertices.data(), (GLenum)hint);

		check_gl_error();

	}

	/// initialize a non-indexed mesh (no index buffer, vertex buffer only)
	template<typename TVertexCollection>
	void Initialize(const TVertexCollection& vertices)
	{
		check_gl_error();

		gl::GenVertexArrays(1, &m_vao);
		gl::BindVertexArray(m_vao);

		check_gl_error();

		/// vertex buffer
		gl::GenBuffers(1, &m_vertexBuffer);

		SetVertexData(vertices);

		Material->Bind();


		// bind some attributes
		{
			auto location = Material->GetAttribLocation("Position");

			if (location >= 0)
			{
				gl::VertexAttribPointer(location, 3, gl::FLOAT, false, 0, nullptr);
				gl::EnableVertexAttribArray(location);
			}
		}

		{
			auto location = Material->GetAttribLocation("Character");

			if (location >= 0)
			{
				gl::VertexAttribIPointer(location, 1, gl::BYTE, 0, nullptr);
				gl::EnableVertexAttribArray(location);
			}

		}

		check_gl_error();

		gl::BindVertexArray(0);

	}

	template<typename TVertexCollection, typename TIndexCollection>
	void Initialize(TVertexCollection& vertices, TIndexCollection& indices)
	{

		/// make sure there aren't any pending OpenGL errors
		check_gl_error();

		gl::GenVertexArrays(1, &m_vao);
		gl::BindVertexArray(m_vao);
		check_gl_error();

		/// create a GL buffer for the vertices
		gl::GenBuffers(1, &m_vertexBuffer);
		/// now, generate and populate the index buffer
		gl::GenBuffers(1, &m_indexBuffer);

		/// bind the buffer, so that subsequent buffer operations affect this object.
		gl::BindBuffer((GLenum)BufferTarget::ArrayBuffer, m_vertexBuffer);
		/// make the new buffer active
		gl::BindBuffer((GLenum)BufferTarget::ElementArrayBuffer, m_indexBuffer);

		if (!IsDynamic)
		{
			/// number of bytes that our vertex collection occupies
			size_t vertexBufferSize = vertices.size() * sizeof(vertices[0]);
			m_vertexCount = vertices.size();

			/// copy the buffer data to the GPU's memory
			gl::BufferData(
				/// what kind of buffer is this - ArrayBuffer, Element Array Buffer
				(GLenum)BufferTarget::ArrayBuffer
				/// number of bytes we're sending to OpenGL
				, vertexBufferSize
				/// Host pointer to data to copy
				, vertices.data()
				/// how this buffer will be used - see OpenGL docs
				, (GLenum)BufferUsageHint::StaticDraw
				);




			m_indexCount = indices.size();
			size_t indexBufferSize = indices.size() * sizeof(indices[0]);
			gl::BufferData(
				/// buffer type - ElementArrayBuffer is an index buffer
				(GLenum)BufferTarget::ElementArrayBuffer
				, indexBufferSize
				, indices.data()
				, (GLenum)BufferUsageHint::StaticDraw
				);
		}

		/// get the attribute location of Position (vertex) from the compiled shader
		auto location = Material->GetAttribLocation("Position");

		/// enable position - really useful when we have a lot of vertex attributes and want to disable some of them
		gl::EnableVertexAttribArray(location);

		/// Describe the vertex format to GL. This is a 3-component struct with float members (ie, vec3 in GLSL)
		gl::VertexAttribPointer(location, 3, gl::FLOAT, false, 0, nullptr);


		/// make sure there aren't any pending OpenGL errors
		check_gl_error();

		gl::BindVertexArray(0);

	}

	void Initialize(const std::vector<Vector3>& vertices, const std::vector<Vector4>& colors, const std::vector<GLushort>& indices);

	void OnRender(const GameTime& time) override;

private:
	size_t m_vertexCount = 0;;
	GLuint m_vertexBuffer = 0;

	size_t m_indexCount = 0;
	GLuint m_indexBuffer = 0;

	GLuint m_vao = 0;

};



#endif /* Mesh_hpp */
