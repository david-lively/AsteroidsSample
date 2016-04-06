#ifndef IDENTITY_H
#define IDENTITY_H

#include <string>

class Identity
{
public:

	Identity(const std::string& name)
	{
		Id = ++m_nextId;
		Name = name + std::to_string(Id);
	}

	Identity() : Identity("identity")
	{
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

