#include <string>
#include <ctype.h>
#include <vector>

using namespace std;


#include "Parser.h"
#include "Log.h"


void Parser::Parse(const std::string& text)
{
	m_currentTokenIndex = 0;
	m_tokens.clear();

	Tokenize(text);
}


void Parser::Tokenize(const std::string& text)
{
	m_tokens.clear();

	string current;

	int i = 0;

	for (int i = 0; i < text.length(); ++i)
	while (i < text.size())
	{
		char c = text[i];
		
		if (!isspace(c))
		{
			current += c;
		}
		else if (current.length() > 0)
		{
			m_tokens.push_back(current);
			current.clear();
		}
	
		++i;
	}
}


bool Parser::Eof()
{
	return m_currentTokenIndex < m_tokens.size();
}

const string Parser::Next()
{
	if (Eof())
	{
		Log::Error << "Attempt to read past end of buffer\n";
		throw;
	}

	
	string result = m_tokens[m_currentTokenIndex];

	++m_currentTokenIndex;

	return result;
}


