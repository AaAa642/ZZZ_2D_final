#include "ResourcesManager.h"
#include "utility.h"

struct ImageResInfo
{
	std::string id;
	LPCTSTR path;
};

struct AtlasResInfo
{
	std::string id;
	LPCTSTR path;
	int num_frame = 0;
};

static const std::vector<ImageResInfo> image_info_list =
{
	{"background",_T(R"(resources\background.png)")},
	{"ui_heart",  _T(R"(resources\ui_heart.png)")}



};

static const std::vector<AtlasResInfo> atlas_info_list =
{
	{ "player_idle_right",		_T(R"(resources\player\basic\idle\%d.png)"),		1 },
	{ "player_run_right",		_T(R"(resources\player\basic\run\%d.png)"),			2 },
	{ "player_roll_right",		_T(R"(resources\player\basic\roll\%d.png)"),		4 },
	{ "player_na_melee_right",  _T(R"(resources\player\basic\na\%d.png)"),			8 },
	{ "player_na_ranged_right", _T(R"(resources\player\basic\na\%d.png)"),			8 },
	{ "player_sa_right",		_T(R"(resources\player\basic\sa\%d.png)"),			3 },
	{ "player_ult_right",		_T(R"(resources\player\basic\ult\%d.png)"),			3 },
	{ "player_jump_right",		_T(R"(resources\player\basic\jump\%d.png)"),		8 },
	{ "player_fall_right",		_T(R"(resources\player\basic\fall\%d.png)"),		4 },
	{ "player_dead_right",		_T(R"(resources\player\basic\dead\%d.png)"),		1 },

	{ "player_vfx_na_melee_right",	_T(R"(resources\player\special\na_melee\%d.png)"),		3 },
	{ "player_vfx_na_ranged_right",	_T(R"(resources\player\special\na_ranged\%d.png)"),		3 },
	{ "player_vfx_sa_right",		_T(R"(resources\player\special\sa\%d.png)"),			2 },
	{ "player_vfx_ult_right",		_T(R"(resources\player\special\ult\%d.png)"),			2 },

	{ "player_idle_left",		_T(R"(resources\player\basic\idle_L\%d.png)"),		1 },
	{ "player_run_left",		_T(R"(resources\player\basic\run_L\%d.png)"),			2 },
	{ "player_roll_left",		_T(R"(resources\player\basic\roll_L\%d.png)"),		4 },
	{ "player_na_melee_left",  _T(R"(resources\player\basic\na_L\%d.png)"),			8 },
	{ "player_na_ranged_left", _T(R"(resources\player\basic\na_L\%d.png)"),			8 },
	{ "player_sa_left",		_T(R"(resources\player\basic\sa_L\%d.png)"),			    3 },
	{ "player_ult_left",		_T(R"(resources\player\basic\ult_L\%d.png)"),			3 },
	{ "player_jump_left",		_T(R"(resources\player\basic\jump_L\%d.png)"),		8 },
	{ "player_fall_left",		_T(R"(resources\player\basic\fall_L\%d.png)"),		4 },


	{"barb_break",					_T(R"(resources\enemy\barb_break\%d.png)")			,3 },
	{"barb_loose",					_T(R"(resources\enemy\barb_loose\%d.png)")			,5 },
	{ "silk",						_T(R"(resources\enemy\silk\%d.png)")				,9 },
	{ "sword_left",					_T(R"(resources\enemy\sword\%d.png)")				,3 },

	{ "enemy_aim_left",				_T(R"(resources\enemy\aim\%d.png)")				    ,9 },
	{ "enemy_dash_in_air_left",		_T(R"(resources\enemy\dash_in_air\%d.png)")			,2 },
	{ "enemy_dash_on_floor_left",	_T(R"(resources\enemy\dash_on_floor\%d.png)")		,2 },
	{ "enemy_fall_left",			_T(R"(resources\enemy\fall\%d.png)")				,4 },
	{ "enemy_idle_left",			_T(R"(resources\enemy\idle\%d.png)")				,6 },
	{ "enemy_jump_left",			_T(R"(resources\enemy\jump\%d.png)")				,8 },
	{ "enemy_run_left",				_T(R"(resources\enemy\run\%d.png)")					,8 },
	{ "enemy_squat_left",			_T(R"(resources\enemy\squat\%d.png)")				,10},
	{ "enemy_throw_barb_left",		_T(R"(resources\enemy\throw_barb\%d.png)")			,8 },
	{ "enemy_throw_silk_left",		_T(R"(resources\enemy\throw_silk\%d.png)")			,17},
	{ "enemy_throw_sword_left",		_T(R"(resources\enemy\throw_sword\%d.png)")			,16},

	{"enemy_vfx_dash_in_air_left",	_T(R"(resources\enemy\vfx_dash_in_air\%d.png)")		,5 },
	{"enemy_vfx_dash_on_floor_left",_T(R"(resources\enemy\vfx_dash_on_floor\%d.png)")	,6 },

};

