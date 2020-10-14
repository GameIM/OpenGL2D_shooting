/**
* @file GameOverScene.h
*
* �Q�[���I�[�o�[��ʂ̂��߂̍\���̋y�ъ֐���錾����w�b�_�t�@�C��
*/
#include "GLFWEW.h"
#include "Sprite.h"
#include "Audio.h"

struct GameOverScene
{
	Sprite bg;
	Sprite gameOver;
	Sprite backTitle;
	const int modeStart = 0;//�^�C�g���\���҂����[�h
	const int modeGameOver = 1;//���͎�t���[�h
	const int modeNextState = 2;//�Q�[���J�n�҂����[�h
	int mode;//���s���̃��[�h
	float timer;
	Audio::SoundPtr GameOverBgm;
};
bool initialize(GameOverScene*);
void finalize(GameOverScene*);
void processInput(GLFWEW::WindowRef, GameOverScene*);
void update(GLFWEW::WindowRef, GameOverScene*);
void render(GLFWEW::WindowRef, GameOverScene*);
