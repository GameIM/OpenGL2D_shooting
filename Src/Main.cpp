/*
@file Main.cpp
*/
#include "MainScene.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameData.h"
#include "Actor.h"
#include "GLFWEW.h"
#include "Texture.h"
#include "Sprite.h"
#include "Font.h"
#include "TiledMap.h"
#include "Audio.h"
#include <random>
#include <glm/gtc/constants.hpp>

const char windowTitle[] = "Shoting Game";//タイトルバーに表示される文章

std::mt19937 random;//乱数を発生させる変数(乱数エンジン)


SpriteRenderer renderer;//スプライト描画用変数
FontRenderer fontRenderer;//フォント描画用変数
Sprite sprBackground;//背景用スプライト
Actor sprPlayer;//自機用スプライト

glm::vec3 playerVelocity;//自機の移動速度
Actor playerBulletList[128];//自機の弾のリスト
Actor playerLaserList[3];//自機のレーザーのリスト
Actor enemyList[128];//敵のリスト
Actor effectList[128];//爆発などの特殊効果用スプライトのリスト
Actor itemList[64];//アイテム用スプライトのリスト
Actor enemyBulletList[256];//敵の弾のリスト

int score;//プレイヤーの得点
float timer;

const int weaponLevelMin = 1;//自機の武器強化の最低段階
const int weaponLevelMax = 5;//自機の武器強化の最高段階
int weaponLevel;//自機の武器強化段階

const int weaponTypeWideShot = 0;//広範囲ショット
const int weaponTypeLaser = 1;//レーザー
int weaponType;//選択中の武器

//音声制御用変数
Audio::SoundPtr bgm;
Audio::SoundPtr sePlayerShot;
Audio::SoundPtr sePlayerLaser;
Audio::SoundPtr seBlast;
Audio::SoundPtr sePowerUp;

//敵のアニメーション
const FrameAnimation::KeyFrame enemyKeyFrames[] =
{
	{0.000f, glm::vec2(480,0), glm::vec2(32,32)},
	{0.125f, glm::vec2(480,96), glm::vec2(32,32)},
	{0.250f, glm::vec2(480,64), glm::vec2(32,32)},
	{0.375f, glm::vec2(480,32), glm::vec2(32,32)},
	{0.500f, glm::vec2(480,0), glm::vec2(32,32)},
};
FrameAnimation::TimelinePtr tlEnemy;

//自機のアニメーション
const FrameAnimation::KeyFrame playerKeyFrames[] =
{
	{0.000f, glm::vec2(0,0), glm::vec2(64,32)},
	{0.125f, glm::vec2(0,32), glm::vec2(64,32)},
	{0.250f, glm::vec2(0,64), glm::vec2(64,32)},
	{0.375f, glm::vec2(0,0), glm::vec2(64, 32)},
};
FrameAnimation::TimelinePtr tlPlayer;

//爆発アニメーション
const FrameAnimation::KeyFrame blastKeyFrames[] =
{
	{0 / 60.0f, glm::vec2(416,0), glm::vec2(32,32)},
	{5 / 60.0f, glm::vec2(416,32), glm::vec2(32,32)},
	{10 / 60.0f, glm::vec2(416,64), glm::vec2(32,32)},
	{15 / 60.0f, glm::vec2(416,96), glm::vec2(32,32)},
	{20 / 60.0f, glm::vec2(416,96), glm::vec2(32,32)},
};
FrameAnimation::TimelinePtr tlBlast;

//敵の出現を制御するためのデータ
TiledMap enemyMap;
float mapCurrentPosX;
float mapProcessedX;
bool isEndOfMap;//マップの終端に到達したらtrue

float enemyGenerationTimer;//次の敵が出現するまでの時間(単位:秒)

//ゲームの状態
int gameState;//実行中の場面ID
Actor* boss;//ボスActorのポインタ
bool isStagePassed;//ステージをクリアしていればtrue

TitleScene titleScene;
MainScene mainScene;
GameOverScene gameOverScene;

//プロトタイプ宣言
void processInput(GLFWEW::WindowRef);
void update(GLFWEW::WindowRef);
void render(GLFWEW::WindowRef);
void playerBulletAndEnemyContactHandler(Actor* bullet, Actor* enemy);
void playerAndEnemyBulletContactHandler(Actor* player, Actor* bullet);
void playerLaserAndEnemyContactHandler(Actor* laser, Actor* enemy);
void playerAndEnemyContactHandler(Actor* player, Actor* enemy);
void playerAndItemContactHandler(Actor*, Actor*);
void stopPlayerLaser();
void generateObjectFromMap(float);
void updateEnemies(float);

