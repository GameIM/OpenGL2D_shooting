/**
* @file GameOverScene.cpp
*/
#include "GameOverScene.h"
#include "TitleScene.h"
#include "GameData.h"

/**
* ゲームオーバー画面の初期設定を行う
*
* @param scene ゲームオーバー画面用構造体のポインタ
*
* @retval true 初期化成功
* @retval false 初期化失敗
*/
bool initialize(GameOverScene* scene)
{
	Audio::EngineRef audio = Audio::Engine::Instance();
	scene->GameOverBgm = audio.Prepare("Res/Audio/GameOver.mp3");
	scene->GameOverBgm->Play();
	scene->bg = Sprite("Res/GameOver.png");
	scene->gameOver = Sprite("Res/Over.png");
	scene->backTitle = Sprite("Res/BackTitle.png",glm::vec3(0,-150,0));
	scene->mode = scene->modeStart;
	scene->timer = 0.5f;//入力を受け付けない期間(秒)
	return true;
}

/**
* ゲームオーバー画面の終了処理を行う
*
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void finalize(GameOverScene* scene)
{
	scene->bg = Sprite();
	scene->gameOver = Sprite();
	scene->backTitle = Sprite();
	scene->GameOverBgm->Stop();
}

/**
* ゲームオーバー画面のプレイヤー入力を処理する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void processInput(GLFWEW::WindowRef window, GameOverScene* scene)
{
	window.Update();
	if (scene->timer > 0)
	{
		return;
	}
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown & GamePad::START)
	{
		finalize(scene);//ゲームオーバー画面の後始末

		//タイトル画面に戻る
		gameState = gameStateTitle;
		initialize(&titleScene);
	}
}

/**
* ゲームオーバー画面を更新する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void update(GLFWEW::WindowRef window, GameOverScene* scene)
{
	const float deltaTime = window.DeltaTime();

	scene->bg.Update(deltaTime);
	scene->gameOver.Update(deltaTime);
	scene->backTitle.Update(deltaTime);

	if (scene->timer > 0)
	{
		scene->timer -= deltaTime;
	}

	if (scene->mode == scene->modeStart)
	{
		scene->mode = scene->modeGameOver;
	}
	else if (scene->mode == scene->modeNextState)
	{
		finalize(scene);//ゲームオーバー画面の後始末
		initialize(&titleScene);
	}
}

/**
* ゲームオーバー画面を更新する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void render(GLFWEW::WindowRef window, GameOverScene* scene)
{
	renderer.BeginUpdate();
	renderer.AddVertices(scene->bg);
	renderer.AddVertices(scene->gameOver);
	renderer.AddVertices(scene->backTitle);
	renderer.EndUpdate();
	renderer.Draw(glm::vec2(windowWidth, windowHeight));
	window.SwapBuffers();
}