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
	scene->bg = Sprite("Res/UnknownPlanet.png");
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
	if (scene->timer > 0)
	{
		return;
	}
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown & (GamePad::A | GamePad::START))
	{
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
	if (scene->timer > 0)
	{
		scene->timer -= deltaTime;
	}
	scene->bg.Update(deltaTime);
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
	renderer.EndUpdate();
	renderer.Draw(glm::vec2(windowWidth, windowHeight));

	fontRenderer.BeginUpdate();
	fontRenderer.AddString(glm::vec2(-120, 16), "GAME OVER");
	//fontRenderer.Scale(glm::vec2(2.0f, 2.0f));
	if ((int)(scene->timer * 10) % 2)
	{
		fontRenderer.AddString(glm::vec2(-250, -30), "PRESS ENTER BACK TATLE");
	}
	fontRenderer.EndUpdate();
	fontRenderer.Draw();

	window.SwapBuffers();
}