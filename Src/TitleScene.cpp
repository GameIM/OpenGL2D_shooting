/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include "MainScene.h"
#include "GameData.h"

/**
* �^�C�g����ʗp�̍\���̂̏����ݒ���s��
*
* @param scene �^�C�g����ʗp�\���̂̃|�C���^
* @param gamestate �Q�[����Ԃ�\���ϐ��̃|�C���^
*
* @retval true ����������
* @retval false ���������s
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
	scene->timer = 0.5f;//���͂��󂯕t���Ȃ�����(�b)
	return true;
}

/**
* �^�C�g����ʂ̏I���������s��
*
* @param scene �^�C�g����ʗp�\���̂̃|�C���^
*/
void finalize(TitleScene* scene)
{
	scene->bg = Sprite();
	scene->logo = Sprite();
	scene->titleBgm->Stop();
}

/**
* �^�C�g����ʂ̃v���C���[���͂���������
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �^�C�g����ʗp�\���̂̃|�C���^
*/
void processInput(GLFWEW::WindowRef window, TitleScene* scene)
{
	window.Update();
	//���͎�t���[�h�ɂȂ�܂łȂɂ����Ȃ�
	if (scene->mode != scene->modeTitle)
	{
		return;
	}
	//A�܂���START�{�^���������ꂽ��A�Q�[���J�n�҂����[�h�Ɉڂ�
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown &GamePad::START)
	{
		scene->mode = scene->modeNextState;
		scene->timer = 2.0f;
		Audio::Engine::Instance().Prepare("Res/Audio/Start.xwm")->Play();
	}
}

/**
* �^�C�g����ʂ��X�V����
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �^�C�g����ʗp�\���̂̃|�C���^
*/
void update(GLFWEW::WindowRef window, TitleScene* scene)
{
	const float deltaTime = window.DeltaTime();

	scene->bg.Update(deltaTime);
	scene->logo.Update(deltaTime);
	scene->Press.Update(deltaTime);

	//�^�C�}�[��0�ȉ��ɂȂ�܂ŃJ�E���g�_�E��
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
		finalize(scene);//�^�C�g����ʂ̌�n��

		//�Q�[���̏����ݒ���s��
		gameState = gameStateMain;
		mainScene.stageNo = 1;//�X�e�[�W1����J�n
		initialize(&mainScene);
		
	}
}

/**
* �^�C�g����ʂ�`�悷��
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �^�C�g����ʗp�\���̂̃|�C���^
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

