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
	Sprite GameOver;
	Sprite BackTitle;
	const int modeStart = 0;//タイトル表示待ちモード
	const int modeGameOver = 1;//入力受付モード
	const int modeNextState = 2;//ゲーム開始待ちモード
	int mode;//実行中のモード
	float timer;
	Audio::SoundPtr GameOverBgm;
};
bool initialize(GameOverScene*);
void finalize(GameOverScene*);
void processInput(GLFWEW::WindowRef, GameOverScene*);
void update(GLFWEW::WindowRef, GameOverScene*);
void render(GLFWEW::WindowRef, GameOverScene*);
