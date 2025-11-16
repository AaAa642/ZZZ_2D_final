#include "Player_State_Nodes.h"
#include "Player.h"
#include "CharacterManager.h"

//PlayerIdleNode
void PlayerIdleNode::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("player_idle");
}

void PlayerIdleNode :: on_update(float delta)
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	if (player->can_mode())
		player->mode_change();

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->can_roll())
		player->switch_state("roll");
	else if (player->can_jump())
		player->switch_state("jump");
	else if (player->can_na())
		player->switch_state("na");
	else if (player->can_sa())
		player->switch_state("sa");
	else if (player->can_ult())
		player->switch_state("ult");
	else if (player->get_velocity().y > 0)
		player->switch_state("fall");
	else if (player->is_on_floor() && player->get_move_aixs() != 0)
		player->switch_state("run");

}


//PlayerRunNode
void PlayerRunNode::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("player_run");
	//播放跑步声
}

void PlayerRunNode::on_update(float delta)
{
	Player* player = (Player*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->get_move_aixs() == 0)
		player->switch_state("idle");
	else if (player->can_roll())
		player->switch_state("roll");
	else if (player->can_jump())
		player->switch_state("jump");
	else if (player->can_na())
		player->switch_state("na");
	else if (player->can_sa())
		player->switch_state("sa");
	else if (player->can_ult())
		player->switch_state("ult");
	else if (player->get_velocity().y > 0)
		player->switch_state("fall");

}

void PlayerRunNode::on_exit()
{
	//停止播放跑步声
}


//PlayerRollNode
PlayerRollNode::PlayerRollNode()	
{
	timer.set_wait_time(0.30f);		//翻滚持续时间
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			Player* player = (Player*)CharacterManager::instance()->get_player();
			player->set_rolling(false);
		});
}

void PlayerRollNode::on_enter()	//设置is_rolling为1，下面的节点类似，如is_sa和is_attacking同时置1
{
	CharacterManager::instance()->get_player()->set_animation("player_roll");

	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->get_hurt_box()->set_enabled(false);
	player->set_rolling(true);
	player->on_roll();

	timer.restart();

	//音效播放
}

void PlayerRollNode::on_update(float delta)
{
	timer.on_update(delta);

	Player* player = (Player*)CharacterManager::instance()->get_player();
	if (!player->get_rolling())
	{
		if (player->get_move_aixs() != 0)
			player->switch_state("run");
		else if (player->can_jump())
			player->switch_state("jump");
		else
			player->switch_state("idle");
	}
}

void PlayerRollNode::on_exit()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->get_hurt_box()->set_enabled(true);
}


//PlayerNaNode
PlayerNaNode::PlayerNaNode()
{
	timer.set_wait_time(0.5f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			Player* player = (Player*)CharacterManager::instance()->get_player();
			player->set_attacking(false);
			player->set_na(false);
		});
}

void PlayerNaNode::on_enter()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	switch (player->get_attack_way())
	{
	case Player::AttackWay::melee:
		player->set_animation("player_na_melee");
		break;
	case Player::AttackWay::ranged:
		player->set_animation("player_na_ranged");
		break;
	}

	player->get_hit_box()->set_enabled(true);
	player->set_attacking(true);
	player->set_na(true);
	update_na_hit_box_position();
	player->on_na();
	player->add_energy(20);
	player->add_concerto(50);
//player->decrease_hp();
	
	timer.restart();

	//音效播放
}

void PlayerNaNode::on_update(float delta)
{
	timer.on_update(delta);
	update_na_hit_box_position();

	Player* player = (Player*)CharacterManager::instance()->get_player();
	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (!player->get_na())
	{
		if (player->get_velocity().y > 0)
			player->switch_state("fall");
		else if (player->get_move_aixs() == 0)
			player->switch_state("idle");
		else if (player->is_on_floor() && player->get_move_aixs() != 0)
			player->switch_state("run");
	}
}

void PlayerNaNode::on_exit()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->get_hit_box()->set_enabled(false);
	player->reset_attack_vfx();
}

void PlayerNaNode::update_na_hit_box_position()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	Vector2 pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	hit_box->size={ 135,45 };
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;
	switch (player->get_attcack_dir())
	{
	case Player::AttackDir::Left:
		pos_hit_box = { pos_center.x - size_hit_box.x / 2,pos_center.y };
		break;
	case Player::AttackDir::Right:
		pos_hit_box = { pos_center.x + size_hit_box.x/2,pos_center.y };
		break;
	}
	hit_box->position=pos_hit_box;
}


//PlayerSaNode
PlayerSaNode::PlayerSaNode()
{
	timer.set_wait_time(0.4f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			Player* player = (Player*)CharacterManager::instance()->get_player();
			player->set_attacking(false);
			player->set_sa(false);
		});
}