/**
* メイン画面用の構造体の初期設定を行う
*
* @param scene メイン画面用構造体のポインタ
*
* @retval true 初期化成功
* @retval false 初期化失敗
*/
bool initialize(MainScene* scene)
{
	sprBackground = Sprite("Res/UnknownPlanet.png");
	sprPlayer.spr =
		Sprite("Res/Objects.png", glm::vec3(0, 0, 0), Rect(0, 0, 64, 32));
	sprPlayer.collisionShape = Rect(-24, -8, 48, 16);
	sprPlayer.health = 100;
	initializeActorList(std::begin(enemyList), std::end(enemyList));
	initializeActorList(std::begin(playerBulletList), std::end(playerBulletList));
	initializeActorList(std::begin(effectList), std::end(effectList));
	initializeActorList(std::begin(itemList), std::end(itemList));
	initializeActorList(std::begin(playerLaserList), std::end(playerLaserList));
	initializeActorList(std::begin(enemyBulletList), std::end(enemyBulletList));

	if (scene->stageNo == 1)
	{
		score = 0;
		weaponLevel = weaponLevelMin;
		weaponType = weaponTypeWideShot;
	}

	//敵配置マップを読み込む
	enemyMap.Load("Res/EnemyMap.json");
	mapCurrentPosX = mapProcessedX = windowWidth;
	isEndOfMap = false;
	boss = nullptr;
	isStagePassed = false;

	//音声を準備する
	Audio::EngineRef audio = Audio::Engine::Instance();
	seBlast = audio.Prepare("Res/Audio/Blast.xwm");
	sePlayerShot = audio.Prepare("Res/Audio/PlayerShot.xwm");
	sePlayerLaser = audio.Prepare("Res/Audio/Laser.xwm");
	sePowerUp = audio.Prepare("Res/Audio/GetItem.xwm");
	bgm = audio.Prepare("Res/Audio/Neolith.xwm");

	//BGMをループを再生する
	bgm->Play(Audio::Flag_Loop);
	return true; 
}

/*
プログラムのエントリポイント
*/
int main()
{
	//アプリケーションの初期化
	GLFWEW::WindowRef window = GLFWEW::Window::Instance();
	if (!window.Initialize(windowWidth, windowHeight, windowTitle))
	{
		return 1;
	}
	//音声再生システムの初期化
	Audio::EngineRef audio = Audio::Engine::Instance();
	if (!audio.Initialize())
	{
		return 1;
	}
	if (!Texture::Initialize())
	{
		return 1;
	}
	if (!renderer.Initialize(1024))
	{
		return 1;
	}
	if (!fontRenderer.Initialize(1024, glm::vec2(windowWidth, windowHeight)))
	{
		return 1;
	}
	if (!fontRenderer.LoadFromFile("Res/Font/makinas_scrap.fnt"))
	{
		return 1;
	}

	random.seed(std::random_device()());//乱数エンジンの初期化

	//アニメーションタイムラインの作成
	tlEnemy = FrameAnimation::Timeline::Create(enemyKeyFrames);//敵タイムラインの作成
	tlPlayer = FrameAnimation::Timeline::Create(playerKeyFrames);//プレイヤータイムラインの作成
	tlBlast = FrameAnimation::Timeline::Create(blastKeyFrames);//爆発タイムラインの作成
	
	initializeMapObjectDataList();

	//タイトル画面を初期化する
	gameState = gameStateTitle;
	initialize(&titleScene);

	//ゲームループ
	while (!window.ShouldClose())
	{
		processInput(window);
		update(window);
		render(window);
		audio.Update();
	}

	Texture::Finalize();
	audio.Destroy();
	return 0;
}

