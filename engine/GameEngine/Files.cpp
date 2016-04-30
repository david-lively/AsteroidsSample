#include "Files.h"

#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <vector>

using namespace std;


/// <summary>
/// Reads the specified text file.
/// </summary>
/// <param name="path">path to the file</param>
/// <returns></returns>
string Files::Read(const string& path)
{
	std::ifstream t(path);
	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign(
		std::istreambuf_iterator<char>(t)
		,
		std::istreambuf_iterator<char>()
		);

	return str;
}


/// <summary>
/// Determines if the file exists
/// </summary>
/// <param name="path">path to the file</param>
/// <returns></returns>
bool Files::Exists(const string& path)
{
	ifstream ifile(path.c_str());

	return ifile.is_open();
}

vector<string> Files::ReadLines(const std::string& path)
{
	/*
	std::ifstream file("filename");
	std::string line;
	while (std::getline(file, line)) {
		// whatever
	}
	*/
	vector<string> lines;
	
	ifstream file(path);
	string line;

	while (getline(file, line))
	{
		lines.push_back(line);
	}

	Log::Info << "Read " << lines.size() << " lines from \"" << path << "\"" << endl;

	return lines;
}