void PlayerSaNode::on_enter()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->set_animation("player_sa");

	player->get_hurt_box()->set_enabled(false);
	player->get_hit_box()->set_enabled(true);
	player->set_attacking(true);
	player->set_sa(true);
	update_sa_hit_box_position();
	player->on_sa();
	player->add_concerto(250);

	timer.restart();
	//音效播放
}

void PlayerSaNode::on_update(float delta)
{
	timer.on_update(delta);
	update_sa_hit_box_position();

	Player* player = (Player*)CharacterManager::instance()->get_player();
	if (player->get_sa())
		player->set_velocity({ 0,0 });
	if (!player->get_sa())
	{
		if (player->get_move_aixs() != 0)
			player->switch_state("run");
		else if (player->can_jump())
			player->switch_state("jump");
		else
			player->switch_state("idle");
	}
}

void PlayerSaNode::on_exit()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->get_hit_box()->set_enabled(false);
	player->get_hurt_box()->set_enabled(true);
	player->reset_attack_vfx();
}

void PlayerSaNode::update_sa_hit_box_position()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	Vector2 pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	hit_box->size = { 80,300 };		//碰撞箱大小
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;
	switch (player->get_attcack_dir())
	{
	case Player::AttackDir::Left:
		pos_hit_box = { pos_center.x - size_hit_box.x / 2,pos_center.y };
		break;
	case Player::AttackDir::Right:
		pos_hit_box = { pos_center.x + size_hit_box.x / 2,pos_center.y };
		break;
	}
	hit_box->position=pos_hit_box;
}


//PlayerUltNode
PlayerUltNode::PlayerUltNode()
{
	timer.set_wait_time(0.4f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			Player* player = (Player*)CharacterManager::instance()->get_player();
			player->set_attacking(false);
			player->set_ult(false);
		});
}

void PlayerUltNode::on_enter()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->set_animation("player_ult");

	player->get_hurt_box()->set_enabled(false);
	player->get_hit_box()->set_enabled(true);
	player->set_attacking(true);
	player->set_ult(true);
	update_ult_hit_box_position();
	player->on_ult();

	timer.restart();
	//音效播放
}

void PlayerUltNode::on_update(float delta)
{
	timer.on_update(delta);
	update_ult_hit_box_position();

	Player* player = (Player*)CharacterManager::instance()->get_player();
	if (player->get_ult())
		player->set_velocity({ 0,0 });
	if (!player->get_ult())
	{
		if (player->get_move_aixs() != 0)
			player->switch_state("run");
		else if (player->can_jump())
			player->switch_state("jump");
		else
			player->switch_state("idle");
	}
}

void PlayerUltNode::on_exit()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->get_hit_box()->set_enabled(false);
	player->get_hurt_box()->set_enabled(true);
	player->reset_attack_vfx();
}

void PlayerUltNode::update_ult_hit_box_position()
{
	Player* player = (Player*)CharacterManager::instance()->get_player();
	Vector2 pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	hit_box->size={ 300,300 };
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;
	switch (player->get_attcack_dir())
	{
	case Player::AttackDir::Left:
		pos_hit_box = { pos_center.x - size_hit_box.x / 2,pos_center.y };
		break;
	case Player::AttackDir::Right:
		pos_hit_box = { pos_center.x + size_hit_box.x / 2,pos_center.y };
		break;
	}
	hit_box->position=pos_hit_box;
}



//PlayerJumpNode
void PlayerJumpNode::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("player_jump");

	Player* player = (Player*)CharacterManager::instance()->get_player();
	player->on_jump();

	//音效播放
}

void PlayerJumpNode::on_update(float delta)
{
	Player* player = (Player*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->get_velocity().y > 0)
		player->switch_state("fall");
	else if (player->can_na())
		player->switch_state("na");
}


//PlayerFallNode
void PlayerFallNode::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("player_fall");
}

void PlayerFallNode::on_update(float delta)
{
	Player* player = (Player*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("dead");
	else if (player->is_on_floor())
	{
		player->switch_state("idle");
		player->on_land();

		//音效播放
	}
	else if (player->can_na())
		player->switch_state("na");
}



///layerDeadNode
PlayerDeadNode::PlayerDeadNode()
{
	timer.set_wait_time(2.0f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			MessageBox(GetHWnd(), _T("Signal Lost!\n菜就多练"), _T("挑战失败"),MB_OK);
			//场景跳转代码
			exit(0);
		});
}

void PlayerDeadNode::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("player_dead");

	//音效播放
}

void PlayerDeadNode::on_update(float delta)
{
	timer.on_update(delta);
}