/**
*プレイヤーの入力を処理する
*
* @param window ゲームを開始するウィンドウ
*/
void processInput(GLFWEW::WindowRef window)
{
	if (gameState == gameStateTitle)
	{
		processInput(window, &titleScene);
		return;
	}
	else if (gameState == gameStateGameOver)
	{
		processInput(window, &gameOverScene);
		return;
	}

	window.Update();

	if (sprPlayer.health <= 0)
	{
		playerVelocity = glm::vec3(0, 0, 0);
		stopPlayerLaser();
	}
	else
	{
		//自機の速度を設定する
		const GamePad gamepad = window.GetGamePad();
		if (gamepad.buttons &GamePad::DPAD_UP)
		{
			playerVelocity.y = 1;
		}
		else if (gamepad.buttons &GamePad::DPAD_DOWN)
		{
			playerVelocity.y = -1;
		}
		else
		{
			playerVelocity.y = 0;
		}
		if (gamepad.buttons &GamePad::DPAD_RIGHT)
		{
			playerVelocity.x = 1;
		}
		else if (gamepad.buttons &GamePad::DPAD_LEFT)
		{
			playerVelocity.x = -1;
		}
		else
		{
			playerVelocity.x = 0;
		}
		if (playerVelocity.x || playerVelocity.y)
		{
			//playerVelocity *= 400.0f;//プレイヤーの移動速度
			playerVelocity = glm::normalize(playerVelocity) * 400.0f;//ベクトルの長さを正規化
		}

		//武器の切り替え
		if (gamepad.buttonDown & GamePad::B)
		{
			if (weaponType == weaponTypeWideShot)
			{
				weaponType = weaponTypeLaser;
			}
			else
			{
				weaponType = weaponTypeWideShot;
			}
		}

		//弾の反射
		if (weaponType == weaponTypeWideShot &&
			(gamepad.buttonDown & GamePad::A))
		{
			for (int i = 0; i < weaponLevel; i++)
			{
				//空いている弾の構造体の関数呼び出し
				Actor* bullet = findAvailableActor(
					std::begin(playerBulletList), std::end(playerBulletList));

				//空いている構造体が見つかったら、それを使って弾を発射する
				if (bullet->health <= 0)
				{
					bullet->spr = Sprite("Res/Objects.png",
						sprPlayer.spr.Position(), Rect(64, 0, 32, 16));
					const float angles[] = { 0,7.5f,-7.5f,15.0f,-15.0f };
					const float radian = angles[i] / 180.0f * 3.14f;
					const float c = std::cos(radian);
					const float s = std::sin(radian);
					bullet->spr.Tweener(TweenAnimation::Animate::Create(
						TweenAnimation::MoveBy::Create(1, glm::vec3(1200 * c , 1200 * s, 0),
						TweenAnimation::EasingType::Linear)));
					bullet->spr.Rotation(radian);
					bullet->collisionShape = Rect(-16, -8, 32, 16);
					bullet->health = 1;
				}
			}
			sePlayerShot->Play();//弾の発射音を再生
		}
		if ((weaponType == weaponTypeLaser) && (gamepad.buttons & GamePad::A))
		{
			if (playerLaserList[0].health <= 0)
			{
				const glm::vec3 posFiringPoint = sprPlayer.spr.Position();
				playerLaserList[0].spr = Sprite("Res/Objects.png", posFiringPoint,
					Rect(96, 0, 32, 16));
				playerLaserList[0].collisionShape = Rect(-8, -4, 16, 8);
				playerLaserList[0].health = 1;
				playerLaserList[1].spr = Sprite("Res/Objects.png", posFiringPoint,
					Rect(112, 0, 32, 16));
				playerLaserList[1].health = 1;
				playerLaserList[1].collisionShape = Rect(-8, -4, 16, 8);
				playerLaserList[2].spr = Sprite("Res/Objects.png", posFiringPoint,
					Rect(128, 0, 32, 16));
				playerLaserList[2].collisionShape = Rect(-8, -4, 16, 8);
				playerLaserList[2].health = 1;
				sePlayerLaser->Play(Audio::Flag_Loop);
			}
		}
		else
		{
			stopPlayerLaser();
		}
	}
}

