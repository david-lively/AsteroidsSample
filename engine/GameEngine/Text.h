#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>
#include <queue>


#include "Common.h"
#include "Drawable.h"
#include "Vectors.h"

class Text : public Drawable
{
public:
	GLuint FontTextureId = 0;
	Vector2 TextureSize;
	Vector2 CellSize;
	Vector2 RenderPosition;
	float FontSize = 100.f;


	//std::queue<std::string> Strings;
	std::string Data;

	Text(const std::string& name) : Drawable(name)
	{
		Data = Name;
	}

	bool OnInitialize() override;
	void OnRender(const GameTime& time) override;
	void OnUpdate(const GameTime& time) override;

private:
	int m_fontTextureWidth = 0;
	int m_fontTextureHeight = 0;

};

#endif