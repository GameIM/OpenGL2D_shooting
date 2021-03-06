/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include "MainScene.h"
#include "FirstHelpScene.h"
#include "GameData.h"

/**
* タイトル画面用の構造体の初期設定を行う
*
* @param scene タイトル画面用構造体のポインタ
* @param gamestate ゲーム状態を表す変数のポインタ
*
* @retval true 初期化成功
* @retval false 初期化失敗
*/
bool initialize(TitleScene* scene)
{
	Audio::EngineRef audio = Audio::Engine::Instance();
	scene->titleBgm = audio.Prepare("Res/Audio/Title.mp3");
	scene->titleBgm->Play(Audio::Flag_Loop);
	scene->bg = Sprite("Res/title.png");
	scene->logo = Sprite("Res/Title_S.png", glm::vec3(0, 100, 0));
	scene->Press = Sprite("Res/Enter.png", glm::vec3(-10,-140,0));
	scene->mode = scene->modeStart;
	scene->timer = 0.5f;//入力を受け付けない時間(秒)
	return true;
}

/**
* タイトル画面の終了処理を行う
*
* @param scene タイトル画面用構造体のポインタ
*/
void finalize(TitleScene* scene)
{
	scene->bg = Sprite();
	scene->logo = Sprite();
	scene->titleBgm->Stop();
}

/**
* タイトル画面のプレイヤー入力を処理する
*
* @param window ゲームを管理するウィンドウ
* @param scene タイトル画面用構造体のポインタ
*/
void processInput(GLFWEW::WindowRef window, TitleScene* scene)
{
	window.Update();
	//入力受付モードになるまでなにもしない
	if (scene->mode != scene->modeTitle)
	{
		return;
	}
	//STARTボタンが押されたら、ゲーム開始待ちモードに移る
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown &GamePad::START)
	{
		scene->mode = scene->modeNextState;
		scene->timer = 2.0f;
		Audio::Engine::Instance().Prepare("Res/Audio/Start.xwm")->Play();
	}
}

/**
* タイトル画面を更新する
*
* @param window ゲームを管理するウィンドウ
* @param scene タイトル画面用構造体のポインタ
*/
void update(GLFWEW::WindowRef window, TitleScene* scene)
{
	const float deltaTime = window.DeltaTime();

	scene->bg.Update(deltaTime);
	scene->logo.Update(deltaTime);
	scene->Press.Update(deltaTime);

	//タイマーが0以下になるまでカウントダウン
	if (scene->timer > 0)
	{
		scene->timer -= deltaTime;
		return;
	}

	if (scene->mode == scene->modeStart)
	{
		scene->mode = scene->modeTitle;
	}
	else if (scene->mode == scene->modeNextState)
	{
		finalize(scene);//タイトル画面の後始末

		//ゲームの初期設定を行う
		gameState = gameStateHelp;
		initialize(&firstHelpScene);
		
	}
}

/**
* タイトル画面を描画する
*
* @param window ゲームを管理するウィンドウ
* @param scene タイトル画面用構造体のポインタ
*/
void render(GLFWEW::WindowRef window, TitleScene* scene)
{
	renderer.BeginUpdate();
	renderer.AddVertices(scene->bg);
	renderer.AddVertices(scene->logo);
	if (scene->mode == scene->modeTitle)
	{
		renderer.AddVertices(scene->Press);
	}
	else if (scene->mode == scene->modeNextState)
	{
		if ((int)(scene->timer * 10) % 2)
		{
			renderer.AddVertices(scene->Press);
		}
	}
	renderer.EndUpdate();
	renderer.Draw(glm::vec2(windowWidth, windowHeight));
	window.SwapBuffers();
}

