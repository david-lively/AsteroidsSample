#ifndef SPRITE_H
#define SPRITE_H

#include <string>

#include "Drawable.h"

class Sprite : public Drawable
{
public:
	const int MaxLayers = 10;
	int Layer = 0;

	int Width = 0;
	int Height = 0;
	GLuint TextureId = 0;
	bool InScreenSpace = false;

	std::string Filenane;

	Sprite();
	~Sprite();

	Sprite(const std::string& name) : Drawable(name)
	{

	}

	void OnRender(const GameTime& time) override;
	void Load(const std::string& filename, bool invertY = false);

	bool OnInitialize() override;


private:

};

#endif