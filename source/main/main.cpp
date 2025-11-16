#include "utility.h"
#include "ResourcesManager.h"
#include "CharacterManager.h"
#include "CollisionManager.h"

#include <graphics.h>
#include <chrono>
#include <thread>

#include <iostream>


static void draw_background()
{
	static IMAGE* img_background = ResourcesManager::instance()->find_image("background");//取名背景图background
	static Rect rect_dst =
	{
		(getwidth() - img_background->getwidth()) / 2,
		(getheight() - img_background->getheight()) / 2,
		img_background->getwidth(),
		img_background->getheight()
	};
	putimage_ex(img_background, &rect_dst);
}

static void draw_remain_player_hp()
{
	static IMAGE* img_ui_heart = ResourcesManager::instance()->find_image("ui_heart");
	Rect rect_dst = { 0,10,img_ui_heart->getwidth(),img_ui_heart->getheight() };
	for (int i = 0; i < CharacterManager::instance()->get_player()->get_hp(); i++)
	{
		rect_dst.x = 10 + i * 40;
		putimage_ex(img_ui_heart, &rect_dst);
	}
}

static void draw_remain_enemy_hp()
{
	static IMAGE* img_ui_heart = ResourcesManager::instance()->find_image("ui_heart");
	Rect rect_dst = { 800,10,img_ui_heart->getwidth(),img_ui_heart->getheight() };
	for (int i = 0; i < CharacterManager::instance()->get_enemy()->get_hp(); i++)
	{
		rect_dst.x = 800 + i * 40;
		putimage_ex(img_ui_heart, &rect_dst);
	}
}

int main(int argc,char** argv)
{
	using namespace std::chrono;

	HWND hwnd=initgraph(1280, 720, EX_SHOWCONSOLE);
	SetWindowText(hwnd, _T("ZZZ_2D"));

	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id)
	{
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("无法加载：%s"), id);
		MessageBox(hwnd, err_msg, _T("资源加载失败"),MB_OK | MB_ICONERROR);
		return -1;
	}

	//BGM播放

	//游戏玩法说明
	std::cout<<"玩家初始生命：10"<<std::endl;
	std::cout << "Boss初始生命：12" << std::endl;
	std::cout << "每次普攻获取能量20，喧响值20" << std::endl;
	std::cout << "每次战技获取喧响值250" << std::endl << std::endl;
	std::cout << "按键说明如下：" << std::endl;
	std::cout << "W,S：\t\t左右" << std::endl;
	std::cout << "A,K,SPACE：\t跳跃" << std::endl;
	std::cout << "S,L：\t\t冲刺（期间无敌）" << std::endl;
	std::cout << "J：\t\t普攻" << std::endl;
	std::cout << "E：\t\t战技（能量100释放，存储上限300）" << std::endl;
	std::cout << "Q：\t\t终结技（喧响值3000释放，存储上限3000）" << std::endl;

	const nanoseconds frame_duration(1000000000 / 14);
	steady_clock::time_point last_tick=steady_clock::now();

	ExMessage msg;
	bool running = true;

	BeginBatchDraw();

	while (running)
	{
		while (peekmessage(&msg))  
		{
			//消息处理部分
			CharacterManager::instance()->on_input(msg);
		}
		
		steady_clock::time_point frame_start = std::chrono::steady_clock::now();
		duration<float> delta = duration<float>(frame_start - last_tick);
		//处理逻辑更新
	
		CharacterManager::instance()->on_update(delta.count());
		CollisionManager::instance()->process_collide();
	
		cleardevice();

		draw_background();
	    CharacterManager::instance()->on_render();
		//CollisionManager::instance()->on_debug_render();
		draw_remain_player_hp();
		draw_remain_enemy_hp();
		

		FlushBatchDraw();

		last_tick = frame_start;
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
		if (sleep_duration > nanoseconds(0))
			std::this_thread::sleep_for(sleep_duration);
	}

	EndBatchDraw();
	return 0;
}