/**
*ゲームの状態を更新する
*
* @param window ゲームを管理するウィンドウ
*/
void update(GLFWEW::WindowRef window)
{
	if (gameState == gameStateTitle)
	{
		update(window, &titleScene);
		return;
	}
	else if (gameState == gameStateMain)
	{
		//自機が破壊されていたらゲームオーバー画面を表示する
		if (sprPlayer.health <= 0)
		{
			bgm->Stop();//BGMを停止する
			gameState = gameStateGameOver;
			initialize(&gameOverScene);
			return;
		}
	}
	else if (gameState == gameStateGameOver)
	{
		update(window, &gameOverScene);
		return;
	}

	const float deltaTime = window.DeltaTime();//前回の更新からの経過時間(秒)

	//ステージクリア判定
	if (!isStagePassed && boss && (boss->health <= 0))
	{
		isStagePassed = true;
		boss = nullptr;
		timer = 2;
	}
	if (isStagePassed)
	{
		timer -= deltaTime;
		if (timer <= 0)
		{
			stopPlayerLaser();
			++mainScene.stageNo;
			initialize(&mainScene);
			return;
		}
	}

	//自機の移動
	if (sprPlayer.health > 0)
	{
		if (playerVelocity.x || playerVelocity.y)
		{
			glm::vec3 newPos = sprPlayer.spr.Position() + playerVelocity * deltaTime;

			//自機の移動範囲を画面内に制限する
			const Rect playerRect = sprPlayer.spr.Rectangle();
			if (newPos.x < -0.5f * (windowWidth - playerRect.size.x))
			{
				newPos.x = -0.5f *(windowWidth - playerRect.size.x);
			}
			else if (newPos.x > 0.5f * (windowWidth - playerRect.size.x))
			{
				newPos.x = 0.5f * (windowWidth - playerRect.size.x);
			}
			if (newPos.y < -0.5f * (windowHeight - playerRect.size.y))
			{
				newPos.y = -0.5f * (windowHeight - playerRect.size.y);
			}
			else if (newPos.y > 0.5f * (windowHeight - playerRect.size.y))
			{
				newPos.y = 0.5f * (windowHeight - playerRect.size.y);
			}
			sprPlayer.spr.Position(newPos);
		}
		sprPlayer.spr.Update(deltaTime);

		/*
		//得点に応じて自機の武器を強化する
		weponLevel = weponLevelMin + score / 2000;
		if (weponLevel > weponLevelMax)
		{
			weponLevel = weponLevelMax;
		}
		*/

#if 1
		generateObjectFromMap(deltaTime);
#else
		//出現までの時間が0以下になったら敵を出現させる
		enemyGenerationTimer -= deltaTime;
		if (enemyGenerationTimer <= 0)
		{
			//空いてる(破壊されている)敵構造体を検索
			Actor* enemy = nullptr;
			for (Actor* i = std::begin(enemyList); i != std::end(enemyList); ++i)
			{
				if (i->health <= 0)
				{
					enemy = i;
					break;
				}
			}
			//空いてる構造体が見つかったら、それを使って敵を出現させる
			if (enemy != nullptr)
			{
				const std::uniform_real_distribution<float> y_distribution(
					-0.5f * windowHeight, 2.0f * windowHeight);
				enemy->spr = Sprite("Res/Objects.png",
					glm::vec3(0.5f * windowWidth,
						y_distribution(random), 0), Rect(480, 0, 32, 32));
				enemy->spr.Animator(FrameAnimation::Animate::Create(tlEnemy));
				namespace TA = TweenAnimation;
				TA::SequencePtr seq = TA::Sequence::Create(4);
				seq->Add(TA::MoveBy::Create(
					1, glm::vec3(0, 150, 0),
					TA::EasingType::EaseInOut, TA::Target::Y));
				seq->Add(TA::MoveBy::Create(
					1, glm::vec3(-200, 0, 0),
					TA::EasingType::EaseInOut, TA::Target::X));
				seq->Add(TA::MoveBy::Create(
					1, glm::vec3(0, -150, 0),
					TA::EasingType::EaseInOut, TA::Target::Y));
				seq->Add(TA::MoveBy::Create(
					1, glm::vec3(200, 0, 0),
					TA::EasingType::EaseInOut, TA::Target::X));
				seq->Add(TA::MoveBy::Create(
					1, glm::vec3(-200, 0, 0),
					TA::EasingType::EaseInOut, TA::Target::X));
				TA::ParallelizePtr par = TA::Parallelize::Create(1);
				par->Add(seq);
				par->Add(TA::MoveBy::Create(8, glm::vec3(-1000, 0, 0),
					TA::EasingType::Linear, TA::Target::X));
				enemy->spr.Tweener(TA::Animate::Create(seq));
				enemy->collisionShape = Rect(-16, -16, 32, 32);
				enemy->health = 10;
				//次の敵が出現するまでの時間を設定する
				enemyGenerationTimer = 2;
			}
		}
#endif
		//Actor配列の更新の関数呼び出し
		updateEnemies(deltaTime);
		updateActorList(std::begin(enemyList), std::end(enemyList), deltaTime);
		updateActorList(std::begin(playerBulletList), std::end(playerBulletList), deltaTime);
		updateActorList(std::begin(effectList), std::end(effectList), deltaTime);
		updateActorList(std::begin(itemList), std::end(itemList),deltaTime);
		updateActorList(std::begin(playerLaserList), std::end(playerLaserList),deltaTime);
		updateActorList(std::begin(enemyBulletList), std::end(enemyBulletList), deltaTime);

		//レーザーの移動
		if (playerLaserList[0].health > 0)
		{
			//レーザー(頭部)の移動
			const float laserSpeed = 1600.0f;
			const glm::vec3 posFiringPoint = sprPlayer.spr.Position();
			glm::vec3 posHead = playerLaserList[2].spr.Position();
			posHead.x += laserSpeed * deltaTime;
			if (posHead.x > windowWidth * 0.5f)
			{
				posHead.x = windowWidth * 0.5f;
			}
			posHead.y = posFiringPoint.y;
			playerLaserList[2].spr.Position(posHead);

			//レーザー(胴部)の移動
			const float halfHeadSize = playerLaserList[2].spr.Rectangle().size.x * 0.5f;
			const float halfTailSize = playerLaserList[0].spr.Rectangle().size.x * 0.5f;
			const float bodySize = playerLaserList[1].spr.Rectangle().size.x;
			const float bodyLength =
				posHead.x - posFiringPoint.x - halfHeadSize - halfTailSize;
			glm::vec3 posBody = playerLaserList[1].spr.Position();
			posBody.x = posFiringPoint.x + (posHead.x - posFiringPoint.x) * 0.5f;
			posBody.y = posFiringPoint.y;
			playerLaserList[1].spr.Position(posBody);
			playerLaserList[1].spr.Scale(glm::vec2(bodyLength / bodySize, 1));
			playerLaserList[1].collisionShape =
				Rect(-bodyLength * 0.5f, -4, bodyLength, 8);

			//レーザー(尾部)の移動
			playerLaserList[0].spr.Position(posFiringPoint);
		}

		//自機の弾と敵の衝突判定
		detectCollision(std::begin(playerBulletList), std::end(playerBulletList),
			std::begin(enemyList), std::end(enemyList),
			playerBulletAndEnemyContactHandler);

		//自機と敵の弾の衝突判定
		detectCollision(
			&sprPlayer, &sprPlayer + 1,
			std::begin(enemyBulletList), std::end(enemyBulletList),
			playerAndEnemyBulletContactHandler);

		//自機のレーザーと敵の衝突判定
		detectCollision(
			std::begin(playerLaserList), std::end(playerLaserList),
			std::begin(enemyList), std::end(enemyList),
			playerLaserAndEnemyContactHandler);

		//自機と敵の衝突判定
		detectCollision(
			&sprPlayer, &sprPlayer + 1,
			std::begin(enemyList), std::end(enemyList),
			playerAndEnemyContactHandler);

		//自機とアイテムの衝突判定
		detectCollision(
		&sprPlayer, &sprPlayer + 1,
			std::begin(itemList), std::end(itemList),
			playerAndItemContactHandler);
	}
	
}

