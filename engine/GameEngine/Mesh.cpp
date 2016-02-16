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

#include "Matrix.h"
#include "Vectors.h"

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
    
    auto& material = *Material;

    /// tell GL which program (shaders) we're using
    gl::UseProgram(material.Program());
    
    SetUniforms(time);
    
    /// bind the vertex and index buffers
    gl::BindBuffer((GLenum)BufferTarget::ElementArrayBuffer, m_indexBuffer);
    gl::BindBuffer((GLenum)BufferTarget::ArrayBuffer, m_vertexBuffer);
    
    /// get the attribute location of Position (vertex) from the compiled shader
    auto location = gl::GetAttribLocation(material.Program(), "Pos");
    
    /// enable position - really useful when we have a lot of vertex attributes and want to disable some of them
    gl::EnableVertexAttribArray(location);
    
    /// Describe the vertex format to GL. This is a 3-component struct with float members (ie, vec3 in GLSL)
    gl::VertexAttribPointer(location, 3, gl::FLOAT, false, 0, nullptr);
    //56 draw it!
    gl::DrawElements((GLenum)Type, (GLuint)m_indexCount, gl::UNSIGNED_SHORT, (GLvoid*)nullptr);
    
    /// unbind the program
    gl::UseProgram(0);
    
    check_gl_error();
}

void Mesh::SetUniforms(const GameTime& time)
{
    SetUniform("GameTimeTotalSeconds",time.TotalSeconds());
    SetUniform("TimeScale", 0.5f);
    
    
    
    auto rotation = Matrix::CreateRotationY(time.TotalSeconds());
    auto translation = Matrix::CreateTranslation(0, 0, -2.0f );
    
    int w;
    int h;
    
    Game::Instance()->GetFramebufferSize(&w, &h);
    
    float aspect = w * 1.f / h;

    auto projection = Matrix::CreatePerspective(45 * 3.14159 / 180, aspect , 1, 1000);
    SetUniform("Projection",projection);
    

    auto world = rotation * translation;
    SetUniform("World",world);
    
    SetUniform("Rotation",rotation);
    

    Vector2 windowSize;
    windowSize.X = w;
    windowSize.Y = h;
    
    SetUniform("WindowSize",windowSize);

}





