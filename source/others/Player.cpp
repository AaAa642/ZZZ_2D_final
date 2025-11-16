#include "Player.h"
#include "ResourcesManager.h"
#include "Player_State_Nodes.h"

#include <cmath>


Player::Player()
{
	hp = 10;

	is_facing_left = false;
	position = { 250,200 };
	logic_height = 120;

	hit_box->size = { 150,150 };
	hurt_box->size = { 40,80 };

	hit_box->layer_src = Collisionlayer::None;
	hit_box->layer_dst = Collisionlayer::Enemy;

	hurt_box->layer_src = Collisionlayer::Player;
	hurt_box->layer_dst = Collisionlayer::None;

	hit_box->enabled = false;
	hurt_box->on_collide = [&]()
		{
			decrease_hp();
		};


	timer_roll_cd.set_wait_time(CD_ROLL);
	timer_roll_cd.set_one_shot(true);
	timer_roll_cd.set_on_timeout([&]()
		{
			is_roll_cd_comp = true;
		});

	timer_na_cd.set_wait_time(CD_NA);
	timer_na_cd.set_one_shot(true);
	timer_na_cd.set_on_timeout([&]()
		{
			is_attacking_cd_comp = true;
		});

	timer_Mode_cd.set_wait_time(CD_MODE);
	timer_Mode_cd.set_one_shot(true);
	timer_Mode_cd.set_on_timeout([&]()
		{
			is_modechanging_cd_comp = true;
		});

	//{资源加载代码}
	{
		AnimationGroup& animation_idle = animation_pool["player_idle"];

		Animation& animation_idle_left = animation_idle.left;
		animation_idle_left.set_interval(0.075f);
		animation_idle_left.set_loop(true);
		animation_idle_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_idle_left.add_frame(ResourcesManager::instance()->find_atlas("player_idle_left"));

		Animation& animation_idle_right = animation_idle.right;
		animation_idle_right.set_interval(0.075f);
		animation_idle_right.set_loop(true);
		animation_idle_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_idle_right.add_frame(ResourcesManager::instance()->find_atlas("player_idle_right"));
	}
	{
		AnimationGroup& animation_run = animation_pool["player_run"];

		Animation& animation_run_left = animation_run.left;
		animation_run_left.set_interval(0.075f);
		animation_run_left.set_loop(true);
		animation_run_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_run_left.add_frame(ResourcesManager::instance()->find_atlas("player_run_left"));

		Animation& animation_run_right = animation_run.right;
		animation_run_right.set_interval(0.075f);
		animation_run_right.set_loop(true);
		animation_run_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_run_right.add_frame(ResourcesManager::instance()->find_atlas("player_run_right"));
	}
	{
		AnimationGroup& animation_roll = animation_pool["player_roll"];

		Animation& animation_roll_left = animation_roll.left;
		animation_roll_left.set_interval(0.05f);
		animation_roll_left.set_loop(false);
		animation_roll_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_roll_left.add_frame(ResourcesManager::instance()->find_atlas("player_roll_left"));

		Animation& animation_roll_right = animation_roll.right;
		animation_roll_right.set_interval(0.05f);
		animation_roll_right.set_loop(false);
		animation_roll_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_roll_right.add_frame(ResourcesManager::instance()->find_atlas("player_roll_right"));
	}
	{
		AnimationGroup& animation_na_melee = animation_pool["player_na_melee"];

		Animation& animation_na_melee_left = animation_na_melee.left;
		animation_na_melee_left.set_interval(0.05f);
		animation_na_melee_left.set_loop(false);
		animation_na_melee_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_na_melee_left.add_frame(ResourcesManager::instance()->find_atlas("player_na_melee_left"));

		Animation& animation_na_melee_right = animation_na_melee.right;
		animation_na_melee_right.set_interval(0.05f);
		animation_na_melee_right.set_loop(false);
		animation_na_melee_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_na_melee_right.add_frame(ResourcesManager::instance()->find_atlas("player_na_melee_right"));
	}
	{
		AnimationGroup& animation_na = animation_pool["player_na_ranged"];

		Animation& animation_na_left = animation_na.left;
		animation_na_left.set_interval(0.05f);
		animation_na_left.set_loop(false);
		animation_na_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_na_left.add_frame(ResourcesManager::instance()->find_atlas("player_na_ranged_left"));

		Animation& animation_na_right = animation_na.right;
		animation_na_right.set_interval(0.05f);
		animation_na_right.set_loop(false);
		animation_na_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_na_right.add_frame(ResourcesManager::instance()->find_atlas("player_na_ranged_right"));
	}
	{
		AnimationGroup& animation_sa = animation_pool["player_sa"];

		Animation& animation_sa_left = animation_sa.left;
		animation_sa_left.set_interval(0.05f);
		animation_sa_left.set_loop(false);
		animation_sa_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_sa_left.add_frame(ResourcesManager::instance()->find_atlas("player_sa_left"));

		Animation& animation_sa_right = animation_sa.right;
		animation_sa_right.set_interval(0.05f);
		animation_sa_right.set_loop(false);
		animation_sa_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_sa_right.add_frame(ResourcesManager::instance()->find_atlas("player_sa_right"));
	}
	{
		AnimationGroup& animation_ult = animation_pool["player_ult"];

		Animation& animation_ult_left = animation_ult.left;
		animation_ult_left.set_interval(0.05f);
		animation_ult_left.set_loop(false);
		animation_ult_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_ult_left.add_frame(ResourcesManager::instance()->find_atlas("player_ult_left"));

		Animation& animation_ult_right = animation_ult.right;
		animation_ult_right.set_interval(0.05f);
		animation_ult_right.set_loop(false);
		animation_ult_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_ult_right.add_frame(ResourcesManager::instance()->find_atlas("player_ult_right"));
	}
	{
		AnimationGroup& animation_jump = animation_pool["player_jump"];

		Animation& animation_jump_left = animation_jump.left;
		animation_jump_left.set_interval(0.075f);
		animation_jump_left.set_loop(true);
		animation_jump_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_jump_left.add_frame(ResourcesManager::instance()->find_atlas("player_jump_left"));

		Animation& animation_jump_right = animation_jump.right;
		animation_jump_right.set_interval(0.075f);
		animation_jump_right.set_loop(true);
		animation_jump_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_jump_right.add_frame(ResourcesManager::instance()->find_atlas("player_jump_right"));
	}
	{
		AnimationGroup& animation_fall = animation_pool["player_fall"];

		Animation& animation_fall_left = animation_fall.left;
		animation_fall_left.set_interval(0.15f);
		animation_fall_left.set_loop(true);
		animation_fall_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_fall_left.add_frame(ResourcesManager::instance()->find_atlas("player_fall_left"));

		Animation& animation_fall_right = animation_fall.right;
		animation_fall_right.set_interval(0.075f);
		animation_fall_right.set_loop(true);
		animation_fall_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_fall_right.add_frame(ResourcesManager::instance()->find_atlas("player_fall_right"));
	}
	{
		AnimationGroup& animation_dead = animation_pool["player_dead"];

		Animation& animation_dead_left = animation_dead.left;
		animation_dead_left.set_interval(0.075f);
		animation_dead_left.set_loop(false);
		animation_dead_left.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_dead_left.add_frame(ResourcesManager::instance()->find_atlas("player_dead_left"));

		Animation& animation_dead_right = animation_dead.right;
		animation_dead_right.set_interval(0.075f);
		animation_dead_right.set_loop(false);
		animation_dead_right.set_anchor_mode(Animation::anchorMode::bottomed);
		animation_dead_right.add_frame(ResourcesManager::instance()->find_atlas("player_dead_right"));
	}
	//player_{run fall jump roll idle dead na sa ult}
	//{特效加载代码}
	// {na(melee,ranged) sa ult}
	{
		animation_na_melee_left.set_interval(0.1f);
		animation_na_melee_left.set_loop(false);
		animation_na_melee_left.set_anchor_mode(Animation::anchorMode::centered);
		animation_na_melee_left.add_frame(ResourcesManager::instance()->find_atlas("player_vfx_na_melee_left"));
	}
	{
		animation_na_melee_right.set_interval(0.1f);
		animation_na_melee_right.set_loop(false);
		animation_na_melee_right.set_anchor_mode(Animation::anchorMode::centered);
		animation_na_melee_right.add_frame(ResourcesManager::instance()->find_atlas("player_vfx_na_melee_right"));
	}
	{
		animation_na_ranged_left.set_interval(0.1f);
		animation_na_ranged_left.set_loop(false);
		animation_na_ranged_left.set_anchor_mode(Animation::anchorMode::centered);
		animation_na_ranged_left.add_frame(ResourcesManager::instance()->find_atlas("player_vfx_na_ranged_left"));
	}
	{
		animation_na_ranged_right.set_interval(0.1f);
		animation_na_ranged_right.set_loop(false);
		animation_na_ranged_right.set_anchor_mode(Animation::anchorMode::centered);
		animation_na_ranged_right.add_frame(ResourcesManager::instance()->find_atlas("player_vfx_na_ranged_right"));
	}
	{
		animation_sa_left.set_interval(0.3f);
		animation_sa_left.set_loop(false);
		animation_sa_left.set_anchor_mode(Animation::anchorMode::centered);
		animation_sa_left.add_frame(ResourcesManager::instance()->find_atlas("player_vfx_sa_left"));
	}
	{
		animation_sa_right.set_interval(0.3f);
		animation_sa_right.set_loop(false);
		animation_sa_right.set_anchor_mode(Animation::anchorMode::centered);
		animation_sa_right.add_frame(ResourcesManager::instance()->find_atlas("player_vfx_sa_right"));
	}
	{
		animation_ult_left.set_interval(0.3f);
		animation_ult_left.set_loop(false);
		animation_ult_left.set_anchor_mode(Animation::anchorMode::centered);
		animation_ult_left.add_frame(ResourcesManager::instance()->find_atlas("player_vfx_ult_left"));
	}
	{
		animation_ult_right.set_interval(0.3f);
		animation_ult_right.set_loop(false);
		animation_ult_right.set_anchor_mode(Animation::anchorMode::centered);
		animation_ult_right.add_frame(ResourcesManager::instance()->find_atlas("player_vfx_ult_right"));
	}
	
	//{状态机初始化}
	state_machine.register_state("idle", new PlayerIdleNode());
	state_machine.register_state("run",  new PlayerRunNode());
	state_machine.register_state("roll", new PlayerRollNode());
	state_machine.register_state("na",	 new PlayerNaNode());
	state_machine.register_state("sa",	 new PlayerSaNode());
	state_machine.register_state("ult",  new PlayerUltNode());
	state_machine.register_state("jump", new PlayerJumpNode());
	state_machine.register_state("fall", new PlayerFallNode());
	state_machine.register_state("dead", new PlayerDeadNode());

	state_machine.set_entry("idle");
}