/**
*ゲームの状態を描画する
*
* @param window ゲームを管理するウィンドウ
*/
void render(GLFWEW::WindowRef window)
{
	if (gameState == gameStateTitle)
	{
		render(window, &titleScene);
		return;
	}
	else if (gameState == gameStateGameOver)
	{
		render(window, &gameOverScene);
		return;
	}

	renderer.BeginUpdate();
	renderer.AddVertices(sprBackground);
	if (sprPlayer.health > 0)
	{
		renderer.AddVertices(sprPlayer.spr);
	}
	renderActorList(std::begin(enemyList) ,std::end(enemyList));
	renderActorList(std::begin(playerBulletList), std::end(playerBulletList));
	renderActorList(std::begin(effectList), std::end(effectList));
	renderActorList(std::begin(itemList), std::end(itemList));
	renderActorList(std::begin(playerLaserList), std::end(playerLaserList));
	renderActorList(std::begin(enemyBulletList), std::end(enemyBulletList));
	renderer.EndUpdate();
	renderer.Draw({ windowWidth,windowHeight });

	fontRenderer.BeginUpdate();
	char str[64];
	snprintf(str, sizeof(str), "SCORE:%08d", score);
	fontRenderer.AddString(glm::vec2(-120, 300), str);
	snprintf(str, sizeof(str), "体力:%02d/%02d", sprPlayer.health,100);
	fontRenderer.Color(glm::vec4(0.2f, 0.7f, 0.2f, 1));
	fontRenderer.Scale(glm::vec2(0.7f, 0.7f));
	fontRenderer.AddString(glm::vec2(250, 280), str);
	fontRenderer.Scale(glm::vec2(0.6f, 0.6f));
	fontRenderer.AddString(glm::vec2(-390,280), "上下左右キー:移動");
	fontRenderer.AddString(glm::vec2(-390,260), "SPACEキー:攻撃");
	fontRenderer.AddString(glm::vec2(-390,240), "Sキー:武器切り替え");

	snprintf(str, sizeof(str), "STAGE: %02d", mainScene.stageNo);
	fontRenderer.Scale(glm::vec2(0.4f, 0.4f));
	fontRenderer.Color(glm::vec4(0.75f, 0.75f, 1, 1));
	fontRenderer.Thickness(0);
	fontRenderer.AddString(glm::vec2(-384, 300), str);
	fontRenderer.Scale(glm::vec2(1, 1));
	fontRenderer.Color(glm::vec4(1, 1, 1, 1));
	fontRenderer.Thickness(0.33f);

	

	if (isStagePassed)
	{
		fontRenderer.AddString(glm::vec2(-100, 32), "STAGE CLEAR!");
	}
	fontRenderer.EndUpdate();
	fontRenderer.Draw();

	window.SwapBuffers();
}

