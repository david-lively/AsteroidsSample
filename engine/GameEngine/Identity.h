#ifndef IDENTITY_H
#define IDENTITY_H

#include <string>

class Identity
{
public:

	Identity(const std::string& name) : Identity()
	{
		Name = name + "." + std::to_string(Id);
	}

	Identity()
	{
		Id = ++m_nextId;
		Name = "object." + std::to_string(Id);
	}

	~Identity()
	{

	}

	std::string Name;
	int Id;

private:
	static int m_nextId;
};

#endif