Player::~Player() = default;

void Player::on_input(const ExMessage& msg)
{
	if (hp <= 0) return;

	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case 0x41:	//A
		case VK_LEFT:
			is_left_key_down = true;
			break;
		case 0x44:	//D
		case VK_RIGHT:
			is_right_key_down = true;
			break;
		case 0x57:	//W
		case 0x4B:	//k
		case VK_UP:
		case VK_SPACE:
			is_jump_key_down = true;
			break;
		case 0x53:	//S
		case 0x4C:  //L
		case VK_DOWN:
			is_roll_key_down = true;
			break;
		case 0x45:	//E
			is_sa_key_down = true;
			break;
		case 0x51:	//Q
			is_ult_key_down = true;
			break;
		case 0x4A:	//J
			is_na_key_down = true;
			break;
		case 0x4D:	//M
			is_mode_key_down = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case 0x41:	//A
		case VK_LEFT:
			is_left_key_down = false;
			break;
		case 0x44:	//D
		case VK_RIGHT:
			is_right_key_down = false;
			break;
		case 0x57:	//W
		case 0x4B:	//k
		case VK_UP:
		case VK_SPACE:
			is_jump_key_down = false;
			break;
		case 0x53:	//S
		case 0x4C:  //L
		case VK_DOWN:
			is_roll_key_down = false;
			break;
		case 0x45:	//E
			is_sa_key_down = false;
			break;
		case 0x51:	//Q
			is_ult_key_down = false;
			break;
		case 0x4A:	//J
			is_na_key_down = false;
			break;
		case 0x4D:	//M
			is_mode_key_down = false;
			break;
		}
		break;
	}
}