/**
* 衝突を検出する
*
* @param fristA 衝突させる配列Aの先頭ポインタ
* @param lastA 衝突させる配列Aの終端ポインタ
* @param fristB 衝突させる配列Bの先頭ポインタ
* @param lastB 衝突させる配列Bの終端ポインタ
* @param handler A-B間で衝突が検出されたときに実行する関数
*/
void detectCollision(Actor* firstA, Actor* lastA,
	Actor*firstB, Actor* lastB, CollisionHandlerType handler)
{
	for(Actor* a = firstA; a != lastA; ++a)
	{
		if (a->health <= 0)
		{
			continue;
		}
		Rect rectA = a->collisionShape;
		rectA.origin += glm::vec2(a->spr.Position());
		for (Actor* b = firstB; b != lastB; ++b)
		{
			if (b->health <= 0)
			{
				continue;
			}
			Rect rectB = b->collisionShape;
			rectB.origin += glm::vec2(b->spr.Position());
			if (detectCollision(&rectA, &rectB))
			{
				handler(a, b);
				if (a->health <= 0)
				{
					break;
				}
			}
		}
	}
}

/**
* 自機の弾と敵の衝突を処理する
*
* @param bullet 自機の弾のポインタ
* @param enemy 敵のポインタ
*/
void playerBulletAndEnemyContactHandler(Actor* bullet, Actor* enemy)
{
	float health;
	health = enemy->health;
	enemy->health -= bullet->health;
	bullet->health -= health;

	if (enemy->health <= 0)
	{
		score += 100;//敵を破壊したら得点を増やす
		Actor* blast = findAvailableActor(
			std::begin(effectList), std::end(effectList));
		if(blast != nullptr)
		{
			blast->spr = Sprite("Res/Objects.png",enemy->spr.Position());
			blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
			namespace TA = TweenAnimation;
			blast->spr.Tweener(TA::Animate::Create(
				TA::Rotation::Create(20 / 60.0f, 1.5f)));
			blast->health = 1;
			seBlast->Play();//爆発音を再生
		}
	}
}

/**
* 自機と敵の弾の衝突を処理する
*
* @param bullet 自機の弾のポインタ
* @param enemy 敵のポインタ
*/
void playerAndEnemyBulletContactHandler(Actor* player, Actor* bullet)
{
	float health;
	health = bullet->health;
	bullet->health -= player->health;
	player->health -= health;

	if (player->health >= 0)
	{
		score -= 50;//敵の弾に衝突したらスコアを減らす
		if (score <= 0)
		{
			score = 0;
		}
		Actor* blast = findAvailableActor(
			std::begin(effectList), std::end(effectList));
		if (blast != nullptr)
		{
			blast->spr = Sprite("Res/Objects.png", bullet->spr.Position());
			blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
			namespace TA = TweenAnimation;
			blast->spr.Tweener(TA::Animate::Create(
				TA::Rotation::Create(20 / 60.0f, 1.5f)));
			blast->health = 1;
			seBlast->Play();//爆発音を再生
		}
	}
}

