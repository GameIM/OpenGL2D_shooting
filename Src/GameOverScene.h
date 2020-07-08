/**
* @file GameOverScene.h
*
* ゲームオーバー画面のための構造体及び関数を宣言するヘッダファイル
*/
#include "GLFWEW.h"
#include "Sprite.h"
#include "Audio.h"

struct GameOverScene
{
	Sprite bg;
	float timer;
};
bool initialize(GameOverScene*);
void finalize(GameOverScene*);
void processInput(GLFWEW::WindowRef, GameOverScene*);
void update(GLFWEW::WindowRef, GameOverScene*);
void render(GLFWEW::WindowRef, GameOverScene*);
