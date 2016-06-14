#include "ShaderProcessor.h"
#include "Common.h"


#include <string>
#include <vector>
#include <sstream>
#include <regex>

using namespace std;

#include "Files.h"

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

	/// reserve an arbitrary amount of space for the string
	result.reserve(lines.size() * 255);

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

	string pattern = "\\s*#\\s*include\\s*[<\"]([^>^\"]+)[>\"]";
	regex include(pattern);

	smatch matches;

	while (getline(stream, line))
	{
		if (regex_match(line, matches, include))
		{
			if (matches.size() < 2)
			{
				Log::Error << "#include directive does specify a filename\n";
				continue;
			}
			else
			{
				string filename = matches[1];

				if (!Files::Exists(filename))
				{
					Log::Warning << "#include references missing file \"" << filename << "\"." << endl;
					continue;
				}

				Log::Info << "Parsing #include file " << filename << endl;

				auto content = Files::Read(filename);


				lines.push_back(Parse(content));
			}
		
		}
		else
			lines.push_back(line);
	}

	return combine(lines,"\n");
}

