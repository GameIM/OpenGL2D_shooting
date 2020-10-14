/**
* @file GameClearScene.cpp
*/
#include "GameClearScene.h"
#include "TitleScene.h"
#include "GameData.h"

/**
* �Q�[���N���A��ʂ̏����ݒ���s��
*
* @param scene �Q�[���N���A��ʗp�\���̂̃|�C���^
*
* @retval true ����������
* @retval false ���������s
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
	scene->timer = 0.5f;//���͂��󂯕t���Ȃ�����(�b)
	return true;
}

/**
* �Q�[���N���A��ʂ̏I���������s��
*
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void finalize(GameClearScene* scene)
{
	scene->bg = Sprite();
	scene->gameClear = Sprite();
	scene->backTitle = Sprite();
	scene->gameClearBgm->Stop();
}

/**
* �Q�[���N���A��ʂ̃v���C���[���͂���������
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
*/
void processInput(GLFWEW::WindowRef window, GameClearScene* scene)
{
	window.Update();
	const GamePad gamepad = window.GetGamePad();
	if (gamepad.buttonDown & (GamePad::A | GamePad::START))
	{
		finalize(scene);//�Q�[���N���A��ʂ̌�n��
		//�^�C�g����ʂɖ߂�
		gameState = gameStateTitle;
		initialize(&titleScene);
	}
}

/**
* �Q�[���N���A��ʂ��X�V����
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
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
		finalize(scene);//�Q�[���N���A��ʂ̌�n��
		initialize(&titleScene);
	}
}

/**
* �Q�[���N���A��ʂ��X�V����
*
* @param window �Q�[�����Ǘ�����E�B���h�E
* @param scene �Q�[���I�[�o�[��ʗp�\���̂̃|�C���^
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