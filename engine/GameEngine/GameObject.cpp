#include <algorithm>
#include <exception>
#include <vector>

#include "GameObject.h"
#include "Log.h"
#include "Transform.h"
#include "Game.h"

using namespace std;

void GameObject::BuildCombinedMatrix(Matrix& target)
{
	WorldEntity* we = dynamic_cast<WorldEntity*>(this);

	if (nullptr != we)
	{
		target = target * we->Transform.GetMatrix();
	}

	if (nullptr != Parent)
		Parent->BuildCombinedMatrix(target);
}

void GameObject::DoPreUpdate(const GameTime& time)
{
	if (!Enabled)
		return;

	OnPreUpdate(time);

	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		try
		{
			if (nullptr != *it)
				(*it)->DoPreUpdate(time);
		}
		catch (std::exception ex)
		{
			Log::Error << "Exception! " << ex.what() << endl;

		}
	}

}

void GameObject::DoUpdate(const GameTime& time)
{
	if (!Enabled)
		return;

	OnUpdate(time);

	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		try
		{
			if (nullptr != *it)
				(*it)->DoUpdate(time);
		}
		catch (std::exception ex)
		{
			Log::Error << "Exception! " << ex.what() << endl;
			DEBUG_BREAK;
		}
	}

}

void GameObject::DoPostUpdate(const GameTime& time)
{
	if (!Enabled)
		return;

	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		try
		{
			if (nullptr != *it)
				(*it)->DoPostUpdate(time);

		}
		catch (std::exception ex)
		{
			Log::Error << "Exception! " << ex.what() << endl;
			DEBUG_BREAK;
		}
	}

	OnPostUpdate(time);

}

void GameObject::DoPreRender(const GameTime& time)
{
	if (!Enabled)
		return;

	OnPreRender(time);

	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		if (nullptr != *it)
			(*it)->DoPreRender(time);
	}

}

void GameObject::DoRender(const GameTime& time)
{
	check_gl_error();

	if (!Enabled)
		return;


	auto& env = Game::Instance().Environment();
	auto* we = dynamic_cast<WorldEntity*>(this);

	if (nullptr != we)
	{
		env.PushMatrix(we->Transform.GetMatrix());
	}

	OnRender(time);

	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		if (nullptr != *it)
			(*it)->DoRender(time);
	}

	if (nullptr != we)
	{
		env.PopMatrix();
	}

}

void GameObject::DoPostRender(const GameTime& time)
{
	if (!Enabled)
		return;

	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		if (nullptr != *it)
			(*it)->DoPostRender(time);
	}

	OnPostRender(time);
}




void GameObject::Update(const GameTime& time)
{
	if (!Enabled)
		return;

	// initialize objects that were created in the last frame.
	DoPreUpdate(time);
	DoUpdate(time);
	DoPostUpdate(time);
}

void GameObject::Render(const GameTime& time)
{
	check_gl_error();

	if (!Enabled)
		return;

	DoPreRender(time);
	DoRender(time);
	DoPostRender(time);

	check_gl_error();
}

std::map<GameObject*, int> initCount;

bool GameObject::Initialize()
{
	if (m_isInitialized)
	{
		Log::Warning << "Game is already initialized.";
		return true;
	}

	Log::Info << "Initialize " << Name << endl;

	if (initCount[this]++ > 0)
	{
		Log::Error << "Object initialized multiple times! WTF!\n";
		DEBUG_BREAK;
	}

	bool result = true;

	for (auto it = begin(m_newObjects); it != end(m_newObjects); ++it)
	{
		result &= (**it).Initialize();
	}

	result &= OnInitialize();

	m_isInitialized = result;

	return m_isInitialized;
}


void GameObject::WindowResize(const int width, const int height)
{
	OnWindowResize(width, height);

	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		(**it).WindowResize(width, height);

	}
}

void GameObject::OnWindowResize(const int width, const int height)
{

}

void GameObject::ProcessNewObjects()
{
	if (!m_isInitialized && !Initialize())
	{
		Log::Error << "Initialization failed for " << Name << endl;
	}

	for (auto it : m_newObjects)
	{
		it->ProcessNewObjects();
		m_children.insert(it);
	}

	m_newObjects.clear();

}

int GameObject::CountObjects()
{
	int result = 1;

	for (auto child : m_children)
	{
		result += child->CountObjects();
	}

	return result;
}

void GameObject::Reload(const std::string& tag)
{
	OnReload(tag);

	for (auto child : m_children)
	{
		child->Reload(tag);
	}

}

void GameObject::Reset(const GameTime& time)
{
	OnReset(time);

	for (auto child : m_children)
	{
		child->Reset(time);
	}
}





















