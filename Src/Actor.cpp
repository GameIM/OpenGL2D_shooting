/**
* @file Actor.cpp
*/
#include "Actor.h"
#include "GameData.h"

/**
*2�̒����`�̏Փˏ�Ԃ𒲂ׂ�
*
* @param �����`����1
* @param �����`����2
*
* @retval true �Փ˂��Ă���
* @retval false �Փ˂��Ă��Ȃ�
*/
bool detectCollision(const Rect* lhs, const Rect* rhs)
{
	return
		lhs->origin.x < rhs->origin.x + rhs->size.x &&
		lhs->origin.x + lhs->size.x > rhs->origin.x &&
		lhs->origin.y < rhs->origin.y + rhs->size.y &&
		lhs->origin.y + lhs->size.y > rhs->origin.y;
}

/**
* Actor�̔z�������������
*
* @param first ����������z��̐擪
* @param last ����������z��̏I�[
*/
void initializeActorList(Actor* first, Actor* last)
{
	//�G�̔z���������
	for (Actor* i = first; i != last; ++i)
	{
		i->health = 0;
	}
}

/**
* Actor�̔z����X�V����
*
* @param first �X�V����z��̐擪
* @param last �X�V����z��̏I�[
* @param deltaTime �O��̍X�V����̌o�ߎ���
*/
void updateActorList(Actor* first, Actor* last, float deltaTime)
{
	for (Actor* i = first; i != last; ++i)
	{
		if (i->health > 0)
		{
			i->spr.Update(deltaTime);
			if (i->spr.Tweener() && i->spr.Tweener()->IsFinished())
			{
				i->health = 0;
			}
		}
	}
}

/**
* Actor�z���`�悷��
*
* @param first �`�悷��z��̐擪
* @param last �`�悷��z��̏I�[
*/
void renderActorList(const Actor* first, const Actor* last)
{
	for (const Actor* i = first; i != last; ++i)
	{
		if (i->health > 0)
		{
			renderer.AddVertices(i->spr);
		}
	}
}

/**
* ���p�\��Actor���擾����
*
* @param first �����Ώۂ̐擪�v�f�̃|�C���^
* @param last �����Ώۂ̏I�[�v�f�̃|�C���^
*
* @return ���p�\��Actor�̃|�C���^
*		���p�\��Actor��������Ȃ����nullptr
*
* [first, last]�͈̔͂���A���p�\��(health��0�ȉ���)Actor����������
*/
Actor* findAvailableActor(Actor* first, Actor* last)
{
	Actor* result = nullptr;
	for (Actor* i = first; i != last; ++i)
	{
		if (i->health <= 0)
		{
			result = i;
			break;
		}
	}
	return result;
}