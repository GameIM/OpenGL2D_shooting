/**
* @file GameData.h
*/
#include "Sprite.h"
#include "Font.h"

extern SpriteRenderer renderer;//スプライト描画用変数
extern FontRenderer fontRenderer;//フォント描画用変数

const int windowWidth = 800;//ウィンドウの描画領域の幅
const int windowHeight = 600;//ウィンドウの描画領域の高さ

const int gameStateTitle = 0;//タイトル画面の場面ID
const int gameStateMain = 1;//ゲーム画面の場面ID
const int gameStateGameOver = 2;//ゲームオーバー画面の場面ID
const int gameStatePose = 3;//ポーズ画面の場面ID
const int gameStateClear = 4;//ゲームクリア画面の場面ID
extern int gameState;//実行中の場面ID

//先行宣言
struct TitleScene;
struct MainScene;
struct GameOverScene;

extern TitleScene titleScene;
extern MainScene mainScene;
extern GameOverScene gameOverScene;
