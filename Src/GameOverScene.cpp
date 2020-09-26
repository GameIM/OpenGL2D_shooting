/**
* @file GameOverScene.cpp
*/
#include "GameOverScene.h"
#include "titleScene.h"
#include "GameData.h"

/**
* �Q�[���I�[�o�[��ʂ̏����ݒ���s��
*
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*
* @retval true ����������
* @retval false ���������s
*/
bool initialize(GameOverScene* scene)
{
	Audio::EngineRef audio = Audio::Engine::Instance();
	scene->GameOverBgm = audio.Prepare("Res/Audio/GameOver.mp3");
	scene->GameOverBgm->Play();
	scene->bg = Sprite("Res/GameOver.png");
	scene->GameOver = Sprite("Res/Over.png");
	scene->BackTitle = Sprite("Res/BackTitle.png",glm::vec3(0,-150,0));
	scene->mode = scene->modeStart;
	scene->timer = 0.5f;//���͂��󂯕t���Ȃ�����(�b)
	return true;
}

/**
* �Q�[���I�[�o�[��ʂ̏I���������s��
*
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void finalize(GameOverScene* scene)
{
	scene->bg = Sprite();
	scene->GameOver = Sprite();
	scene->BackTitle = Sprite();
	scene->GameOverBgm->Stop();
}

/**
* �Q�[���I�[�o�[��ʂ̃v���C���[���͂���������
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void processInput(GLFWEW::WindowRef window, GameOverScene* scene)
{
	window.Update();
	/*
	if (scene->timer > 0)
	{
		return;
	}
	*/
	//���͎�t���[�h�ɂȂ�܂łȂɂ����Ȃ�
	if (scene->mode != scene->modeGameOver)
	{
		return;
	}
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown & (GamePad::A | GamePad::START))
	{
		scene->mode = scene->modeNextState;
		scene->timer = 2.0f;
		Audio::Engine::Instance().Prepare("Res/Audio/Start.xwm")->Play();
		finalize(scene);//�Q�[���I�[�o�[��ʂ̌�n��

		//�^�C�g����ʂɖ߂�
		gameState = gameStateTitle;
		initialize(&titleScene);
	}
}

/**
* �Q�[���I�[�o�[��ʂ��X�V����
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void update(GLFWEW::WindowRef window, GameOverScene* scene)
{
	const float deltaTime = window.DeltaTime();

	scene->bg.Update(deltaTime);
	scene->GameOver.Update(deltaTime);
	scene->BackTitle.Update(deltaTime);

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
		finalize(scene);//�Q�[���I�[�o�[��ʂ̌�n��
		initialize(&titleScene);
	}
}

/**
* �Q�[���I�[�o�[��ʂ��X�V����
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void render(GLFWEW::WindowRef window, GameOverScene* scene)
{
	renderer.BeginUpdate();
	renderer.AddVertices(scene->bg);
	renderer.AddVertices(scene->GameOver);
	if (scene->mode == scene->modeGameOver)
	{
		renderer.AddVertices(scene->BackTitle);
	}
	else if (scene->mode == scene->modeNextState)
	{
		if ((int)(scene->timer * 10) % 2)
		{
			renderer.AddVertices(scene->BackTitle);
		}
	}
	renderer.EndUpdate();
	renderer.Draw(glm::vec2(windowWidth, windowHeight));
	window.SwapBuffers();
}