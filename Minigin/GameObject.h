#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <algorithm>
#include "Transform.h"
#include "Component.h"
#include "ResourceManager.h"
#include "Texture2D.h"

namespace dae
{
    class GameObject final
    {
    public:
        GameObject() = default;
        ~GameObject()
        {
            for (auto* child : m_Children)
            {
                child->m_Parent = nullptr;
            }
            m_Children.clear();
            SetParent(nullptr, false);
        }


        void Update(float deltaTime);
        void Render() const;
        void FixedUpdate();

        void SetPosition(float x, float y, float z = 0.0f)
        {
            m_transform.SetLocalPosition({ x, y, z });
            m_transform.SetPositionDirty();
        }

        Transform& GetTransform() { return m_transform; }
        const Transform& GetTransform() const { return m_transform; }

        // COMPONENTS

        template <typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            T* ptr = component.get();

            m_ComponentMap[std::type_index(typeid(T))] = ptr;
            m_Components.push_back(std::move(component));

            return ptr;
        }

        template <typename T>
        T* GetComponent() const
        {
            auto it = m_ComponentMap.find(std::type_index(typeid(T)));
            return (it != m_ComponentMap.end()) ? static_cast<T*>(it->second) : nullptr;
        }

        template <typename T>
        void RemoveComponent()
        {
            auto typeIndex = std::type_index(typeid(T));
            auto it = m_ComponentMap.find(typeIndex);
            if (it != m_ComponentMap.end())
            {
                m_ComponentMap.erase(it);
                m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(),
                    [](const std::unique_ptr<Component>& component) {
                        return dynamic_cast<T*>(component.get()) != nullptr;
                    }), m_Components.end());
            }
        }

        // PARENT-CHILD RELATIONSHIP

        void SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition)
        {
            if (parent.get() == this || m_Parent == parent.get() || IsChildOf(parent.get()))
                return;

            glm::vec3 worldPos = m_transform.GetWorldPosition();

            if (m_Parent)
            {
                auto& siblings = m_Parent->m_Children;
                siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
            }

            m_Parent = parent.get();

            if (m_Parent)
            {
                m_Parent->m_Children.push_back(this);

                if (keepWorldPosition)
                    m_transform.SetLocalPosition(m_Parent->GetTransform().GetWorldPosition());
            }
            else m_transform.SetLocalPosition(worldPos);

            m_transform.SetPositionDirty();
        }

        bool IsChildOf(const GameObject* potentialChild) const
        {
            if (m_Parent == nullptr) return false;
            for (const GameObject* parent = m_Parent; parent != nullptr; parent = parent->m_Parent)
            {
                if (parent == potentialChild) return true;
            }
            return false;
        }


        GameObject* GetParent() const { return m_Parent; }
        const std::vector<GameObject*>& GetChildren() const { return m_Children; }

        GameObject* GetChildAt(size_t index) const
        {
            if (index >= m_Children.size()) throw std::out_of_range("Child index out of bounds");
            return m_Children[index];
        }

        size_t GetChildCount() const { return m_Children.size(); }

    private:
        Transform m_transform{};
        std::vector<std::unique_ptr<Component>> m_Components{};
        std::unordered_map<std::type_index, Component*> m_ComponentMap{};

        GameObject* m_Parent{ nullptr };
        std::vector<GameObject*> m_Children{};
    };
}
