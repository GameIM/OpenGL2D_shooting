/**
* @file MainScene.h
*/
#include "Sprite.h"

//マップに配置されたオブジェクトのID
const int tileId_SmallEnemy = 256;
const int tileId_MediumEnemy = 191;
const int tileId_LargeEnemy = 63;
const int tileId_Boss = 75;
const int tileId_PowerUpItem = 230;
const int tiled_HealItem = 227;


/**
* マップに配置されたオブジェクトのデータ
*/
struct MapObjectData
{
	int id;//オブジェクトのID
	Rect imageRect;//画像の範囲
	Rect collisionShape;//衝突判定の範囲
	int health;//初期体力
	FrameAnimation::TimelinePtr tl;//アニメーション タイムライン

	using CreateTweenFunc = TweenAnimation::TweenPtr(*)();//行動設定関数の型
	CreateTweenFunc createTween;
};

/**
* メイン画面で使用する構造体
*/
struct MainScene
{
	int stageNo;//現在のステージ番号
};
bool initialize(MainScene*);

void initializeMapObjectDataList();
const MapObjectData* findMapObjectData(int);