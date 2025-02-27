#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotateComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "Scene.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto to = std::make_shared<dae::GameObject>();
	to->SetPosition(180, 20);
	to->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	scene.Add(to);

	auto fpsCounter = std::make_shared<dae::GameObject>();
	fpsCounter->SetPosition(20, 20);
	fpsCounter->AddComponent<dae::FPSComponent>(font);
	scene.Add(fpsCounter);

	auto rotating = std::make_shared<dae::GameObject>();
	rotating->AddComponent<dae::RenderComponent>("pacman.tga");
	rotating->SetPosition(200, 300);
	rotating->AddComponent<dae::RotateComponent>(270.0f, false);
	scene.Add(rotating);

	auto rotating2 = std::make_shared<dae::GameObject>();
	rotating2->AddComponent<dae::RenderComponent>("mspacman.tga");
	rotating2->SetParent(rotating, false);
	rotating2->AddComponent<dae::RotateComponent>(360.0f, true); 
	scene.Add(rotating2);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}