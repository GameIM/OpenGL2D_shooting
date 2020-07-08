/**
* @file Actor.h
*
* Actor�\���̋y��Actor�\���̂Ɋ֘A����֐���錾����w�b�_�t�@�C��
*/
#include "Sprite.h"

/**
* �Q�[���L�����N�^�[�\����
*/
struct Actor//�G�̍\����
{
	Sprite spr;//�摜�\���p�X�v���C�g
	Rect collisionShape;//�Փ˔���̈ʒu�Ƒ傫��
	int health;//�ϋv�l(0�ȉ��Ȃ�j�󂳂�Ă���)
	float timer;//���ړI�^�C�}�[
	int id;//���ʗpID
};
bool detectCollision(const Rect* lhs, const Rect* rhs);
void initializeActorList(Actor*, Actor*);
void updateActorList(Actor*, Actor*, float);
void renderActorList(const Actor*, const Actor*);
Actor* findAvailableActor(Actor*, Actor*);//��������֐�

using CollisionHandlerType = void(*)(Actor*, Actor*);//�Փˏ����֐��̌^
void detectCollision(Actor*, Actor*, Actor*, Actor*, CollisionHandlerType);
