/*
@file Main.cpp
*/
#include "MainScene.h"
#include "TitleScene.h"
#include "FirstHelpScene.h"
#include "GameClearScene.h"
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

const char windowTitle[] = "Shoting Game";//�^�C�g���o�[�ɕ\������镶��

std::mt19937 random;//�����𔭐�������ϐ�(�����G���W��)

SpriteRenderer renderer;//�X�v���C�g�`��p�ϐ�
FontRenderer fontRenderer;//�t�H���g�`��p�ϐ�

Sprite sprBackground;//�w�i�p�X�v���C�g
Sprite sprHelp;//��������p�̃X�v���C�g
Sprite sprESC;//���C����ʗp�̃X�v���C�g
Sprite sprKeypad;//�L�[�p�b�h�̃X�v���C�g
Sprite sprBackMain;//���C����ʑJ�ڗp�̃X�v���C�g
Sprite sprSkey;//S�L�[�p�̃X�v���C�g
Sprite sprSpace;//�X�y�[�X�p�̃X�v���C�g
Sprite sprUp;//�J�[�\���L�[��p�̃X�v���C�g
Sprite sprDown;//�J�[�\���L�[���p�̃X�v���C�g
Sprite sprLeft;//�J�[�\���L�[�E�p�̃X�v���C�g
Sprite sprRight;//�J�[�\���L�[���p�̃X�v���C�g
Sprite sprRise;//���C����ʂւ̃X�v���C�g
Sprite sprNumData[8];//8���̃X�v���C�g
Actor sprPlayer;//���@�p�X�v���C�g

Sprite sprHand;//��������̕����X�v���C�g
Sprite sprWepon;//����̐ؑւ̕����X�v���C�g
Sprite sprAttack;//�U���p�̕����X�v���C�g
Sprite sprCursors;//�ړ��p�̕����X�v���C�g
Sprite sprClear;//�X�e�[�W�N���A�p�̕����X�v���C�g
Sprite sprSos;//������ʂ֑����p�̕����X�v���C�g
Sprite sprScore;//�X�R�A�̕����p�X�v���C�g
Sprite sprNumber[10];//�X�R�A��0����9�܂ŗp�̕����X�v���C�g
Sprite sprStageNum;//�X�e�[�W�ԍ��p�̕����X�v���C�g
Sprite sprStart;//�Q�[���̊J�n���ɕ\�����镶���X�v���C�g

glm::vec3 playerVelocity;//���@�̈ړ����x
Actor playerBulletList[128];//���@�̒e�̃��X�g
Actor playerLaserList[3];//���@�̃��[�U�[�̃��X�g
Actor enemyList[128];//�G�̃��X�g
Actor effectList[128];//�����Ȃǂ̓�����ʗp�X�v���C�g�̃��X�g
Actor itemList[25];//�A�C�e���p�X�v���C�g�̃��X�g
Actor healItemList[15];//�񕜃A�C�e���p�̃��X�g
Actor enemyBulletList[900];//�G�̒e�̃��X�g

int score;//�v���C���[�̓��_
float timer;
float shotTimer;//�e�̎�t����
float waitTime = 2.0f;
const int weaponLevelMin = 1;//���@�̕��틭���̍Œ�i�K
const int weaponLevelMax = 5;//���@�̕��틭���̍ō��i�K
int weaponLevel;//���@�̕��틭���i�K

const float maxHealth = 50;//���@�̗̑͂̍ő�l

const int weaponTypeWideShot = 0;//�L�͈̓V���b�g
const int weaponTypeLaser = 1;//���[�U�[
int weaponType;//�I�𒆂̕���


//��������p�ϐ�
Audio::SoundPtr bgm;
Audio::SoundPtr sePlayerShot;
Audio::SoundPtr sePlayerLaser;
Audio::SoundPtr seBlast;
Audio::SoundPtr seBlastBoss;
Audio::SoundPtr sePowerUp;
Audio::SoundPtr seHeal;

//�G�̃A�j���[�V����
const FrameAnimation::KeyFrame enemyKeyFrames[] =
{
	{0.000f, glm::vec2(480,0), glm::vec2(32,32)},
	{0.125f, glm::vec2(480,96), glm::vec2(32,32)},
	{0.250f, glm::vec2(480,64), glm::vec2(32,32)},
	{0.375f, glm::vec2(480,32), glm::vec2(32,32)},
	{0.500f, glm::vec2(480,0), glm::vec2(32,32)},
};
FrameAnimation::TimelinePtr tlEnemy;

//���@�̃A�j���[�V����
const FrameAnimation::KeyFrame playerKeyFrames[] =
{
	{0.000f, glm::vec2(0,0), glm::vec2(64,32)},
	{0.125f, glm::vec2(0,32), glm::vec2(64,32)},
	{0.250f, glm::vec2(0,64), glm::vec2(64,32)},
	{0.375f, glm::vec2(0,0), glm::vec2(64, 32)},
};
FrameAnimation::TimelinePtr tlPlayer;

//�����A�j���[�V����
const FrameAnimation::KeyFrame blastKeyFrames[] =
{
	{0 / 60.0f, glm::vec2(416,0), glm::vec2(32,32)},
	{5 / 60.0f, glm::vec2(416,32), glm::vec2(32,32)},
	{10 / 60.0f, glm::vec2(416,64), glm::vec2(32,32)},
	{15 / 60.0f, glm::vec2(416,96), glm::vec2(32,32)},
	{20 / 60.0f, glm::vec2(416,96), glm::vec2(32,32)},
};
FrameAnimation::TimelinePtr tlBlast;

//�G�̏o���𐧌䂷�邽�߂̃f�[�^
TiledMap enemyMap;
float mapCurrentPosX;
float mapProcessedX;
bool isEndOfMap;//�}�b�v�̏I�[�ɓ��B������true

float enemyGenerationTimer;//���̓G���o������܂ł̎���(�P��:�b)

//�Q�[���̏��
int gameState;//���s���̏��ID
Actor* boss;//�{�XActor�̃|�C���^
bool isStagePassed;//�X�e�[�W���N���A���Ă����true

TitleScene titleScene;
FirstHelpScene firstHelpScene;
MainScene mainScene;
GameClearScene gameClearScene;
GameOverScene gameOverScene;


//�v���g�^�C�v�錾
void processInput(GLFWEW::WindowRef);
void update(GLFWEW::WindowRef);
void render(GLFWEW::WindowRef);
void playerBulletAndEnemyContactHandler(Actor* bullet, Actor* enemy);
void playerAndEnemyBulletContactHandler(Actor* player, Actor* bullet);
void playerLaserAndEnemyContactHandler(Actor* laser, Actor* enemy);
void playerAndEnemyContactHandler(Actor* player, Actor* enemy);
void playerAndItemContactHandler(Actor*, Actor*);
void playerAndHealItemContactHandler(Actor*, Actor*);
void stopPlayerLaser();
void generateObjectFromMap(float);
void updateEnemies(float);

