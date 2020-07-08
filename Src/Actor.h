/**
* @file Actor.h
*
* Actor構造体及びActor構造体に関連する関数を宣言するヘッダファイル
*/
#include "Sprite.h"

/**
* ゲームキャラクター構造体
*/
struct Actor//敵の構造体
{
	Sprite spr;//画像表示用スプライト
	Rect collisionShape;//衝突判定の位置と大きさ
	float health;//耐久値(0以下なら破壊されている)
	float timer;//多目的タイマー
	int id;//識別用ID
};
bool detectCollision(const Rect* lhs, const Rect* rhs);
void initializeActorList(Actor*, Actor*);
void updateActorList(Actor*, Actor*, float);
void renderActorList(const Actor*, const Actor*);
Actor* findAvailableActor(Actor*, Actor*);//検索する関数

using CollisionHandlerType = void(*)(Actor*, Actor*);//衝突処理関数の型
void detectCollision(Actor*, Actor*, Actor*, Actor*, CollisionHandlerType);