void Player::on_update(float delta)
{
	if (hp > 0 && !is_rolling)
		velocity.x = get_move_aixs() * SPEED_RUN;

	if (get_move_aixs() != 0)
		is_facing_left = (get_move_aixs() < 0);

	timer_roll_cd.on_update(delta);
	timer_na_cd.on_update(delta);
	timer_Mode_cd.on_update(delta);

	Character::on_update(delta);		

	if (is_na && !is_facing_left && attack_way == AttackWay::melee)  current_attack_animation = &animation_na_melee_right;	//动画结尾的on_finish函数应该将is_attacking置0
	if (is_na && is_facing_left && attack_way == AttackWay::melee)   current_attack_animation = &animation_na_melee_left;		//不对，可以在节点退出逻辑那里置位，宏观位置差不多就是了
	if (is_na && !is_facing_left && attack_way == AttackWay::ranged) current_attack_animation = &animation_na_ranged_right;
	if (is_na && is_facing_left && attack_way == AttackWay::ranged)  current_attack_animation = &animation_na_ranged_left;
	if (is_sa && !is_facing_left)  current_attack_animation = &animation_sa_right;
	if (is_sa && is_facing_left)   current_attack_animation = &animation_sa_left;
	if (is_ult && !is_facing_left) current_attack_animation = &animation_ult_right;
	if (is_ult && is_facing_left)  current_attack_animation = &animation_ult_left;

	if (is_attacking)
	{
		current_attack_animation->set_position(get_logic_center());
		current_attack_animation->on_update(delta);
	}

}

