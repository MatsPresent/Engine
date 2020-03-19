#include "Multiverse.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextObject.h"
#include "Transform.h"

#include <memory>
#include <iostream>

int main(int, char**)
{
	mv::Multiverse::init();

	auto& scene = mv::SceneManager::instance().CreateScene("Demo");

	auto go = std::make_shared<mv::GameObject>();
	go->SetTexture("background.jpg");
	scene.Add(go);

	go = std::make_shared<mv::GameObject>();
	go->SetTexture("logo.png");
	go->SetPosition(216, 180);
	scene.Add(go);
	mv::Multiverse::run();

	mv::Multiverse::cleanup();
	return 0;
}