/**
* 自機のレーザーと敵の衝突判定
*
* @param laser 自機のレーザーのポインタ
* @param enemy 敵のポインタ
*/
void playerLaserAndEnemyContactHandler(Actor* laser, Actor* enemy)
{
	enemy->health -= laser->health;
	if (enemy->health <= 0)
	{
		score += 30;

		//爆発を表示する
		Actor* blast =
			findAvailableActor(std::begin(effectList), std::end(effectList));
		if (blast != nullptr)
		{
			blast->spr = Sprite("Res/Objects.png", enemy->spr.Position());
			blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
			namespace TA = TweenAnimation;
			blast->spr.Tweener(
				TA::Animate::Create(TA::Rotation::Create(20 / 60.0f, 1.5f)));
			blast->health = 1;
			seBlast->Play();//爆発音を再生
		}
	}
}

/**
* 自機と敵の衝突を処理する
*
* @param bullet 自機のポインタ
* @param enemy 敵のポインタ
*/
void playerAndEnemyContactHandler(Actor* player, Actor* enemy)
{
	const int tmp = player->health;
	player->health -= enemy->health;
	enemy->health -= tmp;

	if (enemy->health <= 0)
	{
		Actor* blast = findAvailableActor(
			std::begin(effectList), std::end(effectList));
		if (blast != nullptr)
		{
			blast->spr = Sprite("Res/Objects.png", enemy->spr.Position());
			blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
			namespace TA = TweenAnimation;
			blast->spr.Tweener(TA::Animate::Create(
				TA::Rotation::Create(20 / 60.0f, 1.5f)));
			blast->health = 1;
			seBlast->Play();//爆発音を再生
		}
	}
	if (player->health <= 0)
	{
		Actor* blast = findAvailableActor(
			std::begin(effectList), std::end(effectList));
		if (blast != nullptr)
		{
			blast->spr = Sprite("Res/Objects.png", player->spr.Position());
			blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
			namespace TA = TweenAnimation;
			blast->spr.Tweener(TA::Animate::Create(
				TA::Rotation::Create(20 / 60.0f, 1.5f)));
			blast->spr.Scale(glm::vec2(2, 2));//爆発の大きさ
			blast->health = 1;
			seBlast->Play();//爆発音を再生
		}
	}
}

/**
* 自機とアイテムの衝突を処理する
*
* @param player 自機のポインタ
* @param item アイテムのポインタ
*/
void playerAndItemContactHandler(Actor* player, Actor* item)
{
	item->health = 0;
	sePowerUp->Play();

	//自機の武器を強化する
	weaponLevel++;
	if (weaponLevel > weaponLevelMax)
	{
		weaponLevel = weaponLevelMax;
	}
}

void stopPlayerLaser()
{
	if (playerLaserList[0].health > 0)
	{
		for (Actor* i = std::begin(playerLaserList);
			i != std::end(playerLaserList); i++)
		{
			i->spr = Sprite();
			i->health = 0;
		}
		sePlayerLaser->Stop();
	}
}

