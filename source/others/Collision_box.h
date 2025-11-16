#pragma once

#include "Vector2.h"
#include "Colllision_layer.h"

#include <functional>

class CollisionBox
{
	friend class CollisionManager;
	friend class PlayerNaNode;
	friend class PlayerSaNode;
	friend class PlayerUltNode;
	friend class Character;
	friend class Player;
	friend class Enemy;
	friend class Barb;
	friend class Sword;

public:
	void set_enabled(bool flag)
	{
		enabled = flag;
	}

	void set_layer_src(Collisionlayer layer)
	{
		layer_src = layer;
	}

	void set_layer_dst(Collisionlayer layer)
	{
		layer_dst = layer;
	}

	void set_on_collide(std::function<void()> on_colide)
	{
		this->on_collide = on_collide;
	}

	void set_size(const Vector2& size)
	{
		this->size = size;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	

private:
	Vector2 size;
	Vector2 position;
	bool enabled = true;
	std::function<void()> on_collide;
	Collisionlayer layer_src = Collisionlayer::None;
	Collisionlayer layer_dst = Collisionlayer::None;

private:
	CollisionBox() = default;
	~CollisionBox() = default;

};