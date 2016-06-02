#ifndef PREPROCESSOR_H
#define	PREPROCESSOR_H

#include <string>
#include <map>

#include "Shader.h"

namespace Parsing
{
	/*
	Preprocess # macros in shader
	*/
	class Preprocessor
	{
	public:
		Preprocessor();
		virtual ~Preprocessor() = 0;

		// process any # macros
		static std::string Process(const std::string& text);

		// split a unified shader source file into separate shaders
		void Split(ShaderType, const std::string& text, std::map<ShaderType, std::string>& shaders);
	};
}

#endif

