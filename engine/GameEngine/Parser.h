#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Parser
{
public:
	void Parse(const std::string& text);
	
	// true if at the end of the stream.
	bool Eof();

	// return the next token
	const std::string Next();

	void Tokenize(const std::string& text);

private:
	std::vector<std::string> m_tokens;
	int m_currentTokenIndex;


};


#endif