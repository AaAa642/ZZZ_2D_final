#pragma once

#include "Timer.h"
#include "State_Node.h"


class PlayerIdleNode : public StateNode
{
public:
	PlayerIdleNode() = default;
	~PlayerIdleNode() = default;

	void on_enter() override;
	void on_update(float delta) override;

};


class PlayerRunNode : public StateNode
{
public:
	PlayerRunNode() = default;
	~PlayerRunNode() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;
};


class PlayerRollNode : public StateNode
{
public:
	PlayerRollNode();
	~PlayerRollNode() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	Timer timer;
};


class PlayerNaNode : public StateNode
{
public:
	PlayerNaNode();
	~PlayerNaNode() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

	void update_na_hit_box_position();

private:
	Timer timer;
};


class PlayerSaNode : public StateNode
{
public:
	PlayerSaNode();
	~PlayerSaNode() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

	void update_sa_hit_box_position();

private:
	Timer timer;
};


class PlayerUltNode : public StateNode
{
public:
	PlayerUltNode();
	~PlayerUltNode() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

	void update_ult_hit_box_position();

private:
	Timer timer;

};


class PlayerJumpNode : public StateNode
{
public:
	PlayerJumpNode() = default;
	~PlayerJumpNode() = default;

	void on_enter() override;
	void on_update(float delta) override;

};


class PlayerFallNode : public StateNode
{
public:
	PlayerFallNode() = default;
	~PlayerFallNode() = default;

	void on_enter() override;
	void on_update(float delta) override;

};


class PlayerDeadNode : public StateNode
{
public:
	PlayerDeadNode();
	~PlayerDeadNode() = default;

	void on_enter() override;
	void on_update(float delta) override;

private:
	Timer timer;
};