/**
* マップデータを見て敵やアイテムを出現させる
*
* @param deltaTime 前回の更新からの経過時間
*/
void generateObjectFromMap(float deltaTime)
{
	if (isEndOfMap)
	{
		return;
	}
	const TiledMap::Layer& tiledMapLayer = enemyMap.GetLayer(0);
	const glm::vec2 tileSize =
		enemyMap.GetTileSet(tiledMapLayer.tilesetNo).size;

	//敵配置マップ参照位置の更新
	const float enemyMapScrollSpeed = 100;//更新速度
	mapCurrentPosX += enemyMapScrollSpeed * deltaTime;
	if (mapCurrentPosX >= tiledMapLayer.size.x * tileSize.x)
	{
		//終端を超えたら停止
		isEndOfMap = true;
		return;
	}
	//次の列に到達したらデータを読む
	if (mapCurrentPosX - mapProcessedX >= tileSize.x)
	{
		mapProcessedX += tileSize.x;
		const int mapX = static_cast<int>(mapProcessedX / tileSize.x);
		for (int mapY = 0; mapY < tiledMapLayer.size.y; ++mapY)
		{
			const int tileId = tiledMapLayer.At(mapY, mapX);
			const MapObjectData* data = findMapObjectData(tileId);
			if (data != nullptr)
			{
				Actor* enemy = findAvailableActor(
					std::begin(enemyList), std::end(enemyList));

				//空いている構造体が見つかったら、それを使って敵を出現させる
				if (enemy != nullptr)
				{
					const float y =
						(windowHeight - data->imageRect.size.y) * 0.5f -
						static_cast<float>(mapY * tileSize.y);
					const float x = (windowWidth + data->imageRect.size.x) * 0.5f;
					enemy->spr = Sprite("Res/Objects.png",glm::vec3(x,y,0),data->imageRect);
					enemy->spr.Animator(FrameAnimation::Animate::Create(data->tl));
					enemy->spr.Tweener(TweenAnimation::Animate::Create(data->createTween()));
					enemy->collisionShape = data->collisionShape;
					enemy->health = data->health;
					enemy->timer = 0;
					enemy->id = data->id;
					//ボスのポインタを設定する
					if (data->id == tileId_Boss && boss == nullptr)
					{
						boss = enemy;
					}
				}
			}
			else if (tileId == tileId_PowerUpItem)
			{
				//アイテムを出現させる
				Actor* item = findAvailableActor(
					std::begin(itemList), std::end(itemList));
				if (item != nullptr)
				{
					const float y =
						windowHeight * 0.5f - (float)(mapY * tileSize.x);
					item->spr = Sprite("Res/Objects.png", glm::vec3(0.5f * windowWidth, y, 0),
						Rect(160, 32, 32, 32));
					namespace TA = TweenAnimation;
					item->spr.Tweener(
						TA::Animate::Create(TA::MoveBy::Create(16, glm::vec3(-1000, 0, 0))));
					item->collisionShape = Rect(-16, -16, 32, 32);
					item->health = 1;
				}
			}
		}
	}
}

/**
* 敵の状態を更新する
*
* @param deltaTime 前回の更新からの経過時間
*/
void updateEnemies(float deltaTime)
{
	const glm::vec3 playerPos = sprPlayer.spr.Position();
	for (Actor* i = std::begin(enemyList); i != std::end(enemyList); i++)
	{
		if (i->health <= 0)
		{
			continue;
		}
		//一定時間ごとに弾を発射
		const float shotInterval = 2.0f;
		i->timer += deltaTime;
		if (i->timer < shotInterval)
		{
			continue;
		}
		i->timer -= shotInterval;

		//大型の敵の攻撃
		if (i->id == tileId_LargeEnemy)
		{
			for (int n = 0; n < 2; n++)
			{
				Actor* bullet = findAvailableActor(
					std::begin(enemyBulletList), std::end(enemyBulletList));
				if (bullet == nullptr)
				{
					continue;
				}
				glm::vec3 pos = i->spr.Position();
				pos.y += (float)(16 - n * 32);
				bullet->spr =
					Sprite("Res/Objects.png", pos, Rect(464, 0, 16, 16));
				namespace TA = TweenAnimation;
				bullet->spr.Tweener(TA::Animate::Create(
					TA::MoveBy::Create(4, glm::vec3(-1200, 0, 0))));
				bullet->spr.Scale(glm::vec2(3, 1));
				bullet->collisionShape = Rect(-12, -4, 24, 8);
				bullet->health = 1;
			}
			continue;
		}
		//画面外にいるときは発射しない
		const glm::vec3 enemyPos = i->spr.Position();
		if (enemyPos.x <= windowWidth * -0.5f || enemyPos.x > windowWidth * 0.5f)
		{
			continue;
		}
		if (enemyPos.y <= windowHeight * -0.5f || enemyPos.y > windowHeight * 0.5f)
		{
			continue;
		}
		//空いている敵の弾を検索
		Actor* bullet = findAvailableActor(
			std::begin(enemyBulletList), std::end(enemyBulletList));
		if (bullet == nullptr)
		{
			continue;
		}
		//自機の方向を計算
		const glm::vec3 distance = playerPos - i->spr.Position();
		const float radian = std::atan2(distance.y, distance.x);
		const float c = std::cos(radian);
		const float s = std::sin(radian);

		//自機に向かって弾を発射
		bullet->spr =
			Sprite("Res/Objects.png", i->spr.Position(), Rect(464, 0, 16, 16));
		namespace TA = TweenAnimation;
		bullet->spr.Tweener(TA::Animate::Create(
			TA::MoveBy::Create(8, glm::vec3(1200.0f * c, 1200.0f * s, 0))));
		bullet->spr.Rotation(radian + 3.14f);
		bullet->collisionShape = Rect(4, -4, 8, 8);
		bullet->health = 1;
	}
}