/**
* ���C����ʗp�̍\���̂̏����ݒ���s��
*
* @param scene ���C����ʗp�\���̂̃|�C���^
*
* @retval true ����������
* @retval false ���������s
*/
bool initialize(MainScene* scene)
{
	sprBackground = Sprite("Res/stage_1.png");
	sprPlayer.spr =
		Sprite("Res/Objects.png", glm::vec3(0, 0, 0), Rect(0, 0, 64, 32));
	sprPlayer.collisionShape = Rect(-24, -8, 48, 16);
	waitTime = 2.0f;
	sprPlayer.health = 50;
	initializeActorList(std::begin(enemyList), std::end(enemyList));
	initializeActorList(std::begin(playerBulletList), std::end(playerBulletList));
	initializeActorList(std::begin(effectList), std::end(effectList));
	initializeActorList(std::begin(itemList), std::end(itemList));
	initializeActorList(std::begin(healItemList), std::end(healItemList));
	initializeActorList(std::begin(playerLaserList), std::end(playerLaserList));
	initializeActorList(std::begin(enemyBulletList), std::end(enemyBulletList));

	if (scene->stageNo == 1)
	{
		score = 0;
		weaponLevel = weaponLevelMin;
		weaponType = weaponTypeWideShot;
	}

	//�G�z�u�}�b�v��ǂݍ���
	enemyMap.Load("Res/EnemyMap.json");
	mapCurrentPosX = mapProcessedX = windowWidth;
	isEndOfMap = false;
	boss = nullptr;
	isStagePassed = false;

	//��������������
	Audio::EngineRef audio = Audio::Engine::Instance();
	seBlast = audio.Prepare("Res/Audio/Blast.xwm");
	seBlastBoss = audio.Prepare("Res/Audio/BlastBoss.mp3");
	sePlayerShot = audio.Prepare("Res/Audio/PlayerShot.xwm");
	sePlayerLaser = audio.Prepare("Res/Audio/Laser.xwm");
	sePowerUp = audio.Prepare("Res/Audio/GetItem.xwm");
	seHeal = audio.Prepare("Res/Audio/Heal.mp3");
	bgm = audio.Prepare("Res/Audio/Main.mp3");

	//BGM�����[�v���Đ�����
	bgm->Play(Audio::Flag_Loop);
	return true; 
}

