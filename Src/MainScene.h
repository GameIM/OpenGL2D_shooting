/**
* @file MainScene.h
*/
#include "Sprite.h"

//�}�b�v�ɔz�u���ꂽ�I�u�W�F�N�g��ID
const int tileId_SmallEnemy = 256;
const int tileId_MediumEnemy = 191;
const int tileId_LargeEnemy = 63;
const int tileId_Boss = 75;
const int tileId_PowerUpItem = 230;
const int tiled_HealItem = 227;


/**
* �}�b�v�ɔz�u���ꂽ�I�u�W�F�N�g�̃f�[�^
*/
struct MapObjectData
{
	int id;//�I�u�W�F�N�g��ID
	Rect imageRect;//�摜�͈̔�
	Rect collisionShape;//�Փ˔���͈̔�
	int health;//�����̗�
	FrameAnimation::TimelinePtr tl;//�A�j���[�V���� �^�C�����C��

	using CreateTweenFunc = TweenAnimation::TweenPtr(*)();//�s���ݒ�֐��̌^
	CreateTweenFunc createTween;
};

/**
* ���C����ʂŎg�p����\����
*/
struct MainScene
{
	int stageNo;//���݂̃X�e�[�W�ԍ�
};
bool initialize(MainScene*);

void initializeMapObjectDataList();
const MapObjectData* findMapObjectData(int);