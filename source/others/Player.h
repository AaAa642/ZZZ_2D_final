#pragma once

#include "Character.h"

class Player :public Character
{
public:
	enum class AttackDir
	{
		Left,Right
	};

	enum class AttackWay
	{
		melee,ranged
	};
public:
	Player();
	~Player();


	void on_input(const ExMessage& msg) override;
	void on_update(float delta) override;
	void on_render() override;

	void on_hurt() override;

	void on_roll();
	void on_jump();
	void on_land();
	void on_na();
	void on_sa();
	void on_ult();
	void mode_change();

public:
	void set_rolling(bool flag)
	{
		is_rolling = flag;
	}

	bool get_rolling() const
	{
		return is_rolling;
	}

	bool can_roll() const
	{
		return is_roll_cd_comp && !is_rolling && is_roll_key_down;
	}

	
	bool can_jump() const
	{
		return is_on_floor() && is_jump_key_down 
			&& !is_rolling;
	}

	void set_attacking(bool flag)
	{
		is_attacking = flag;
	}

	bool get_attacking() const
	{
		return is_attacking;
	}
	
	void set_na(bool flag)
	{
		is_na = flag;
	}

	bool get_na() const
	{
		return is_na;
	}

	void set_sa(bool flag)
	{
		is_sa = flag;
	}

	bool get_sa() const
	{
		return is_sa;
	}

	void set_ult(bool flag)
	{
		is_ult = flag;
	}

	bool get_ult() const
	{
		return is_ult;
	}

	bool is_energy_enough() const
	{
		return (energy - 100 >= 0) ? 1 : 0;
	}

	bool is_conerto_enough() const
	{
		return (concerto - 3000 >= 0) ? 1 : 0;
	}

	void add_energy(int enegy)
	{
		this->energy += enegy;
		if (this->energy >= 300)
			this->energy = 300;
	}

	void add_concerto(int concerto)
	{
		this->concerto += concerto;
		if (this->concerto >= 3000)
			this->concerto = 3000;
	}

	bool can_na() const
	{
		return is_attacking_cd_comp && !is_attacking && is_na_key_down
			&& !is_rolling;// && is_on_floor();
	}

	bool can_sa() const
	{
		return is_attacking_cd_comp && !is_attacking && is_sa_key_down && is_energy_enough()
			&& !is_rolling && is_on_floor();
	}

	bool can_ult() const
	{
		return is_attacking_cd_comp && !is_attacking && is_ult_key_down && is_conerto_enough()
			&& !is_rolling && is_on_floor();
	}

	bool can_mode() const
	{
		return is_modechanging_cd_comp && is_mode_key_down 
			&& !is_attacking;
	}

	int get_move_aixs() const
	{
		return is_right_key_down - is_left_key_down;
	}

	AttackDir get_attcack_dir() const
	{
		return is_facing_left ? AttackDir::Left : AttackDir::Right;
	}

	AttackWay get_attack_way() const
	{
		return attack_way;
	}

	void reset_attack_vfx()
	{
		current_attack_animation->reset();
	}

private:
	Timer timer_roll_cd;
	bool is_rolling = false;
	bool is_roll_cd_comp = true;

	Timer timer_na_cd;
	bool is_attacking = false;
	bool is_attacking_cd_comp = true;
	bool is_na = false;

	Timer timer_Mode_cd;
	bool is_modechanging_cd_comp = true;
	bool mode_changing = false;

	bool is_sa = false;
	int energy = 0;

	bool is_ult = false;
	int concerto = 0;
	

	bool is_left_key_down = false;
	bool is_right_key_down = false;
	bool is_jump_key_down = false;
	bool is_roll_key_down = false;
	bool is_na_key_down = false;
	bool is_sa_key_down = false;
	bool is_ult_key_down = false;
	bool is_mode_key_down = false;

	Animation animation_na_melee_left;
	Animation animation_na_melee_right;
	Animation animation_na_ranged_left;
	Animation animation_na_ranged_right;
	Animation animation_sa_left;
	Animation animation_sa_right;
	Animation animation_ult_left;
	Animation animation_ult_right;
	AttackDir attack_dir = AttackDir::Right;
	AttackWay attack_way = AttackWay::melee;
	Animation* current_attack_animation = nullptr;

	const float CD_ROLL = 0.75f;
	const float CD_NA = 0.6f;
	const float CD_MODE = 2.0f;
	const float SPEED_RUN = 300.0f;
	const float SPEED_JUMP = 1000.0f;
	const float SPEED_ROLL = 800.0f;

};