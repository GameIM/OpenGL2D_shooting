/**
* @file FirstHelp.cpp
*/
#include "FirstHelpScene.h"
#include "MainScene.h"
#include "GameData.h"

/**
* 操作説明画面の初期設定を行う
*
* @param scene ゲームオーバー画面用構造体のポインタ
*
* @retval true 初期化成功
* @retval false 初期化失敗
*/
bool initialize(FirstHelpScene* scene)
{
	//スプライトを設定する
	scene->bg = Sprite("Res/Pose.png");
	scene->Keypad = Sprite("Res/Keypad.png", glm::vec3(0, 20, 0));
	scene->Up = Sprite("Res/Cursors.png", glm::vec3(263, -60, 0));
	scene->Left = Sprite("Res/Cursors.png", glm::vec3(225, -100, 0));
	scene->Right = Sprite("Res/Cursors.png", glm::vec3(302, -102, 0));
	scene->Down = Sprite("Res/Down.png", glm::vec3(260, -125, 0));
	scene->Big = Sprite("Res/Big.png", glm::vec3(300, 130, 0));
	scene->Skey = Sprite("Res/Skey.png", glm::vec3(-157, -30, 0));
	scene->Space = Sprite("Res/Space.png", glm::vec3(-25, -105, 0));

	//文字スプライトを設定する
	scene->Hand = Sprite("Res/Hand.png", glm::vec3(0, 260, 0));
	scene->BackMain = Sprite("Res/BackMain.png", glm::vec3(-250, 220, 0));
	scene->Wepon = Sprite("Res/Wepon.png", glm::vec3(-240, -180, 0));
	scene->Attack = Sprite("Res/Attack.png", glm::vec3(60, -210, 0));
	scene->Cursors = Sprite("Res/Move.png", glm::vec3(250, -170, 0));
	scene->ToMain = Sprite("Res/ToMain.png", glm::vec3(245,210,0));
	scene->mode = scene->modeStart;
	scene->timer = 0.5f;//入力を受け付けない時間(秒)
	return true;
}

/**
* 操作説明画面の終了処理を行う
*
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void finalize(FirstHelpScene* scene)
{
	//スプライトの後始末をする
	scene->Keypad = Sprite();
	scene->bg = Sprite();
	scene->Up = Sprite();
	scene->Left = Sprite();
	scene->Right = Sprite();
	scene->Down = Sprite();
	scene->Big = Sprite();
	scene->Skey = Sprite();
	scene->Space = Sprite();

	//文字スプライトの後始末をする
	scene->Hand = Sprite();
	scene->Wepon = Sprite();
	scene->Attack = Sprite();
	scene->Cursors = Sprite();
	scene->ToMain = Sprite();
}

/**
* 操作説明画面のプレイヤー入力を処理する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void processInput(GLFWEW::WindowRef window, FirstHelpScene* scene)
{
	window.Update();
	//入力受付モードになるまでなにもしない
	if (scene->mode != scene->modeGameHelp)
	{
		return;
	}
	//STARTボタンが押されたら、ゲーム開始待ちモードに移る
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown &GamePad::START)
	{
		scene->mode = scene->modeNextState;
		scene->timer = 0.5f;
	}
}

/**
* 操作説明画面を更新する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void update(GLFWEW::WindowRef window, FirstHelpScene* scene)
{
	const float deltaTime = window.DeltaTime();
	//スプライトを更新する
	scene->bg.Update(deltaTime);
	scene->Up.Update(deltaTime);
	scene->Left.Update(deltaTime);
	scene->Right.Update(deltaTime);
	scene->Down.Update(deltaTime);
	scene->Big.Update(deltaTime);
	scene->Skey.Update(deltaTime);
	scene->Space.Update(deltaTime);

	//文字スプライトを更新する
	scene->Keypad.Update(deltaTime);
	scene->Hand.Update(deltaTime);
	scene->Wepon.Update(deltaTime);
	scene->Attack.Update(deltaTime);
	scene->Cursors.Update(deltaTime);
	scene->ToMain.Update(deltaTime);

	//タイマーが0以下になるまでカウントダウン
	if (scene->timer > 0)
	{
		scene->timer -= deltaTime;
		return;
	}
	if (scene->mode == scene->modeStart)
	{
		scene->mode = scene->modeGameHelp;
	}
	else if (scene->mode == scene->modeNextState)
	{
		finalize(scene);//ゲームオーバー画面の後始末

		gameState = gameStateMain;
		mainScene.stageNo = 1;//ステージ1から開始
		initialize(&mainScene);
	}
}

/**
* 操作説明画面を更新する
*
* @param window ゲームを管理するウィンドウ
* @param scene ゲームオーバー画面用構造体のポインタ
*/
void render(GLFWEW::WindowRef window, FirstHelpScene* scene)
{
	renderer.BeginUpdate();
	//スプライトの頂点データを設定する
	renderer.AddVertices(scene->bg);
	renderer.AddVertices(scene->Keypad);
	renderer.AddVertices(scene->Up);
	renderer.AddVertices(scene->Left);
	renderer.AddVertices(scene->Right);
	renderer.AddVertices(scene->Down);
	renderer.AddVertices(scene->Big);
	renderer.AddVertices(scene->Skey);
	renderer.AddVertices(scene->Space);

	//文字スプライトの頂点データを設定する
	renderer.AddVertices(scene->Hand);
	renderer.AddVertices(scene->Wepon);
	renderer.AddVertices(scene->Attack);
	renderer.AddVertices(scene->Cursors);
	renderer.AddVertices(scene->ToMain);
	renderer.EndUpdate();
	renderer.Draw(glm::vec2(windowWidth, windowHeight));
	window.SwapBuffers();
}