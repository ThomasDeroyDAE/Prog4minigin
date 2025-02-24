#include "GameObject.h"
#include "Component.h"
#include "RenderComponent.h"

void dae::GameObject::Update(float deltaTime)
{
    for (const auto& component : m_Components)
    {
        component->Update(deltaTime);
    }
}

void dae::GameObject::FixedUpdate()
{
    for (const auto& component : m_Components)
    {
        component->FixedUpdate();
    }
}

void dae::GameObject::Render() const
{
    for (const auto& component : m_Components)
    {
        component->Render();
    }
}




