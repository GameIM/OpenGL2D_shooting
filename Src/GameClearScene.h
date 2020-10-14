/**
* @ file GameCearScene.h
*
* �Q�[���N���A��ʂ̂��߂̍\���̋y�ъ֐���錾����w�b�_�t�@�C��
*/
#include "GLFWEW.h"
#include "Sprite.h"
#include "Audio.h"

struct GameClearScene
{
	Sprite bg;
	Sprite gameClear;
	Sprite backTitle;
	const int modeStart = 0;//�^�C�g���\���҂����[�h
	const int modeGameClear = 1;//���͎�t���[�h
	const int modeNextState = 2;//�Q�[���J�n�҂����[�h
	int mode;//���s���̃��[�h
	float timer;
	Audio::SoundPtr gameClearBgm;
};
bool initialize(GameClearScene*);
void finalize(GameClearScene*);
void processInput(GLFWEW::WindowRef, GameClearScene*);
void update(GLFWEW::WindowRef, GameClearScene*);
void render(GLFWEW::WindowRef, GameClearScene*);
