#include "Player.h"
#include "Map_setting.h"
#include <doodle\doodle.hpp>
using namespace doodle;

bool moveW = false;
bool moveA = false;
bool moveS = false;
bool moveD = false;

void on_key_pressed(KeyboardButtons button) {
	if (button == KeyboardButtons::W) {
		moveW = true;
	}
	if (button == KeyboardButtons::A) {
		moveA = true;
	}
	if (button == KeyboardButtons::S) {
		moveS = true;
	}
	if (button == KeyboardButtons::D) {
		moveD = true;
	}
}

void on_key_released(KeyboardButtons button) {
	if (button == KeyboardButtons::W) {
		moveW = false;
	}
	if (button == KeyboardButtons::A) {
		moveA = false;
	}
	if (button == KeyboardButtons::S) {
		moveS = false;
	}
	if (button == KeyboardButtons::D) {
		moveD = false;
	}
}


void Player::draw_chara() {
	push_settings();
	set_image_mode(RectMode::Center);
	draw_image(tiles[map_setting.CHARA], chara_pos_x, chara_pos_y, tile_size, tile_size);
	pop_settings();
}

void Player::MOVE() {
	if (moveW == true) {
		chara_pos_y -= 6;
	}
	if (moveA == true) {
		chara_pos_x -= 6;
	}
	if (moveS == true) {
		chara_pos_y += 6;
	}
	if (moveD == true) {
		chara_pos_x += 6;
	}
	//���콺 ���⺸�� ĳ���� �¿����
}

void Player_setting::move_limit(Player* player)
{
	//Player move limit
	if (player->chara_pos_x < player_limit_x1)
	{
		player->chara_pos_x += 6;
	}
	if (player->chara_pos_y > player_limit_y)
	{
		player->chara_pos_y -= 6;
	}
	if (player->chara_pos_x > player_limit_x)
	{
		player->chara_pos_x -= 6;
	}
	if (player->chara_pos_y < player_limit_y1)
	{
		player->chara_pos_y += 6;
	}
	player->draw_chara();
	player->MOVE();
}