void Player::on_render()
{
	Character::on_render();
	if (is_attacking)
		current_attack_animation->on_render();
	if (mode_changing)
	{
		static int count=200;
		RECT r = { 0, 0, 1279, 479 };
		drawtext(_T("攻击方式已变更"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		if(count--)
		{ 
			mode_changing = false;
			count = 200;
		}
	}
	TCHAR Concerto[256];
	TCHAR Energy[256];
	_stprintf_s(Concerto, _T("Concerto:%d（3000）"), concerto);
	_stprintf_s(Energy, _T("Energy:%d（100）"), energy);
	RECT r1 = { 10, 100, 200, 200 };
	drawtext(Concerto, &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT r2 = { 10, 200, 200, 300 };
	drawtext(Energy, &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void Player::on_hurt()
{
	//play_audio(_T("player_hurt"), false);
}

void Player::on_roll()
{
	timer_roll_cd.restart();
	is_roll_cd_comp = false;
	velocity.x = is_facing_left ? -SPEED_ROLL : SPEED_ROLL;
}

void Player::on_jump()
{
	velocity.y -= SPEED_JUMP;
	//有关特效数据更新
}

void Player::on_land()
{
	//有关特效数据更新
}

void Player::on_na()
{
	timer_na_cd.restart();
	is_attacking_cd_comp = false;
}

void Player::on_sa()
{
	timer_na_cd.restart();
	is_attacking_cd_comp = false;
	this->energy -= 100;
}

void Player::on_ult()
{
	timer_na_cd.restart();
	is_attacking_cd_comp = false;
	this->concerto -= 3000;
}

void Player::mode_change()
{
	attack_way = (attack_way == AttackWay::melee) ? AttackWay::ranged : AttackWay::melee;
	timer_Mode_cd.restart();
	is_modechanging_cd_comp = false;
	mode_changing = true;

}


