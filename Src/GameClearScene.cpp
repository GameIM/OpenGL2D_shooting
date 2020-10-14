/**
* @file GameClearScene.cpp
*/
#include "GameClearScene.h"
#include "TitleScene.h"
#include "GameData.h"

/**
* ゲームクリア画面の初期設定を行う
*
* @param scene ゲームクリア画面用構造体のポインタ
*
* @retval true 初期化成功
* @retval false 初期化失敗
*/
bool initialize(GameClearScene* scene)
{
	Audio::EngineRef audio = Audio::Engine::Instance();
	scene->gameClearBgm = audio.Prepare("Res/Audio/happy.mp3");
	scene->gameClearBgm->Play();
	scene->bg = Sprite("Res/GameClear.png");
	scene->gameClear = Sprite("Res/Clear.png");
	scene->backTitle = Sprite("Res/BackTitle.png", glm::vec3(0, -150, 0));
	scene->mode = scene->modeStart;
	scene->timer = 0.5f;//入力を受け付けない期間(秒)
	return true;
}

/**
* ゲームクリア画面の終了処理を行う
*
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void finalize(GameClearScene* scene)
{
	scene->bg = Sprite();
	scene->gameClear = Sprite();
	scene->backTitle = Sprite();
	scene->gameClearBgm->Stop();
}

/**
* ゲームクリア画面のプレイヤー入力を処理する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void processInput(GLFWEW::WindowRef window, GameClearScene* scene)
{
	window.Update();
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown & (GamePad::A | GamePad::START))
	{
		finalize(scene);//ゲームクリア画面の後始末
		//タイトル画面に戻る
		gameState = gameStateTitle;
		initialize(&titleScene);
	}
}

/**
* ゲームクリア画面を更新する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void update(GLFWEW::WindowRef window, GameClearScene* scene)
{
	const float deltaTime = window.DeltaTime();

	scene->bg.Update(deltaTime);
	scene->gameClear.Update(deltaTime);
	scene->backTitle.Update(deltaTime);

	if (scene->timer > 0)
	{
		scene->timer -= deltaTime;
	}

	if (scene->mode == scene->modeStart)
	{
		scene->mode = scene->modeGameClear;
	}
	else if (scene->mode == scene->modeNextState)
	{
		finalize(scene);//ゲームクリア画面の後始末
		initialize(&titleScene);
	}
}

/**
* ゲームクリア画面を更新する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void render(GLFWEW::WindowRef window, GameClearScene* scene)
{
	renderer.BeginUpdate();
	renderer.AddVertices(scene->bg);
	renderer.AddVertices(scene->gameClear);
	renderer.AddVertices(scene->backTitle);
	renderer.EndUpdate();
	renderer.Draw(glm::vec2(windowWidth, windowHeight));
	window.SwapBuffers();
}