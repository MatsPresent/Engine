#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

mv::GameObject::~GameObject() = default;

void mv::GameObject::Update(){}

void mv::GameObject::Render() const
{
	const auto pos = m_Transform.GetPosition();
	Renderer::instance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void mv::GameObject::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::instance().LoadTexture(filename);
}

void mv::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
