/**
* @file FirstHelpScene.h
*/
#include "GLFWEW.h"
#include "Sprite.h"

struct FirstHelpScene
{
	//スプライト用の変数
	Sprite bg;
	Sprite Keypad;
	Sprite Up;
	Sprite Left;
	Sprite Right;
	Sprite Down;
	Sprite Big;
	Sprite Skey;
	Sprite Space;

	//文字スプライト用の変数
	Sprite Hand;
	Sprite BackMain;
	Sprite Wepon;
	Sprite Attack;
	Sprite Cursors;
	Sprite ToMain;
	const int modeStart = 0;//タイトル表示待ちモード
	const int modeGameHelp = 1;//入力受付モード
	const int modeNextState = 2;//ゲーム開始待ちモード
	float timer;//モード切り替えで使用するタイマー
	int mode;//実行中のモード
};
bool initialize(FirstHelpScene*);
void finalize(FirstHelpScene*);
void processInput(GLFWEW::WindowRef, FirstHelpScene*);
void update(GLFWEW::WindowRef, FirstHelpScene*);
void render(GLFWEW::WindowRef, FirstHelpScene*);