/*
�v���O�����̃G���g���|�C���g
*/
int main()
{
	//�A�v���P�[�V�����̏�����
	GLFWEW::WindowRef window = GLFWEW::Window::Instance();
	if (!window.Initialize(windowWidth, windowHeight, windowTitle))
	{
		return 1;
	}
	//�����Đ��V�X�e���̏�����
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

	random.seed(std::random_device()());//�����G���W���̏�����

	//�A�j���[�V�����^�C�����C���̍쐬
	tlEnemy = FrameAnimation::Timeline::Create(enemyKeyFrames);//�G�^�C�����C���̍쐬
	tlPlayer = FrameAnimation::Timeline::Create(playerKeyFrames);//�v���C���[�^�C�����C���̍쐬
	tlBlast = FrameAnimation::Timeline::Create(blastKeyFrames);//�����^�C�����C���̍쐬
	
	initializeMapObjectDataList();

	//�^�C�g����ʂ�����������
	gameState = gameStateTitle;
	initialize(&titleScene);

	//�Q�[�����[�v
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
*�v���C���[�̓��͂���������
*
* @param window �Q�[�����J�n����E�B���h�E
*/
void processInput(GLFWEW::WindowRef window)
{
	//�Q�[���̏�Ԃ��^�C�g����ʂ̏ꍇ
	if (gameState == gameStateTitle)
	{
		processInput(window, &titleScene);
		return;
	}
	//�Q�[���̏�Ԃ����������ʂ̏ꍇ
	else if (gameState == gameStateHelp)
	{
		processInput(window, &firstHelpScene);
		return;
	}
	//�Q�[���̏�Ԃ��N���A��ʂ̏ꍇ
	else if (gameState == gameStateClear)
	{
		processInput(window, &gameClearScene);
		return;
	}
	//�Q�[���̏�Ԃ��Q�[���I�[�o�[��ʂ̏ꍇ
	else if (gameState == gameStateGameOver)
	{
		processInput(window, &gameOverScene);
		return;
	}
	window.Update();

	//���@�̗̑͂�0�ȉ��̏ꍇ
	if (sprPlayer.health <= 0)
	{
		playerVelocity = glm::vec3(0, 0, 0);
		stopPlayerLaser();
		shotTimer = 0;
	}
	else
	{
		//���@�̑��x��ݒ肷��
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
		if (gamepad.buttons&GamePad::DPAD_RIGHT)
		{
			playerVelocity.x = 1;
		}
		else if (gamepad.buttons&GamePad::DPAD_LEFT)
		{
			playerVelocity.x = -1;
		}
		else
		{
			playerVelocity.x = 0;
		}
		if (playerVelocity.x || playerVelocity.y)
		{
			playerVelocity = glm::normalize(playerVelocity) * 400.0f;//�x�N�g���̒����𐳋K��
		}

		//�G�X�L�[���������畐���؂�ւ���
		if (gamepad.buttonDown&GamePad::B)
		{
			//���킪���C�h�V���b�g�̏ꍇ
			if (weaponType == weaponTypeWideShot)
			{
				weaponType = weaponTypeLaser;
			}
			//���킪���[�U�[�̏ꍇ
			else
			{
				weaponType = weaponTypeWideShot;
			}
		}

		//���킪���C�h�V���b�g�ł���
		//�X�y�[�X�L�[�𒷉������Ă��e�̑҂����Ԃ�0�ȉ��Ȃ�
		if (weaponType == weaponTypeWideShot &&
			(gamepad.buttons&GamePad::A && shotTimer <= 0))
		{
			//���C�h�V���b�g�̃��x�����ő�ɂȂ�܂ŌJ��Ԃ�
			for (int i = 0; i < weaponLevel; i++)
			{
				//�󂢂Ă���e�̍\���̂̊֐��Ăяo��
				Actor* bullet = findAvailableActor(
					std::begin(playerBulletList), std::end(playerBulletList));

				//�󂢂Ă���\���̂�����������A������g���Ēe�𔭎˂���
				if (bullet->health <= 0)
				{
					bullet->spr = Sprite("Res/Objects.png",
						sprPlayer.spr.Position(), Rect(64, 0, 32, 16));
					shotTimer = 1.0f / 10.0f;
					const float angles[] = { 0,7.5f,-7.5f,15.0f,-15.0f };
					const float radian = angles[i] / 180.0f * 3.14f;
					const float c = std::cos(radian);
					const float s = std::sin(radian);
					bullet->spr.Tweener(TweenAnimation::Animate::Create(
						TweenAnimation::MoveBy::Create(1, glm::vec3(1200 * c, 1200 * s, 0),
							TweenAnimation::EasingType::Linear)));
					bullet->spr.Rotation(radian);
					bullet->collisionShape = Rect(-16, -8, 32, 16);
					bullet->health = 0.5f;
				}
			}
			sePlayerShot->Play();//�e�̔��ˉ����Đ�
		}
		//���킪��-�U�[�ŃX�y�[�X�L�[���������ꍇ
		if ((weaponType == weaponTypeLaser) && (gamepad.buttons & GamePad::A))
		{
			if (playerLaserList[0].health <= 0)
			{
				const glm::vec3 posFiringPoint = sprPlayer.spr.Position();
				playerLaserList[0].spr = Sprite("Res/Objects.png", posFiringPoint,
					Rect(96, 0, 32, 16));

				playerLaserList[0].collisionShape = Rect(-8, -4, 16, 8);
				playerLaserList[0].health = 0.01f;
				playerLaserList[1].spr = Sprite("Res/Objects.png", posFiringPoint,
					Rect(112, 0, 32, 16));
				playerLaserList[1].health = 0.01f;
				playerLaserList[1].collisionShape = Rect(-8, -4, 16, 8);
				playerLaserList[2].spr = Sprite("Res/Objects.png", posFiringPoint,
					Rect(128, 0, 32, 16));
				playerLaserList[2].collisionShape = Rect(-8, -4, 16, 8);
				playerLaserList[2].health = 0.08f;
				sePlayerLaser->Play(Audio::Flag_Loop);
			}
		}
		else
		{
			stopPlayerLaser();
		}
		//�Q�[���̏�Ԃ����C����ʂł��s�[�L�[������������
		if ((gameState == gameStateMain) && (gamepad.buttonDown & GamePad::X))
		{
			gameState = gameStatePose;
			
		}
		//�Q�[���̏�Ԃ����������ʂł��G�X�P�[�v�L�[������������
		else if ((gameState == gameStatePose) && (gamepad.buttonDown & GamePad::ESC))
		{
			gameState = gameStateMain;
		}
	}
}

/**
*�Q�[���̏�Ԃ��X�V����
*
* @param window �Q�[�����Ǘ�����E�B���h�E
*/
void update(GLFWEW::WindowRef window)
{
	const float deltaTime = window.DeltaTime();//�O��̍X�V����̌o�ߎ���(�b)

	//�Q�[���̏�Ԃ��^�C�g����ʂ̏ꍇ
	if (gameState == gameStateTitle)
	{
		update(window, &titleScene);
		return;
	}
	//�Q�[���̏�Ԃ����������ʂ̏ꍇ
	else if (gameState == gameStateHelp)
	{
		update(window, &firstHelpScene);
		return;
	}
	//�Q�[���N���A�̏ꍇ
	else if (gameState == gameStateClear)
	{
		update(window, &gameClearScene);
		return;
	}

	//�Q�[���I�[�o�[�̏ꍇ
	else if (gameState == gameStateGameOver)
	{
		update(window, &gameOverScene);
		return;
	}
	//�Q�[���̏�Ԃ����C����ʂ̏ꍇ
	else if (gameState == gameStateMain)
	{
		//�X�v���C�g��ݒ肷��
		sprSos = Sprite("Res/Sos.png", glm::vec3(0, 240, 0));
		sprScore = Sprite("Res/Score.png", glm::vec3(-90, 280, 0));

		//�����X�v���C�g��ݒ肷��
		sprStageNum = Sprite("Res/StageNo.png", glm::vec3(-350, 280, 0));

		//�X�v���C�g�̍��W�𔽉f����
		sprSos.Update(deltaTime);
		sprScore.Update(deltaTime);

		//�����X�v���C�g�̍��W�𔽉f����
		sprStageNum.Update(deltaTime);

		// �w�i�̍��W���擾����
		glm::vec3 wallMove = sprBackground.Position();

		// �擾�������W����-X������3�h�b�g������
		wallMove.x = wallMove.x - 0.03;

		// �w�i�X�v���C�g�̍��W�ɁA�擾�������W��ݒ肷��
		sprBackground.Position(wallMove);

		//�ݒ肵�����W�𔽉f����
		sprBackground.Update(deltaTime);

		//���@�̈ړ�
		if (sprPlayer.health > 0)
		{
			if (playerVelocity.x || playerVelocity.y)
			{
				glm::vec3 newPos = sprPlayer.spr.Position();
				newPos = newPos + playerVelocity * deltaTime;

				//���@�̈ړ��͈͂���ʓ��ɐ�������
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

			//�e�̐���
			if (shotTimer > 0)
			{
				shotTimer -= deltaTime;
			}

			//���[�U�[�̈ړ�
			if (playerLaserList[0].health > 0)
			{
				//���[�U�[(����)�̈ړ�
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

				//���[�U�[(����)�̈ړ�
				const float halfHeadSize = playerLaserList[2].spr.Rectangle().size.x * 0.5f;
				const float halfTailSize = playerLaserList[0].spr.Rectangle().size.x * 0.5f;
				const float bodySize = playerLaserList[1].spr.Rectangle().size.x;
				const float bodyLength =
					posHead.x - posFiringPoint.x - halfHeadSize - halfTailSize;
				glm::vec3 posBody = playerLaserList[1].spr.Position();
				posBody.x = posFiringPoint.x + (posHead.x - posFiringPoint.x) *0.5f;
				posBody.y = posFiringPoint.y;
				playerLaserList[1].spr.Position(posBody);
				playerLaserList[1].spr.Scale(glm::vec2(bodyLength / bodySize, 1));
				playerLaserList[1].collisionShape =
					Rect(-bodyLength * 0.5f, -4, bodyLength, 8);
				//���[�U�[(����)�̈ړ�
				playerLaserList[0].spr.Position(posFiringPoint);
			}
			//�G��A�C�e�������o��������
			generateObjectFromMap(deltaTime);

			//Actor�z��̍X�V�̊֐��Ăяo��
			updateEnemies(deltaTime);
			updateActorList(std::begin(enemyList), std::end(enemyList), deltaTime);
			updateActorList(std::begin(playerBulletList), std::end(playerBulletList), deltaTime);
			updateActorList(std::begin(effectList), std::end(effectList), deltaTime);
			updateActorList(std::begin(itemList), std::end(itemList), deltaTime);
			updateActorList(std::begin(healItemList), std::end(healItemList), deltaTime);
			updateActorList(std::begin(playerLaserList), std::end(playerLaserList), deltaTime);
			updateActorList(std::begin(enemyBulletList), std::end(enemyBulletList), deltaTime);


			//���@�̒e�ƓG�̏Փ˔���
			detectCollision(std::begin(playerBulletList), std::end(playerBulletList),
				std::begin(enemyList), std::end(enemyList),
				playerBulletAndEnemyContactHandler);

			//���@�ƓG�̒e�̏Փ˔���
			detectCollision(
				&sprPlayer, &sprPlayer + 1,
				std::begin(enemyBulletList), std::end(enemyBulletList),
				playerAndEnemyBulletContactHandler);

			//���@�̃��[�U�[�ƓG�̏Փ˔���
			detectCollision(
				std::begin(playerLaserList), std::end(playerLaserList),
				std::begin(enemyList), std::end(enemyList),
				playerLaserAndEnemyContactHandler);

			//���@�ƓG�̏Փ˔���
			detectCollision(
				&sprPlayer, &sprPlayer + 1,
				std::begin(enemyList), std::end(enemyList),
				playerAndEnemyContactHandler);

			//���@�ƕ��틭���A�C�e���̏Փ˔���
			detectCollision(
				&sprPlayer, &sprPlayer + 1,
				std::begin(itemList), std::end(itemList),
				playerAndItemContactHandler);

			//���@�Ɖ񕜃A�C�e���̏Փ˔���
			detectCollision(
				&sprPlayer, &sprPlayer + 1,
				std::begin(healItemList), std::end(healItemList),
				playerAndHealItemContactHandler);
		}

		//���@���j�󂳂��3�b��������Q�[���I�[�o�[��ʂ�\������
		// * waitTimer��3��ݒ�
		// * ���@���j�󂳂�Ă�����
		//   * waitTimer����deltaTime������
		//   * waitTimer��0�ȉ��ɂȂ�����
		//     * �Q�[���I�[�o�[��ʂɈڍs����
		else if (sprPlayer.health <= 0)
		{
			sprPlayer.health = 0;
			waitTime -= deltaTime;

			if (waitTime <= 0)
			{
				bgm->Stop();//BGM���~����
				gameState = gameStateGameOver;
				initialize(&gameOverScene);
			}
			return;
		}
	}
	else if (gameState == gameStatePose)
	{
	stopPlayerLaser();
	//���[�U�[���~�߂�
	stopPlayerLaser();
	//�X�v���C�g��ݒ肷��
	sprHelp = Sprite("Res/Pose.png");
	sprKeypad = Sprite("Res/Keypad.png", glm::vec3(0, 20, 0));
	sprUp = Sprite("Res/Cursors.png", glm::vec3(263, -60, 0));
	sprLeft = Sprite("Res/Cursors.png", glm::vec3(225, -100, 0));
	sprRight = Sprite("Res/Cursors.png", glm::vec3(302, -102, 0));
	sprDown = Sprite("Res/Down.png", glm::vec3(260, -125, 0));
	sprRise = Sprite("Res/Rise.png", glm::vec3(-280, 90, 0));
	sprSkey = Sprite("Res/Skey.png", glm::vec3(-157, -30, 0));
	sprSpace = Sprite("Res/Space.png", glm::vec3(-25, -105, 0));

	//�����X�v���C�g��ݒ肷��
	
	sprHand = Sprite("Res/Hand.png", glm::vec3(0, 260, 0));
	sprBackMain = Sprite("Res/BackMain.png", glm::vec3(-250, 220, 0));
	sprWepon = Sprite("Res/Wepon.png", glm::vec3(-240, -180, 0));
	sprAttack = Sprite("Res/Attack.png", glm::vec3(60, -210, 0));
	sprCursors = Sprite("Res/Move.png", glm::vec3(250, -170, 0));

	//�X�v���C�g�̍��W�𔽉f����
	sprHelp.Update(deltaTime);
	sprKeypad.Update(deltaTime);
	sprUp.Update(deltaTime);
	sprLeft.Update(deltaTime);
	sprRight.Update(deltaTime);
	sprDown.Update(deltaTime);
	sprRise.Update(deltaTime);
	sprSkey.Update(deltaTime);
	sprSpace.Update(deltaTime);

	//�����X�v���C�g�̍��W�𔽉f����
	sprHand.Update(deltaTime);
	sprBackMain.Update(deltaTime);
	sprWepon.Update(deltaTime);
	sprAttack.Update(deltaTime);
	sprCursors.Update(deltaTime);
	}

	//�X�e�[�W�N���A����
	if (!isStagePassed && boss && (boss->health <= 0))
	{
		isStagePassed = true;
		boss = nullptr;
		timer = 2;
	}
	if (isStagePassed)
	{
		timer -= deltaTime;
		waitTime -= deltaTime;
		if (timer <= 0)
		{
			if (waitTime <= 0)
			{

				bgm->Stop();//BGM���~����
				stopPlayerLaser();
				gameState = gameStateClear;
				initialize(&gameClearScene);
				return;
			}
		}
	}
}

/**
*�Q�[���̏�Ԃ�`�悷��
*
* @param window �Q�[�����Ǘ�����E�B���h�E
*/
void render(GLFWEW::WindowRef window)
{
	//�Q�[���̏�Ԃ��^�C�g����ʂ̏ꍇ
	if (gameState == gameStateTitle)
	{
		render(window, &titleScene);
		return;
	}
	//�Q�[���̏�Ԃ����������ʂ̏ꍇ
	else if (gameState == gameStateHelp)
	{
		render(window, &firstHelpScene);
		return;
	}
	//�Q�[���̏�Ԃ��N���A��ʂ̏ꍇ
	else if (gameState == gameStateClear)
	{
		render(window, &gameClearScene);
		return;
	}
	//�Q�[���̏�Ԃ��Q�[���I�[�o�[��ʂ̏ꍇ
	else if (gameState == gameStateGameOver)
	{
		render(window, &gameOverScene);
		return;
	}
	renderer.BeginUpdate();//���_�f�[�^�̍쐬���J�n����

	renderer.AddVertices(sprBackground);//���C����ʂ̃X�v���C�g���X�V����

	//�Q�[���̏�Ԃ����C����ʂ̏ꍇ
	if (gameState == gameStateMain)
	{
		//���@�̗̑͂�0���傫���ꍇ
		if ((sprPlayer.health > 0))
		{
			//���@�X�v���C�g�̒��_�f�[�^��ݒ肷��
			renderer.AddVertices(sprPlayer.spr);
		}
		//�X�v���C�g�̒��_�f�[�^��ݒ肷��
		renderer.AddVertices(sprSos);
		renderer.AddVertices(sprScore);

		//�����X�v���C�g�̒��_�f�[�^��ݒ肷��
		renderer.AddVertices(sprStageNum);

		//�A�N�^�[�z���ݒ肷��
		renderActorList(std::begin(enemyList), std::end(enemyList));
		renderActorList(std::begin(playerBulletList), std::end(playerBulletList));
		renderActorList(std::begin(effectList), std::end(effectList));
		renderActorList(std::begin(itemList), std::end(itemList));
		renderActorList(std::begin(healItemList), std::end(healItemList));
		renderActorList(std::begin(playerLaserList), std::end(playerLaserList));
		renderActorList(std::begin(enemyBulletList), std::end(enemyBulletList));
	}
	//�Q�[���̏�Ԃ����������ʂ̏ꍇ
	else if (gameState == gameStatePose)
	{
		//�X�v���C�g�̒��_�f�[�^��ݒ肷��
		renderer.AddVertices(sprHelp);
		renderer.AddVertices(sprKeypad);
		renderer.AddVertices(sprUp);
		renderer.AddVertices(sprLeft);
		renderer.AddVertices(sprRight);
		renderer.AddVertices(sprDown);
		renderer.AddVertices(sprRise);
		renderer.AddVertices(sprSkey);
		renderer.AddVertices(sprSpace);

		//�����X�v���C�g�̒��_�f�[�^��ݒ肷��
		renderer.AddVertices(sprHand);
		renderer.AddVertices(sprBackMain);
		renderer.AddVertices(sprWepon);
		renderer.AddVertices(sprAttack);
		renderer.AddVertices(sprCursors);
	}
	renderer.EndUpdate();//���_�f�[�^�̍쐬���I������
	renderer.Draw({ windowWidth,windowHeight });//�X�v���C�g�ƕ����X�v���C�g��`�悷��
	
	//�t�H���g��`�悷�鎞�ɃQ�[���̏�Ԃ����C����ʂ̏ꍇ
	if (gameState == gameStateMain)
	{
		fontRenderer.BeginUpdate();//�����f�[�^�̍쐬���J�n����
		char str[64];//�ݒ肵��������
		snprintf(str, sizeof(str), "%08d", score);//���l�𕶎���ɕϊ�����
		fontRenderer.Scale(glm::vec2(1.2f));//�������g�傷��
		fontRenderer.AddString(glm::vec2(-20, 310), str);//������ǉ�����

		snprintf(str, sizeof(str), "�̗�:%0.0f/%0.0f", sprPlayer.health, 50.0f);
		fontRenderer.Scale(glm::vec2(0.7f, 0.7f));
		fontRenderer.AddString(glm::vec2(210, 280), str);
		fontRenderer.EndUpdate();//�����f�[�^�̍쐬���I������
		fontRenderer.Draw();//�ǉ�����������\������
	}
	window.SwapBuffers();
}

/**
* �Փ˂����o����
*
* @param fristA �Փ˂�����z��A�̐擪�|�C���^
* @param lastA �Փ˂�����z��A�̏I�[�|�C���^
* @param fristB �Փ˂�����z��B�̐擪�|�C���^
* @param lastB �Փ˂�����z��B�̏I�[�|�C���^
* @param handler A-B�ԂŏՓ˂����o���ꂽ�Ƃ��Ɏ��s����֐�
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
* ���@�̒e�ƓG�̏Փ˂���������
*
* @param bullet ���@�̒e�̃|�C���^
* @param enemy �G�̃|�C���^
*/
void playerBulletAndEnemyContactHandler(Actor* bullet, Actor* enemy)
{
	float health;
	health = enemy->health;
	enemy->health -= bullet->health;
	bullet->health -= health;

	if (enemy->health <= 0)
	{
		if (score <= 0)
		{
			score = 0;
		}
			score += 100;//�G��j�󂵂��瓾�_�𑝂₷
		Actor* blast = findAvailableActor(
			std::begin(effectList), std::end(effectList));

		if (blast != nullptr) { //�����A�N�^�[�̋󂫂�����������A{
			if (enemy->id == tileId_LargeEnemy) { //  ��^�̓G�̂Ƃ��́A{
				//    �傫��������\������.
			}
			else { //  }����ȊO�́A{
				//    ���ʂ̔�����\������
			}
			//  ���������Đ�.
		} //} �����A�N�^�[�󂫏����I��

		//�����A�N�^�[�̋󂫂�����������A
		//  ���ʂ̔�����\������.
		//  ��^�̓G�̂Ƃ��́A{
		//    �傫��������\������.
		//  }����ȊO�́A{
		//    ���������Đ�.
		//  }
		//�����A�N�^�[�󂫏����I��

		if (blast != nullptr)
		{
			if (enemy->id == tileId_Boss)
			{
				//������3�\������
				for (int j = 0; j < 3; j++)
				{
					Actor* blast = findAvailableActor(
						std::begin(effectList), std::end(effectList));
					if (blast != nullptr)
					{
						blast->spr = Sprite("Res/Objects.png", enemy->spr.Position() + glm::vec3(0, 96 * j , 0));
						blast->spr.Scale(glm::vec2(10));
						blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
						namespace TA = TweenAnimation;
						blast->spr.Tweener(TA::Animate::Create(
							TA::Rotation::Create(20 / 60.0f, 1.5f)));
						blast->health = 1;
						seBlastBoss->Play();
					}
				}
			}
			else
			{
				Actor*  blast = findAvailableActor(
					std::begin(effectList), std::end(effectList));
				if (blast != nullptr)
				{
					blast->spr = Sprite("Res/Objects.png", enemy->spr.Position());
					blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
					namespace TA = TweenAnimation;
					blast->spr.Tweener(TA::Animate::Create(
						TA::Rotation::Create(20 / 60.0f, 1.5f)));
					blast->health = 1;
				}
				if (enemy->id == tileId_LargeEnemy)
				{
					blast->spr.Scale(glm::vec2(4));
				}
				else
				{
					blast->spr.Scale(glm::vec2(2));
				}
				seBlast->Play();
			}
		}
	}
}

/**
* ���@�ƓG�̒e�̏Փ˂���������
*
* @param bullet ���@�̒e�̃|�C���^
* @param player �G�̃|�C���^
*/
void playerAndEnemyBulletContactHandler(Actor* player, Actor* bullet)
{
	float health;
	health = bullet->health;
	bullet->health -= player->health;
	player->health -= health;

	if (player->health >= 0)
	{
		//�X�R�A��0�_�ȏ�̏ꍇ
		if (score > 0)
		{
			score -= 50;
		}
		//�X�R�A��0�_�ȉ��̏ꍇ
		else if (score <= 0)
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
			seBlast->Play();//���������Đ�
		}
		else if (player->health <= 0)
		{
			Actor*  blast = findAvailableActor(
				std::begin(effectList), std::end(effectList));
			for (int k = 0; k < 2; k++)
			{
				if (blast != nullptr)
				{
					blast->spr = Sprite("Res/Objects.png", bullet->spr.Position() + glm::vec3(32 * k, 0, 0));
					blast->spr.Scale(glm::vec2(10));
					blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
					namespace TA = TweenAnimation;
					blast->spr.Tweener(TA::Animate::Create(
						TA::Rotation::Create(20 / 60.0f, 1.5f)));
					blast->health = 1;
					seBlastBoss->Play();
				}
			}
		}
		weaponLevel--;
		//���@�̃��x�����㉻������
		if (weaponLevel < weaponLevelMin)
		{
			weaponLevel = weaponLevelMin;
		}
	}
}

/**
* ���@�̃��[�U�[�ƓG�̏Փ˔���
*
* @param laser ���@�̃��[�U�[�̃|�C���^
* @param enemy �G�̃|�C���^
*/
void playerLaserAndEnemyContactHandler(Actor* laser, Actor* enemy)
{
	enemy->health -= laser->health;

	if (enemy->health <= 0)
	{
		score += 30;
		Actor* blast = findAvailableActor(
			std::begin(effectList), std::end(effectList));
		
		if (blast != nullptr)
		{
			if (enemy->id == tileId_Boss)
			{
				//������3�\������
				for (int j = 0; j < 3; j++)
				{
					Actor* blast = findAvailableActor(
						std::begin(effectList), std::end(effectList));
					if (blast != nullptr)
					{
						blast->spr = Sprite("Res/Objects.png", enemy->spr.Position() + glm::vec3(0, 96 * j, 0));
						blast->spr.Scale(glm::vec2(10));
						blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
						namespace TA = TweenAnimation;
						blast->spr.Tweener(TA::Animate::Create(
							TA::Rotation::Create(20 / 60.0f, 1.5f)));
						blast->health = 1;
						seBlastBoss->Play();
					}
				}
			}
			else
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
				}
				if (enemy->id == tileId_LargeEnemy)
				{
					blast->spr.Scale(glm::vec2(4));
				}
				else
				{
					blast->spr.Scale(glm::vec2(2));
				}
				seBlast->Play();//���������Đ�
			}
		}
	}
}

/**
* ���@�ƓG�̏Փ˂���������
*
* @param bullet ���@�̃|�C���^
* @param enemy �G�̃|�C���^
*/
void playerAndEnemyContactHandler(Actor* player, Actor* enemy)
{
	const int tmp = player->health;
	player->health -= enemy->health;
	enemy->health -= tmp;

	if (enemy->health <= 0)
	{
		//�X�R�A��0�_���傫���ꍇ
		if (score > 0)
		{
			score -= 10;
		}
		//�X�R�A��0�_�ȉ��̏ꍇ
		else if (score <= 0)
		{
			score = 0;
		}
		Actor* blast = findAvailableActor(
			std::begin(effectList), std::end(effectList));
		blast->spr = Sprite("Res/Objects.png", enemy->spr.Position());
		blast->spr.Animator(FrameAnimation::Animate::Create(tlBlast));
		namespace TA = TweenAnimation;
		blast->spr.Tweener(TA::Animate::Create(
			TA::Rotation::Create(20 / 60.0f, 1.5f)));
		blast->health = 1;

		if (blast != nullptr)
		{
			if (enemy->id == tileId_LargeEnemy)
			{
				blast->spr.Scale(glm::vec2(4));
			}
			else
			{
				blast->spr.Scale(glm::vec2(2));
			}
			seBlast->Play();
		}
		//���@�̕�����㉻������
		weaponLevel--;
		if (weaponLevel < weaponLevelMin)
		{
			weaponLevel = weaponLevelMin;
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
				blast->spr.Scale(glm::vec2(2, 2));//�����̑傫��
				blast->health = 1;
				seBlast->Play();//���������Đ�
			}
			//���@�̕�����㉻������
			weaponLevel--;
			if (weaponLevel < weaponLevelMin)
			{
				weaponLevel = weaponLevelMin;
			}
		}
}

/**
* ���@�ƃA�C�e���̏Փ˂���������
*
* @param player ���@�̃|�C���^
* @param item �A�C�e���̃|�C���^
*/
void playerAndItemContactHandler(Actor* player, Actor* item)
{
	item->health = 0;
	sePowerUp->Play();

	//���@�̕������������
	weaponLevel++;
	if (weaponLevel > weaponLevelMax)
	{
		weaponLevel = weaponLevelMax;
	}

}

/**
*���@�Ɖ񕜃A�C�e���̏Փ˂���������
*
* @param player ���@�̃|�C���^
* @param item �A�C�e���̃|�C���^
*/
void playerAndHealItemContactHandler(Actor* player, Actor* item)
{
	player->health += 10;
	item->health = 0;
	seHeal->SetVolume(2);
	seHeal->Play();

	//�񕜂̏����ݒ�
	if (player->health > maxHealth)
	{
		player->health = maxHealth;
	}
}

/**
*���[�U�[���~�߂鏈�����쐬����
*/
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
* �}�b�v�f�[�^�����ēG��A�C�e�����o��������
*
* @param deltaTime �O��̍X�V����̌o�ߎ���
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

	//�G�z�u�}�b�v�Q�ƈʒu�̍X�V
	const float enemyMapScrollSpeed = 100;//�X�V���x
	mapCurrentPosX += enemyMapScrollSpeed * deltaTime;
	if (mapCurrentPosX >= tiledMapLayer.size.x * tileSize.x)
	{
		//�I�[�𒴂������~
		isEndOfMap = true;
		return;
	}
	//���̗�ɓ��B������f�[�^��ǂ�
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

				//�󂢂Ă���\���̂�����������A������g���ēG���o��������
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
					//�{�X�̃|�C���^��ݒ肷��
					if (data->id == tileId_Boss && boss == nullptr)
					{
						boss = enemy;
					}
				}
			}
			else if (tileId == tileId_PowerUpItem)
			{
				//���틭���A�C�e�����o��������
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
			else if (tileId == tileId_HealItem)
			{
				//�񕜃A�C�e�����o��������
				Actor* healItem = findAvailableActor(
					std::begin(healItemList), std::end(healItemList));
				if (healItem != nullptr)
				{
					const float sy =
						windowHeight * 0.5f - (float)(mapY * tileSize.x);
					healItem->spr = Sprite("Res/Objects.png", glm::vec3(0.5f * windowWidth, sy, 0),
						Rect(96, 32, 32, 32));
					namespace TA = TweenAnimation;
					healItem->spr.Tweener(
						TA::Animate::Create(TA::MoveBy::Create(16, glm::vec3(-1000, 0, 0))));
					healItem->collisionShape = Rect(-16, -16, 32, 32);
					healItem->health = 1;
				}
			}
		}
	}
}

