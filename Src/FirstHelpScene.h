/**
* @file FirstHelpScene.h
*/
#include "GLFWEW.h"
#include "Sprite.h"

struct FirstHelpScene
{
	//�X�v���C�g�p�̕ϐ�
	Sprite bg;
	Sprite Keypad;
	Sprite Up;
	Sprite Left;
	Sprite Right;
	Sprite Down;
	Sprite Big;
	Sprite Skey;
	Sprite Space;

	//�����X�v���C�g�p�̕ϐ�
	Sprite Hand;
	Sprite BackMain;
	Sprite Wepon;
	Sprite Attack;
	Sprite Cursors;
	Sprite ToMain;
	const int modeStart = 0;//�^�C�g���\���҂����[�h
	const int modeGameHelp = 1;//���͎�t���[�h
	const int modeNextState = 2;//�Q�[���J�n�҂����[�h
	float timer;//���[�h�؂�ւ��Ŏg�p����^�C�}�[
	int mode;//���s���̃��[�h
};
bool initialize(FirstHelpScene*);
void finalize(FirstHelpScene*);
void processInput(GLFWEW::WindowRef, FirstHelpScene*);
void update(GLFWEW::WindowRef, FirstHelpScene*);
void render(GLFWEW::WindowRef, FirstHelpScene*);


