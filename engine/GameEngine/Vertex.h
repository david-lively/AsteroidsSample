#ifndef VERTEX_H
#define VERTEX_H

struct Vertex
{
	Vector3 Position;
};


struct VertexPositionNormal
{
	Vector3 Position;
	Vector3 Normal;
};

struct VertexPositionNormalTexture
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoord;
};



#endif