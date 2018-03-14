#include "Preprocessor.h"
#include "Shader.h"

using namespace Parsing;

#include <string>
#include <map>

using namespace std;

Preprocessor::Preprocessor()
{
}


Preprocessor::~Preprocessor()
{
}

string Preprocessor::Process(const string& text)
{
    string result;
    result.resize(text.length()* 2);

	for (auto it = begin(text); it != end(text); ++it)
	{
		if ('#' == *it)
		{

		}
		else
		{
			while ('\n' != *it)
			{

			}
		}

	}
    
    throw;
}










