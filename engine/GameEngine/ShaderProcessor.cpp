#include "ShaderProcessor.h"
#include "Common.h"


#include <string>
#include <vector>
#include <sstream>
#include <regex>

using namespace std;

ShaderProcessor::ShaderProcessor()
{
}


ShaderProcessor::~ShaderProcessor()
{
}


/* world's least efficient way to combine a bunch of strings */
string combine(vector<string>& lines, const string& delimiter)
{

	string result;

	for (auto& line : lines)
	{
		result += line + delimiter;
	}


	return result;
}


string ShaderProcessor::Parse(const std::string& source)
{
	istringstream stream(source);
	string line;
	vector<string> lines;

	//#include ((<[^>]+>)|("[^"]+"))
	regex include("#include ((<[^>]+>)|(\"[^\"]+\"))");
	smatch match;

	while (getline(stream, line))
	{
		if (regex_match(line, match, include))
		{
			Log::Info << "Found #include:\n" << line << endl;
		}
		else
			lines.push_back(line);
	}

	return combine(lines,"\n");
}

