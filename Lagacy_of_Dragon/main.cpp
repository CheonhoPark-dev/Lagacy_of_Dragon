#include <doodle/doodle.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "Player.h"
#include "Map_setting.h"
#include "Shooting.h"
#include "Enemy.h"
#include "Window_setting.h"
#include "Logos.h"
#include "Main_menu.h"
#include "Tutorial.h"

using namespace std;
using namespace doodle;

static constexpr int bullet_x = -200;
static constexpr int bullet_y = -200;
static constexpr int bullet_vel = 5;
static constexpr int bullet_size = 10;

//--------------------------------// Timer for scene
double scene_timer = 0;
double tutorial_timer = 0;
int digipenlogo_check = 2;
int teamlogo_check = 7;
int tutorial_check = 2;
//--------------------------------// Tutorial Scene 
int clicked_check = 0;
constexpr int clicked_success = 3;
//--------------------------------// Scene
int scene = 0;
int tutorial_scene = 0;

//--------------------------------// Random Enemy


int Chap1_Enemy = 0;
double bulletradius = 5;
double enemyradius = 15;
double chararadius = 25;

int chap1_point = 20;
//--------------------------------//Bullet
double bullet_timer = 0;
int bullet_check = 4;
int bullet_max = 5;

//--------------------------------// Enemy const
constexpr int enemyWidth = 10;
constexpr int enemyHeight = 10;
//--------------------------------// bool
bool enemy_check = false;
bool bullet_draw_check = false;
bool player_die_check = false;
bool tutorial_scene3 = false;
//--------------------------------// RandomSkill
int randomScene = 0;
int box_x = 500;



int randomboxh = 500;
int randomboxSize = 200;
int acc_x = 0;
double skillTimer = 0;
double SkillTimeCheck = 5;

const Image Fire{ "Fire.jpg" };
const Image Water{ "Water.jpg" };
const Image Star{ "Star.jpg" };

Map_setting map_setting;
Window_setting window_setting;
Logos logos;
Main_menu main_menu;
Tutorial tutorial;
Player_setting player_setting;
Shooting_update shooting_update;
Enemy_update enemy_update;

void on_key_pressed(KeyboardButtons button);
void on_key_released(KeyboardButtons button);

