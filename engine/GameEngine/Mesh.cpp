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

#include <iostream>

using namespace std;


bool Mesh::OnInitialize()
{
    Log::Info << "Mesh::OnInitialize\n";
    
    /// initialize assets for OpenGL
    
    return true;
}


void Mesh::OnRender(const GameTime& time)
{
    check_gl_error();

    gl::BindVertexArray(m_vao);

    Material->Bind();

	Material->SetUniforms(time);
    
    gl::PolygonMode(gl::FRONT_AND_BACK, (GLenum)Material->FillType);
    
    gl::Enable(gl::CULL_FACE);
    gl::FrontFace(gl::CW);

    if (m_indexBuffer > 0)
        gl::DrawElements((GLenum)Type, (GLuint)m_indexCount, gl::UNSIGNED_SHORT, (GLvoid*)nullptr);
    else
        gl::DrawArrays((GLenum)Type, (GLuint)0, (GLsizei)m_vertexCount);
        
        
    /// unbind the program
    gl::UseProgram(0);
    
    check_gl_error();
}




