#ifndef SHADERPROCESSOR_H
#define SHADERPROCESSOR_H

#include <string>
#include <vector>

class ShaderProcessor
{
public:
	ShaderProcessor();
	~ShaderProcessor();

	// parse a  source file and return a vector of the source and any include files.
	static std::string Parse(const std::string& source);
};

#endif