int main()
{
	window_setting.setting();

	vector<Shooting*> bullets;
	vector<Enemy*> enemys;
	vector<Enemy*> tutoenemys;
	vector<int> randomboxloc = { 500, 700, 900 };

	Player* player = new Player{ 0, 0 };

	map_setting.char_pos(player);

	while (!is_window_closed())
	{
		timer += DeltaTime;
		bullet_timer += DeltaTime;
		scene_timer += DeltaTime;
		update_window();


		//Game_start
		//DIGIEPN LOGO
		if (scene == 0)
		{
			logos.digipen_logo();
			if (scene_timer > digipenlogo_check)
			{
				scene += 1;
			}
		}

		//TEAM HATCHLING
		if (scene == 1)
		{
			logos.team_logo();
			if (scene_timer > teamlogo_check)
			{
				scene += 1;
			}
		}

		//Main Menu
		if (scene == 2)
		{
			main_menu.main_UI();

			//Gameplay
			if (main_menu.is_gameplay())
			{
				scene = 6;
			}
			//Settings
			if (main_menu.is_setting())
			{
				scene = 3;
			}
			//Credits
			if (main_menu.is_credit())
			{
				scene = 4;
			}
			//Exit
			if (main_menu.is_exit())
			{
				scene = 5;
			}
		}

		//Settings
		if (scene == 3)
		{
			main_menu.in_setting();
			if (main_menu.is_in_setting())
			{
				scene = 2;
			}
		}

		//Credits
		if (scene == 4)
		{
			main_menu.in_credit();
			if (main_menu.is_in_credit())
			{
				scene = 2;
			}
		}

		//Exit
		if (scene == 5)
		{
			return 0;
		}


		//Gameplay_start
		//Tutorial
		if (scene == 6)
		{
			map_setting.map_creating();

			//First Scene
			if (tutorial_scene == 0)
			{
				not_clicked = true;
				player->chara_pos_x = 300;
				player->chara_pos_y = 300;
				player->draw_chara();

				tutorial_scene += 1;

			}

			//Second Scene(Move)
			if (tutorial_scene == 1)
			{
				tutorial.scene1_guideline();
				player_setting.move_limit(player);

				if (tutorial.is_clear_scene1(player))
				{
					tutorial_scene = 2;
				}
			}

			//Third Scene(Shoot)			
			if (tutorial_scene == 2)
			{
				tutorial.scene2_guideline();
				player_setting.move_limit(player);

				//bullet_create
				shooting_update.bullet_create(bullets, player);

				if (bullets.size() >= 5)
				{
					tutorial_scene = 3;
				}

				//bullet draw
				shooting_update.bullet_draw(bullets);

				//Bullet Remove
				shooting_update.bullet_remove(bullets);
			}

			//Fourth Scene (Kill Enemies)
			if (tutorial_scene == 3)
			{
				tutorial.scene3_guideline();

				//Player move limit
				player_setting.move_limit(player);

				//Bullet_shooting
				shooting_update.bullet_create(bullets, player);


				//Create Enemy
				enemy_update.tuto_enemy_create(tutoenemys);

				//Enemy Move
				enemy_update.enemy_move(tutoenemys, player);

				//bullet draw
				shooting_update.bullet_draw(bullets);

				//Bullet Remove
				shooting_update.bullet_remove(bullets);

				//Bullet Enemy Check
				for (int i = 0; i < bullets.size(); i++)
				{
					for (int j = 0; j < tutoenemys.size(); j++)
					{
						double a = bullets[i]->bullet_pos_x - tutoenemys[j]->x;
						double b = bullets[i]->bullet_pos_y - tutoenemys[j]->y;
						double distance = sqrt(a * a + b * b);

						if (distance < bulletradius + enemyradius)
						{
							score += 1;
							delete bullets[i];
							delete tutoenemys[j];

							bullets.erase(bullets.begin() + i);
							tutoenemys.erase(tutoenemys.begin() + j);
							break;
						}
					}
				}

				//Me Enemy check
				for (int j = 0; j < tutoenemys.size(); j++)
				{
					for (int i = 0; i < bullets.size(); i++)
					{
						double a = player->chara_pos_x - tutoenemys[j]->x;
						double b = player->chara_pos_y - tutoenemys[j]->y;
						double distance = sqrt(a * a + b * b);

						if (distance < chararadius + enemyradius)
						{
							player_die_check = true;
						}
					}
				}

				//Move to Tutorial_last
				if (score >= tuto_enemy_max)
				{
					scene = 7;
				}
			}
		}

		//Tutorial_last
		if (scene == 7)
		{

			//Player move limit
			player_setting.move_limit(player);

			//Bullet_shooting
			shooting_update.bullet_create(bullets, player);

			//Draw Map
			map_setting.map_creating();

			//Create bullet
			shooting_update.bullet_create(bullets, player);

			shooting_update.bullet_draw(bullets);

			//Bullet Remove
			shooting_update.bullet_remove(bullets);

			//Random enemy
			if (timer > timer_check)
			{
				for (int i = 0; i < Max; i++)
				{
					push_settings();
					int r_enemy_y = random(enemyMin, enemyMax);
					int r_enemy_x = random(enemyMin, enemyMax);
					enemys.push_back(new Enemy{ r_enemy_x, r_enemy_y, enemySize });
					doodle::pop_settings();
				}
				timer_check += 4;
			}

			//Enemy move
			enemy_update.enemy_move(enemys, player);

			//Me Enemy check
			for (int j = 0; j < enemys.size(); j++)
			{
				double a = player->chara_pos_x - enemys[j]->x;
				double b = player->chara_pos_y - enemys[j]->y;
				double distance = sqrt(a * a + b * b);

				if (distance < chararadius + enemyradius)
				{
					player_die_check = true;
				}
			}

			//Bullet Enemy Check
			for (int i = 0; i < bullets.size(); i++)
			{
				for (int j = 0; j < enemys.size(); j++)
				{
					double a = bullets[i]->bullet_pos_x - enemys[j]->x;
					double b = bullets[i]->bullet_pos_y - enemys[j]->y;
					double distance = sqrt(a * a + b * b);

					if (distance < bulletradius + enemyradius)
					{
						chap1_point--;
						delete bullets[i];
						bullets.erase(bullets.begin() + i);

						delete enemys[j];
						enemys.erase(enemys.begin() + j);
						break;
					}
				}
			}



			//Move next chapter
			if (chap1_point == 0)
			{
				scene = 8;
			}

			//Draw point
			push_settings();
			draw_text(to_string(chap1_point) + " / 20", score_width, score_height);
			pop_settings();

			player->MOVE();
			player->draw_chara();

		}

		//Tutorial_black scene   
		if (scene == 8)
		{
			clear_background(255);

			//Draw Map
			map_setting.map_creating();

			player->MOVE();
			player->draw_chara();

			if (randomScene == 0)
			{
				//���� ��ġ
				draw_image(Fire, randomboxloc[0], randomboxh, randomboxSize, randomboxSize);
				draw_image(Water, randomboxloc[1], randomboxh, randomboxSize, randomboxSize);
				draw_image(Star, randomboxloc[2], randomboxh, randomboxSize, randomboxSize);

				for (int i = 0; i < 1; i++)
				{
					//�귿
					push_settings();
					no_fill();
					set_outline_color(HexColor{ 0xff0000ff });
					set_outline_width(8.0);
					draw_rectangle(box_x, randomboxh, randomboxSize, randomboxSize);
					pop_settings();

					//�귿 �ӵ�
					acc_x = 30;
					box_x += acc_x;

					//�귿 ����
					if (box_x < randomboxloc[0] || box_x > randomboxloc[2])
					{
						box_x = randomboxloc[0];
					}

					//�귿 �۵�
					if (get_mouse_x() > randomboxloc[0] && get_mouse_x() < randomboxloc[2] + randomboxSize && get_mouse_y() > randomboxh && get_mouse_y() < randomboxh + randomboxSize)
					{

						skillTimer += DeltaTime;
						//�ð� ����ؼ� �ӵ� ������(���� �ӵ��� 30�̶� skillTimer = 5, �ӵ� 6������
						if (skillTimer < SkillTimeCheck)
						{
							box_x -= skillTimer * 6; // �̰� acc_x���� ���ƾ���
						}
						else if (skillTimer > SkillTimeCheck)
						{
							//1��° �ڽ�
							if (box_x <= 675)
							{
								box_x = randomboxloc[0];
								push_settings();
								no_fill();
								set_outline_color(HexColor{ 0xff0000ff });
								set_outline_width(8.0);
								draw_rectangle(box_x, randomboxh, randomboxSize, randomboxSize);
								pop_settings();
							}//2���� �ڽ�
							else if (box_x > 625 || box_x < 825)
							{
								box_x = randomboxloc[1];
								push_settings();
								no_fill();
								set_outline_color(HexColor{ 0xff0000ff });
								set_outline_width(8.0);
								draw_rectangle(box_x, randomboxh, randomboxSize, randomboxSize);
								pop_settings();
							}//3��° �ڽ�
							else if (box_x >= 825)
							{
								box_x = randomboxloc[2];
								push_settings();
								no_fill();
								set_outline_color(HexColor{ 0xff0000ff });
								set_outline_width(8.0);
								draw_rectangle(box_x, randomboxh, randomboxSize, randomboxSize);
								pop_settings();
							}
						}
					}
				}
				//R �ʱ�ȭ
				if (KeyIsPressed && Key == KeyboardButtons::R)
				{
					skillTimer = 0;
					randomScene = 0;
				}

				draw_line(700, randomboxh, 700, 700);
				draw_line(900, randomboxh, 900, 700);

				draw_rectangle(randomboxloc[0], randomboxh, 600, randomboxSize);
			}

		}

	}
	return 0;

	void reset();
}

void reset()
{

}
