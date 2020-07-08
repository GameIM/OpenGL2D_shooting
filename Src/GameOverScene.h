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
	float timer;
};
bool initialize(GameOverScene*);
void finalize(GameOverScene*);
void processInput(GLFWEW::WindowRef, GameOverScene*);
void update(GLFWEW::WindowRef, GameOverScene*);
void render(GLFWEW::WindowRef, GameOverScene*);
