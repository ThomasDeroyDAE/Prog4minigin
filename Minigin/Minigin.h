#pragma once
#include <string>
#include <functional>

namespace dae
{
    class Minigin
    {
    public:
        explicit Minigin(const std::string& dataPath);
        ~Minigin();
        void Run(const std::function<void()>& load);

        Minigin(const Minigin&) = delete;
        Minigin(Minigin&&) = delete;
        Minigin& operator=(const Minigin&) = delete;
        Minigin& operator=(Minigin&&) = delete;

        float GetFPS() const { return m_FPS; }

    private:
        float m_FPS{ 60.0f };
    };
}