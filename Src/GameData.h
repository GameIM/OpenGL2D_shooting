/**
* @file GameData.h
*/
#include "Sprite.h"
#include "Font.h"

extern SpriteRenderer renderer;//�X�v���C�g�`��p�ϐ�
extern FontRenderer fontRenderer;//�t�H���g�`��p�ϐ�

const int windowWidth = 800;//�E�B���h�E�̕`��̈�̕�
const int windowHeight = 600;//�E�B���h�E�̕`��̈�̍���

const int gameStateTitle = 0;//�^�C�g����ʂ̏��ID
const int gameStateMain = 1;//�Q�[����ʂ̏��ID
const int gameStateGameOver = 2;//�Q�[���I�[�o�[��ʂ̏��ID
const int gameStatePose = 3;//�|�[�Y��ʂ̏��ID
const int gameStateClear = 4;//�Q�[���N���A��ʂ̏��ID
const int gameStateHelp = 5;//���������ʂ̏��ID
extern int gameState;//���s���̏��ID

//��s�錾
struct TitleScene;
struct FirstHelpScene;
struct MainScene;
struct GameClearScene;
struct GameOverScene;

extern TitleScene titleScene;
extern FirstHelpScene firstHelpScene;
extern MainScene mainScene;
extern GameClearScene gameClearScene;
extern GameOverScene gameOverScene;
