#include <algorithm>
#include <exception>
#include <vector>

#include "GameObject.h"
#include "Log.h"
#include "Transform.h"
#include "Game.h"
#include "Missile.h"

using namespace std;

void GameObject::SetParent(GameObject* newParent)
{
	if (nullptr != Parent)
	{
		std::vector<GameObject*>& pc = Parent->m_children;
		pc.erase(std::remove(begin(pc), end(pc), this), end(pc));
	}

	Parent = newParent;

	if (nullptr != Parent)
		Parent->m_children.push_back(this);
}


void GameObject::BuildCombinedMatrix(Matrix& target)
{
	WorldEntity* we = dynamic_cast<WorldEntity*>(this);

	if (nullptr != we)
	{
		target = target * we->Transform->GetMatrix();
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
	WorldEntity* we = dynamic_cast<WorldEntity*>(this);

	if (nullptr != we)
	{
		env.PushMatrix(we->Transform->GetMatrix());
		//Log::Debug << "Push matrix " << we->Name << " depth " << env.MatrixStackSize() << endl;
	}

	//Log::Debug << "Render " << Name << endl;
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
	Log::Info << "Initialize " << Name << endl;

	if (initCount[this]++ > 0)
	{
		Log::Error << "Object initialized multiple times! WTF!\n";
		DEBUG_BREAK;
	}

	auto result = OnInitialize();
	check_gl_error();
	for (auto it = begin(m_newObjects); it != end(m_newObjects); ++it)
	{
		result &= (*it)->Initialize();
	}

	m_isInitialized = true;

	return result;
}

void GameObject::Dispose()
{
	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		auto refCount = --((*it)->ReferenceCount);
		if (refCount <= 0)
		{
			(*it)->Dispose();
			delete *it;
		}
	}

	OnDispose();
}


void GameObject::WindowResize(int width, int height)
{
	OnWindowResize(width, height);

	for (auto it = begin(m_children); it != end(m_children); ++it)
	{
		(**it).WindowResize(width, height);

	}
}

void GameObject::OnWindowResize(int width, int height)
{

}


void GameObject::PrintHierarchy(int indent)
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
		m_children.push_back(it);
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




