static inline bool check_image_valid(IMAGE* image)
{
	return GetImageBuffer(image);
}



ResourcesManager* ResourcesManager::manager = nullptr;
ResourcesManager* ResourcesManager::instance()
{
	if (!manager)
		manager = new ResourcesManager();

	return manager;
}

ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() = default;

void ResourcesManager::load()
{
	for (const auto& info : image_info_list)
	{
		IMAGE* image = new IMAGE();
		loadimage(image, info.path);
		if (!check_image_valid(image))
			throw info.path;
		image_pool[info.id] = image;
	}

	for(const auto& info:atlas_info_list)
	{
		Atlas* atlas = new Atlas();
		atlas->load(info.path, info.num_frame);
		for (int i = 0; i < atlas->get_size(); i++)
		{
			IMAGE* image = atlas->get_image(i);
			if (!check_image_valid(image))
				throw info.path;
		}
		atlas_pool[info.id] = atlas;
	}
	//对某些资源进行反转处理
	flip_atlas("player_dead_right", "player_dead_left");

	flip_atlas("player_vfx_na_melee_right", "player_vfx_na_melee_left");
	flip_atlas("player_vfx_na_melee_right", "player_vfx_na_ranged_left");
	flip_atlas("player_vfx_sa_right", "player_vfx_sa_left");
	flip_atlas("player_vfx_ult_right", "player_vfx_ult_left");


	flip_atlas("sword_left", "sword_right");
	flip_atlas("enemy_aim_left", "enemy_aim_right");
	flip_atlas("enemy_dash_in_air_left", "enemy_dash_in_air_right");
	flip_atlas("enemy_dash_on_floor_left", "enemy_dash_on_floor_right");
	flip_atlas("enemy_fall_left", "enemy_fall_right");
	flip_atlas("enemy_idle_left", "enemy_idle_right");
	flip_atlas("enemy_jump_left", "enemy_jump_right");
	flip_atlas("enemy_run_left", "enemy_run_right");
	flip_atlas("enemy_squat_left", "enemy_squat_right");
	flip_atlas("enemy_throw_barb_left", "enemy_throw_barb_right");
	flip_atlas("enemy_throw_silk_left", "enemy_throw_silk_right");
	flip_atlas("enemy_throw_sword_left", "enemy_throw_sword_right");

	flip_atlas("enemy_vfx_dash_in_air_left", "enemy_vfx_dash_in_air_right");
	flip_atlas("enemy_vfx_dash_on_floor_left", "enemy_vfx_dash_on_floor_right");
	//load_audio();

}

Atlas* ResourcesManager::find_atlas(const std::string & id) const
{
	const auto& itor = atlas_pool.find(id);
	if (itor == atlas_pool.end())
		return nullptr;

	return itor->second;
}

IMAGE* ResourcesManager::find_image(const std::string& id) const
{
	const auto& itor = image_pool.find(id);
	if (itor == image_pool.end())
		return nullptr;

	return itor->second;
}

void ResourcesManager::flip_image(IMAGE* src_image, IMAGE* dst_image, int num_h)
{
	int w = src_image->getwidth();
	int h = src_image->getheight();
	int w_frame = w / num_h;
	Resize(dst_image, w, h);
	DWORD* src_buffer = GetImageBuffer(src_image);
	DWORD* dst_buffer = GetImageBuffer(dst_image);
	for (int i = 0; i < num_h; i++)
	{
		int x_left = i * w_frame;
		int x_right = (i + 1) * w_frame;
		for (int y = 0; y < h; y++)
		{
			for (int x = x_left; x < x_right; x++)
			{
				int idx_src = y * w + x;
				int idx_dst = y * w + x_right - (x - x_left);
				dst_buffer[idx_dst] = src_buffer[idx_src];
			}
		}
	}
}

void ResourcesManager::flip_image(const std::string& src_id, const std::string dst_id, int num_h)
{
	IMAGE* src_image = image_pool[src_id];
	IMAGE* dst_image = new IMAGE();

	flip_image(src_image, dst_image, num_h);

	image_pool[dst_id] = dst_image;
}

void ResourcesManager::flip_atlas(const std::string& src_id, const std::string dst_id)
{
	Atlas* src_atlas = atlas_pool[src_id];
	Atlas* dst_atlas = new Atlas();

	for (int i = 0; i < src_atlas->get_size(); i++)
	{
		IMAGE img_flipped;
		flip_image(src_atlas->get_image(i), &img_flipped);
		dst_atlas->add_img(img_flipped);
	}

	atlas_pool[dst_id] = dst_atlas;
}