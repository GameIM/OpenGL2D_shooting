/**
* @file FirstHelp.cpp
*/
#include "FirstHelpScene.h"
#include "MainScene.h"
#include "GameData.h"

/**
* ���������ʂ̏����ݒ���s��
*
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*
* @retval true ����������
* @retval false ���������s
*/
bool initialize(FirstHelpScene* scene)
{
	//�X�v���C�g��ݒ肷��
	scene->bg = Sprite("Res/Pose.png");
	scene->Keypad = Sprite("Res/Keypad.png", glm::vec3(0, 20, 0));
	scene->Up = Sprite("Res/Cursors.png", glm::vec3(263, -60, 0));
	scene->Left = Sprite("Res/Cursors.png", glm::vec3(225, -100, 0));
	scene->Right = Sprite("Res/Cursors.png", glm::vec3(302, -102, 0));
	scene->Down = Sprite("Res/Down.png", glm::vec3(260, -125, 0));
	scene->Big = Sprite("Res/Big.png", glm::vec3(300, 130, 0));
	scene->Skey = Sprite("Res/Skey.png", glm::vec3(-157, -30, 0));
	scene->Space = Sprite("Res/Space.png", glm::vec3(-25, -105, 0));

	//�����X�v���C�g��ݒ肷��
	scene->Hand = Sprite("Res/Hand.png", glm::vec3(0, 260, 0));
	scene->BackMain = Sprite("Res/BackMain.png", glm::vec3(-250, 220, 0));
	scene->Wepon = Sprite("Res/Wepon.png", glm::vec3(-240, -180, 0));
	scene->Attack = Sprite("Res/Attack.png", glm::vec3(60, -210, 0));
	scene->Cursors = Sprite("Res/Move.png", glm::vec3(250, -170, 0));
	scene->ToMain = Sprite("Res/ToMain.png", glm::vec3(245,210,0));
	scene->mode = scene->modeStart;
	scene->timer = 0.5f;//���͂��󂯕t���Ȃ�����(�b)
	return true;
}

/**
* ���������ʂ̏I���������s��
*
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void finalize(FirstHelpScene* scene)
{
	//�X�v���C�g�̌�n��������
	scene->Keypad = Sprite();
	scene->bg = Sprite();
	scene->Up = Sprite();
	scene->Left = Sprite();
	scene->Right = Sprite();
	scene->Down = Sprite();
	scene->Big = Sprite();
	scene->Skey = Sprite();
	scene->Space = Sprite();

	//�����X�v���C�g�̌�n��������
	scene->Hand = Sprite();
	scene->Wepon = Sprite();
	scene->Attack = Sprite();
	scene->Cursors = Sprite();
	scene->ToMain = Sprite();
}

/**
* ���������ʂ̃v���C���[���͂���������
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void processInput(GLFWEW::WindowRef window, FirstHelpScene* scene)
{
	window.Update();
	//���͎�t���[�h�ɂȂ�܂łȂɂ����Ȃ�
	if (scene->mode != scene->modeGameHelp)
	{
		return;
	}
	//START�{�^���������ꂽ��A�Q�[���J�n�҂����[�h�Ɉڂ�
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown &GamePad::START)
	{
		scene->mode = scene->modeNextState;
		scene->timer = 0.5f;
	}
}

/**
* ���������ʂ��X�V����
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void update(GLFWEW::WindowRef window, FirstHelpScene* scene)
{
	const float deltaTime = window.DeltaTime();
	//�X�v���C�g���X�V����
	scene->bg.Update(deltaTime);
	scene->Up.Update(deltaTime);
	scene->Left.Update(deltaTime);
	scene->Right.Update(deltaTime);
	scene->Down.Update(deltaTime);
	scene->Big.Update(deltaTime);
	scene->Skey.Update(deltaTime);
	scene->Space.Update(deltaTime);

	//�����X�v���C�g���X�V����
	scene->Keypad.Update(deltaTime);
	scene->Hand.Update(deltaTime);
	scene->Wepon.Update(deltaTime);
	scene->Attack.Update(deltaTime);
	scene->Cursors.Update(deltaTime);
	scene->ToMain.Update(deltaTime);

	//�^�C�}�[��0�ȉ��ɂȂ�܂ŃJ�E���g�_�E��
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
		finalize(scene);//�Q�[���I�[�o�[��ʂ̌�n��

		gameState = gameStateMain;
		mainScene.stageNo = 1;//�X�e�[�W1����J�n
		initialize(&mainScene);
	}
}

/**
* ���������ʂ��X�V����
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void render(GLFWEW::WindowRef window, FirstHelpScene* scene)
{
	renderer.BeginUpdate();
	//�X�v���C�g�̒��_�f�[�^��ݒ肷��
	renderer.AddVertices(scene->bg);
	renderer.AddVertices(scene->Keypad);
	renderer.AddVertices(scene->Up);
	renderer.AddVertices(scene->Left);
	renderer.AddVertices(scene->Right);
	renderer.AddVertices(scene->Down);
	renderer.AddVertices(scene->Big);
	renderer.AddVertices(scene->Skey);
	renderer.AddVertices(scene->Space);

	//�����X�v���C�g�̒��_�f�[�^��ݒ肷��
	renderer.AddVertices(scene->Hand);
	renderer.AddVertices(scene->Wepon);
	renderer.AddVertices(scene->Attack);
	renderer.AddVertices(scene->Cursors);
	renderer.AddVertices(scene->ToMain);
	renderer.EndUpdate();
	renderer.Draw(glm::vec2(windowWidth, windowHeight));
	window.SwapBuffers();
}