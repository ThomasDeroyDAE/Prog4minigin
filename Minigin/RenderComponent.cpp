#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& filename)
    : Component(owner)
{
    SetTexture(filename);
}

void dae::RenderComponent::Render() const
{
    if (!m_texture || !GetOwner())  
        return;

    const auto& worldPos = GetOwner()->GetTransform().GetWorldPosition();  
    Renderer::GetInstance().RenderTexture(*m_texture, worldPos.x, worldPos.y);
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}
