#ifndef MODELSTORE_H
#define MODELSTORE_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Common.h"
#include "Mesh.h"
#include "GameObject.h"

class ModelStore
{
public:
	std::map<std::string, const Mesh*> Meshes;

	const bool Exists(const std::string& name) const
	{
		return Meshes.count(name) > 0;
	}

	const Mesh* Get(const std::string& name)
	{
		if (!Exists(name))
		{
			Log::Error << "ModelStore - mesh \"" << name << "\" does not exist." << std::endl;
			throw;
		}

		return Meshes[name];
	}

	void Add(const std::string& name, const Mesh* mesh)
	{
		if (Exists(name))
		{
			Log::Warning << "ModelStore - a mesh named \"" << name << "\" already exists." << std::endl;
			return;
		}
		else
			Meshes[name] = mesh;


	}


private:
};


#endif