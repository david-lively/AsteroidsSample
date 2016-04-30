//
//  Mesh.cpp
//  Assignment2
//
//  Created by David Lively on 2/1/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Mesh.h"
#include "Common.h"
#include "Log.h"
#include "Game.h"

#include <iostream>

using namespace std;


void Mesh::OnRender(const GameTime& time)
{
    check_gl_error();


	if (this->Material->Blend)
	{
		gl::Enable(gl::BLEND);
		gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
	}
	else
		gl::Disable(gl::BLEND);



    gl::BindVertexArray(m_vao);

	check_gl_error();

 //   Material->Bind();
	//check_gl_error();

	auto& env = Game::Instance().Environment();

	if (env.ForceWireframe)
		gl::PolygonMode(gl::FRONT_AND_BACK, (GLenum)PolygonMode::Line);
	else
		gl::PolygonMode(gl::FRONT_AND_BACK, (GLenum)Material->FillType);

	if (Material->DepthTesting)
		gl::Enable(gl::DEPTH_TEST);
	else
		gl::Disable(gl::DEPTH_TEST);

	if (CullBackfaces)
		gl::Enable(gl::CULL_FACE);
	else
		gl::Disable(gl::CULL_FACE);

    gl::FrontFace(gl::CW);
	
	check_gl_error();

    if (m_indexBuffer > 0)
        gl::DrawElements((GLenum)Type, (GLuint)m_indexCount, gl::UNSIGNED_SHORT, (GLvoid*)nullptr);
    else
        gl::DrawArrays((GLenum)Type, (GLuint)0, (GLsizei)m_vertexCount);
        
	check_gl_error();

    /// unbind the program
    gl::UseProgram(0);
    
    check_gl_error();
}

void Initialize(const std::vector<Vector3>& vertices, const std::vector<Vector4>& colors, const std::vector<GLushort>& indices);