/**
* �G�̏�Ԃ��X�V����
*
* @param deltaTime �O��̍X�V����̌o�ߎ���
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
		//��莞�Ԃ��Ƃɒe�𔭎�
		const float shotInterval = 3.0f;
		i->timer += deltaTime;
		if (i->timer < shotInterval)
		{
			continue;
		}
		i->timer -= shotInterval;

		//���^�̓G�̍U��
		if (i->id == tileId_SmallEnemy)
		{
			//���@�̕������v�Z
			const glm::vec3 distance = playerPos - i->spr.Position();
			const float radian = std::atan2(distance.y, distance.x);
			const float c = std::cos(radian);
			const float s = std::sin(radian);

			//��ʊO�ɂ���Ƃ��͔��˂��Ȃ�
			const glm::vec3 enemyPos = i->spr.Position();
			if (enemyPos.x <= windowWidth * -0.5f || enemyPos.x > windowWidth * 0.5f)
			{
				continue;
			}
			if (enemyPos.y <= windowHeight * -0.5f || enemyPos.y > windowHeight * 0.5f)
			{
				continue;
			}
			//�󂢂Ă���G�̒e������
			Actor* bullet = findAvailableActor(
				std::begin(enemyBulletList), std::end(enemyBulletList));
			if (bullet == nullptr)
			{
				continue;
			}
			//���@�Ɍ������Ēe�𔭎�
			Actor* smallBullet = findAvailableActor(
				std::begin(enemyBulletList), std::end(enemyBulletList));
			smallBullet->spr =
				Sprite("Res/Objects.png", enemyPos, Rect(464, 0, 16, 16));
			namespace TA = TweenAnimation;
			smallBullet->spr.Tweener(TA::Animate::Create(
				TA::MoveBy::Create(8, glm::vec3(1200.0f * c, 1200.0f * s, 0))));
			smallBullet->spr.Rotation(radian + 3.14f);
			smallBullet->collisionShape = Rect(-4, -4, 8, 8);
			smallBullet->health = 3;
		}

		//��^�̓G�̍U��
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
				bullet->health = 5;
			}
			continue;
		}
		//�{�X�̍U��
		if (i->id == tileId_Boss)
		{
			{
				//�΂̒e���ˏo���鏈�����쐬����
				for (int j = 1; j < 15; j++)
				{
					Actor* bossBullet = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBullet != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position();
						bossBullet->spr =
							Sprite("Res/Objects.png", bossPos, Rect(448, 0, 16, 16));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(0, 0, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * j));
						seq->Add(TA::MoveBy::Create(
							3.0f , glm::vec3(-1000, 800, 0), TA::EasingType::Linear));
						bossBullet->spr.Tweener(TA::Animate::Create(seq));
						bossBullet->collisionShape = Rect(-4, -4, 8, 8);
						bossBullet->health = 1;
					}
				}
			}
			{
				//�΂̒e���ˏo���鏈�����쐬����
				for (int j = 1; j < 16; j++)
				{
					Actor* bossBullet = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBullet != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position();
						bossBullet->spr =
							Sprite("Res/Objects.png", bossPos, Rect(448, 0, 16, 16));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(0, -20, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * j));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-1000, 600, 0), TA::EasingType::Linear));
						bossBullet->spr.Tweener(TA::Animate::Create(seq));
						bossBullet->collisionShape = Rect(-4, -4, 8, 8);
						bossBullet->health = 1;
					}
				}
			}
			{
				//�΂̒e���ˏo���鏈�����쐬����
				for (int j = 1; j < 16; j++)
				{
					Actor* bossBullet = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBullet != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position();
						bossBullet->spr =
							Sprite("Res/Objects.png", bossPos, Rect(448, 0, 16, 16));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(0, -40, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * j));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-1000, 400, 0), TA::EasingType::Linear));
						bossBullet->spr.Tweener(TA::Animate::Create(seq));
						bossBullet->collisionShape = Rect(-4, -4, 8, 8);
						bossBullet->health = 1;
					}
				}
			}
			{
				//�΂̒e���ˏo���鏈�����쐬����
				for (int j = 1; j < 16; j++)
				{
					Actor* bossBullet = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBullet != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position();
						bossBullet->spr =
							Sprite("Res/Objects.png", bossPos, Rect(448, 0, 16, 16));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(0, -60, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * j));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-1000, 200, 0), TA::EasingType::Linear));
						bossBullet->spr.Tweener(TA::Animate::Create(seq));
						bossBullet->collisionShape = Rect(-4, -4, 8, 8);
						bossBullet->health = 1;
					}
				}
			}
			{
				//�΂̒e���ˏo���鏈�����쐬����
				for (int j = 1; j < 16; j++)
				{
					Actor* bossBullet = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBullet != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position();
						bossBullet->spr =
							Sprite("Res/Objects.png", bossPos, Rect(448, 0, 16, 16));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(0, -80, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * j));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-1000, 0, 0), TA::EasingType::Linear));
						bossBullet->spr.Tweener(TA::Animate::Create(seq));
						bossBullet->collisionShape = Rect(-4, -4, 8, 8);
						bossBullet->health = 1;
					}
				}
			}
			{
				//�΂̒e���ˏo���鏈�����쐬����
				for (int j = 1; j < 16; j++)
				{
					Actor* bossBullet = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBullet != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position();
						bossBullet->spr =
							Sprite("Res/Objects.png", bossPos, Rect(448, 0, 16, 16));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(0, -100, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * j));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-1000, -200, 0), TA::EasingType::Linear));
						bossBullet->spr.Tweener(TA::Animate::Create(seq));
						bossBullet->collisionShape = Rect(-4, -4, 8, 8);
						bossBullet->health = 1;
					}
				}
			}
			{
				//�΂̒e���ˏo���鏈�����쐬����
				for (int j = 1; j < 16; j++)
				{
					Actor* bossBullet = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBullet != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position();
						bossBullet->spr =
							Sprite("Res/Objects.png", bossPos, Rect(448, 0, 16, 16));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(0, -120, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * j));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-1000, -400, 0), TA::EasingType::Linear));
						bossBullet->spr.Tweener(TA::Animate::Create(seq));
						bossBullet->collisionShape = Rect(-4, -4, 8, 8);
						bossBullet->health = 1;
					}
				}
			}
			{
				//�΂̒e���ˏo���鏈�����쐬����
				for (int j = 1; j < 16; j++)
				{
					Actor* bossBullet = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBullet != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position();
						bossBullet->spr =
							Sprite("Res/Objects.png", bossPos, Rect(448, 0, 16, 16));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(0, -140, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * j));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-1000, -600, 0), TA::EasingType::Linear));
						bossBullet->spr.Tweener(TA::Animate::Create(seq));
						bossBullet->collisionShape = Rect(-4, -4, 8, 8);
						bossBullet->health = 1;
					}
				}
			}
			{
				//�̒e���ˏo���鏈�����쐬����
				Actor* bossBulletBlue = findAvailableActor(
					std::begin(enemyBulletList), std::end(enemyBulletList));
				if (bossBulletBlue != nullptr)
				{
					glm::vec3 bossPos = i->spr.Position() + glm::vec3(-64, 128, 0);
					bossBulletBlue->spr =
						Sprite("Res/Objects.png", bossPos, Rect(448, 48, 32, 16));
					bossBulletBlue->spr.Scale(glm::vec2(3, 1));
					namespace TA = TweenAnimation;
					TA::SequencePtr seq = TA::Sequence::Create(14);
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(0, 0, 0), TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, 0, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, 1.0f,TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, -50, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, -1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, 0, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, -1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, 100, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, 1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-50, 0, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, 1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, -50, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, -1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, 0, 0), TA::EasingType::Linear));

					bossBulletBlue->spr.Tweener(TA::Animate::Create(seq));
					bossBulletBlue->collisionShape = Rect(-4, -4, 8, 8);
					bossBulletBlue->health = 5;
				}
			}
			{
				//�̒e���ˏo���鏈�����쐬����
				Actor* bossBulletBlue = findAvailableActor(
					std::begin(enemyBulletList), std::end(enemyBulletList));
				if (bossBulletBlue != nullptr)
				{
					glm::vec3 bossPos = i->spr.Position() + glm::vec3(-64, -128, 0);
					bossBulletBlue->spr =
						Sprite("Res/Objects.png", bossPos, Rect(448, 48, 32, 16));
					bossBulletBlue->spr.Scale(glm::vec2(3, 1));
					namespace TA = TweenAnimation;
					TA::SequencePtr seq = TA::Sequence::Create(14);
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(0, 0, 0), TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, 0, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, -1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, 50, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, 1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, 0, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, 1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, -50, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, -1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-50, 0, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, -1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-100, 50, 0), TA::EasingType::Linear));
					seq->Add(
						TA::Rotation::Create(0.1f, 1.0f, TA::EasingType::Linear));
					seq->Add(TA::MoveBy::Create(
						0.3f, glm::vec3(-50, 0, 0), TA::EasingType::Linear));
					bossBulletBlue->spr.Tweener(TA::Animate::Create(seq));
					bossBulletBlue->collisionShape = Rect(-4, -4, 8, 8);
					bossBulletBlue->health = 5;
				}
			}
			{
				//�Ԃ̒e���ˏo���鏈�����쐬����
				for (int h = 1; h < 8; h++)
				{
					Actor* bossBulletRed = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBulletRed != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position() + glm::vec3(-64, 104, 0);
						bossBulletRed->spr =
							Sprite("Res/Objects.png", bossPos, Rect(464, 0, 16, 16));
						bossBulletRed->spr.Scale(glm::vec2(2, 1));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(6);
						seq->Add(TA::MoveBy::Create(
							0.5f, glm::vec3(0, 0, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * h));
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(-350 , 0, 0), TA::EasingType::Linear));
						seq->Add(TA::MoveBy::Create(
							0.8f, glm::vec3(0, 0, 0), TA::EasingType::Linear));
						seq->Add(
							TA::Rotation::Create(0.1f, 1.5f, TA::EasingType::Linear));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-100 , -500 , 0), TA::EasingType::Linear));

						bossBulletRed->spr.Tweener(TA::Animate::Create(seq));
						bossBulletRed->collisionShape = Rect(-4, -4, 8, 8);
						bossBulletRed->health = 3;
					}
				}
			}
			{
				//�Ԃ̒e���ˏo���鏈�����쐬����
				for (int h = 1; h < 11; h++)
				{
					Actor* bossBulletRed = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBulletRed != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position() + glm::vec3(-64, 16, 0);
						bossBulletRed->spr =
							Sprite("Res/Objects.png", bossPos, Rect(464, 0, 16, 16));
						bossBulletRed->spr.Scale(glm::vec2(2, 1));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.0f, glm::vec3(-10, 0, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * h));
						seq->Add(TA::MoveBy::Create(
							2.5f, glm::vec3(-1000, 0, 0), TA::EasingType::Linear));
						bossBulletRed->spr.Tweener(TA::Animate::Create(seq));
						bossBulletRed->collisionShape = Rect(-4, -4, 8, 8);
						bossBulletRed->health = 3;
					}
				}
			}
			{
				//�Ԃ̒e���ˏo���鏈�����쐬����
				for (int h = 1; h < 8; h++)
				{
					Actor* bossBulletRed = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBulletRed != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position() + glm::vec3(-64, -16, 0);
						bossBulletRed->spr =
							Sprite("Res/Objects.png", bossPos, Rect(464, 0, 16, 16));
						bossBulletRed->spr.Scale(glm::vec2(2, 1));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(6);
						seq->Add(TA::MoveBy::Create(
							0.5f, glm::vec3(0, 0, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * h));
						seq->Add(TA::MoveBy::Create(
							1.5f, glm::vec3(-300, 0, 0), TA::EasingType::Linear));
						seq->Add(TA::MoveBy::Create(
							0.8f, glm::vec3(0, 0, 0), TA::EasingType::Linear));
						seq->Add(
							TA::Rotation::Create(0.1, -1.5, TA::EasingType::Linear));
						seq->Add(TA::MoveBy::Create(
							3.0f, glm::vec3(-100, 600, 0), TA::EasingType::Linear));
						bossBulletRed->spr.Tweener(TA::Animate::Create(seq));
						bossBulletRed->collisionShape = Rect(-4, -4, 8, 8);
						bossBulletRed->health = 3;
					}
				}
			}
			{
				//�Ԃ̒e���ˏo���鏈�����쐬����
				for (int h = 1; h < 11; h++)
				{
					Actor* bossBulletRed = findAvailableActor(
						std::begin(enemyBulletList), std::end(enemyBulletList));
					if (bossBulletRed != nullptr)
					{
						glm::vec3 bossPos = i->spr.Position() + glm::vec3(-64, -104, 0);
						bossBulletRed->spr =
							Sprite("Res/Objects.png", bossPos, Rect(464, 0, 16, 16));
						bossBulletRed->spr.Scale(glm::vec2(2, 1));
						namespace TA = TweenAnimation;
						TA::SequencePtr seq = TA::Sequence::Create(3);
						seq->Add(TA::MoveBy::Create(
							1.0f, glm::vec3(0, 0, 0), TA::EasingType::Linear));
						seq->Add(TA::Wait::Create(0.1f * h));
						seq->Add(TA::MoveBy::Create(
							2.5f, glm::vec3(-1000, 0, 0), TA::EasingType::Linear));
						bossBulletRed->spr.Tweener(TA::Animate::Create(seq));
						bossBulletRed->collisionShape = Rect(-4, -4, 8, 8);
						bossBulletRed->health = 3;
					}
				}
			}
		}
	}
}