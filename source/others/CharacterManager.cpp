 #include "CharacterManager.h"
#include "Player.h"
#include "Enemy.h"

CharacterManager* CharacterManager::manager = nullptr;
CharacterManager* CharacterManager::instance()
{
	if (!manager)
		manager = new CharacterManager();

	return manager;
}

void CharacterManager::on_input(const ExMessage& msg)
{
	player->on_input(msg);
}

void CharacterManager::on_update(float delta)
{
	enemy->on_update(delta);
	player->on_update(delta);
}

void CharacterManager::on_render()
{
	enemy->on_render();
	player->on_render();
}

CharacterManager::CharacterManager()
{
	player = new Player();
	enemy = new Enemy();
}

CharacterManager::~CharacterManager()
{
	delete enemy;
	delete player;
}