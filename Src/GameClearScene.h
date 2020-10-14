/**
* @ file GameCearScene.h
*
* ゲームクリア画面のための構造体及び関数を宣言するヘッダファイル
*/
#include "GLFWEW.h"
#include "Sprite.h"
#include "Audio.h"

struct GameClearScene
{
	Sprite bg;
	Sprite gameClear;
	Sprite backTitle;
	const int modeStart = 0;//タイトル表示待ちモード
	const int modeGameClear = 1;//入力受付モード
	const int modeNextState = 2;//ゲーム開始待ちモード
	int mode;//実行中のモード
	float timer;
	Audio::SoundPtr gameClearBgm;
};
bool initialize(GameClearScene*);
void finalize(GameClearScene*);
void processInput(GLFWEW::WindowRef, GameClearScene*);
void update(GLFWEW::WindowRef, GameClearScene*);
void render(GLFWEW::WindowRef, GameClearScene*);
