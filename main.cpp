#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const char kWindowTitle[] = "LC1B_カトウ_ダイドウ_モリ_はたらくハコビヤ";


typedef struct Vector2
{
	float x;
	float y;
}Vector2;

enum Direction
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
};

enum Block
{
	STAGE_WALL,
	BLOCK_WALL,
	BLOCK_FLOOR,
	BLOCK_BOX,
	BLOCK_GOOL,
	BLOCK_DASH,
	BLOCK_ITEM,
};

typedef struct Player
{
	Vector2 pos;
	Vector2 mapPos;
	Vector2 frontPos;
	Vector2 backPos;
	Vector2 leftPos;
	Vector2 rightPos;
	Vector2 centerPos;
	float width;
	float height;
	float speed;
	int direction;
	int isItemGet;
	int grabBlock;
	int isFrontReady;
	int isBackReady;
	int isLeftReady;
	int isRightReady;
}Player;

typedef struct Robot
{
	Vector2 pos;
	Vector2 mapPos;
	float speed;
	float width;
	float height;
	float speedTmp;
}Robot;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 768, 768);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//========================
	//変数の宣言と初期化
	//========================

	/*---マップチップ---*/
	const int kMapSize = 8;

	float blockSize = 64.0f;

	//map

	int map[kMapSize][kMapSize] =
	{
		{ 0,0,0,0,0,0,0,0, },
		{ 0,1,1,1,1,1,0,0, },
		{ 0,1,2,2,2,1,0,0, },
		{ 0,1,2,2,2,1,0,0, },
		{ 0,1,2,3,2,1,0,0, },
		{ 0,1,1,1,1,1,0,0, },
		{ 0,0,0,0,0,0,0,0, },
		{ 0,0,0,0,0,0,0,0, },

	};

	/*---プレイヤー---*/
	Player player;
	player.pos.x = 192.0f;
	player.pos.y = 192.0f;
	player.width = blockSize;
	player.height = blockSize;
	player.centerPos.x = player.pos.x + player.width / 2.0f;
	player.centerPos.y = player.pos.x + player.height / 2.0f;
	player.speed = 64.0f;
	//ブロックの判定する点
	player.frontPos.x = player.centerPos.x;
	player.frontPos.y = player.centerPos.y + player.height;
	player.backPos.x = player.centerPos.x;
	player.backPos.y = player.centerPos.y - player.height;
	player.rightPos.x = player.centerPos.x + player.width;
	player.rightPos.y = player.centerPos.y;
	player.leftPos.x = player.centerPos.x - player.width;
	player.leftPos.y = player.centerPos.y;
	//フラグ
	player.grabBlock = false;
	player.isFrontReady = false;
	player.isBackReady = false;
	player.isLeftReady = false;
	player.isRightReady = false;
	player.isItemGet = false;
	player.direction = FRONT;


	/*---ロボット---*/
	Robot robot;
	robot.pos.x = 256.0f;
	robot.pos.y = 64.0f;
	robot.width = blockSize;
	robot.height = blockSize;
	robot.speed = 64.0f;

	/*---画像の読み込み---*/
	int wallBlockHandle = Novice::LoadTexture("./Resources/images/wallBlock.png");
	int floorBlockHandle = Novice::LoadTexture("./Resources/images/floorBlock.png");


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//==========================
		//プレイヤーとロボットの更新処理
		//==========================

		/*---移動処理---*/
		if (!player.grabBlock)
		{
			if (keys[DIK_W] && !preKeys[DIK_W])
			{
				/*---プレイヤー---*/
				//向いている方向
				player.direction = BACK;

				//座標の更新
				player.pos.y -= player.speed;
				player.centerPos.y -= player.speed;

				player.backPos.y -= player.speed;
				player.frontPos.y -= player.speed;
				player.leftPos.y -= player.speed;
				player.rightPos.y -= player.speed;

				//フラグを全てfalseに変える
				player.isFrontReady = false;
				player.isBackReady = false;
				player.isLeftReady = false;
				player.isRightReady = false;

				//プレイヤーが壁とボックスに当たった時、座標を戻す
				if (map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_GOOL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_WALL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == STAGE_WALL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_DASH ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_BOX)
				{
					player.pos.y += player.speed;
					robot.pos.y += robot.speed;
					player.centerPos.y += player.speed;

					player.backPos.y += player.speed;
					player.frontPos.y += player.speed;
					player.leftPos.y += player.speed;
					player.rightPos.y += player.speed;

				}

				//ロボット
				robot.pos.y -= robot.speed;

				if (map[static_cast<int>(robot.pos.x/ blockSize)][static_cast<int>(robot.pos.y / blockSize)] == BLOCK_FLOOR ||
					map[static_cast<int>(robot.pos.x / blockSize)][static_cast<int>(robot.pos.y / blockSize)] == STAGE_WALL ||
					map[static_cast<int>(robot.pos.x / blockSize)][static_cast<int>(robot.pos.y / blockSize)] == BLOCK_ITEM)
				{
					robot.pos.y += robot.speed;
				}
			}

			if (keys[DIK_A] && !preKeys[DIK_A])
			{
				//プレイヤー
				player.direction = LEFT;

				//座標の更新
				player.pos.x -= player.speed;
				player.centerPos.x -= player.speed;

				player.backPos.x -= player.speed;
				player.frontPos.x -= player.speed;
				player.leftPos.x -= player.speed;
				player.rightPos.x -= player.speed;

				//フラグを全てfalseに変える
				player.isFrontReady = false;
				player.isBackReady = false;
				player.isLeftReady = false;
				player.isRightReady = false;

				//プレイヤーが壁とボックスに当たった時、座標を戻す
				if (map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_GOOL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_WALL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == STAGE_WALL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_DASH ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_BOX)
				{
					player.pos.x += player.speed;
					robot.pos.x += robot.speed;
					player.centerPos.x += player.speed;

					player.backPos.x += player.speed;
					player.frontPos.x += player.speed;
					player.leftPos.x += player.speed;
					player.rightPos.x += player.speed;
				}

				//ロボット
				robot.pos.x -= robot.speed;

				if (map[static_cast<int>(robot.pos.x / blockSize)][static_cast<int>(robot.pos.y / blockSize)] == BLOCK_FLOOR ||
					map[static_cast<int>(robot.pos.x / blockSize)][static_cast<int>(robot.pos.y / blockSize)] == STAGE_WALL ||
					map[static_cast<int>(robot.pos.x / blockSize)][static_cast<int>(robot.pos.y / blockSize)] == BLOCK_ITEM)
				{
					robot.pos.x += robot.speed;
				}
			}

			if (keys[DIK_S] && !preKeys[DIK_S])
			{
				/*---プレイヤー---*/
				//向いている方向
				player.direction = FRONT;

				//座標の更新
				player.pos.y += player.speed;
				player.centerPos.y += player.speed;

				player.frontPos.y += player.speed;
				player.backPos.y += player.speed;
				player.leftPos.y += player.speed;
				player.rightPos.y += player.speed;

				//フラグを全てfalseに変える
				player.isFrontReady = false;
				player.isBackReady = false;
				player.isLeftReady = false;
				player.isRightReady = false;

				//プレイヤーが壁とボックスに当たった時、座標を戻す
				if (map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_GOOL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_WALL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == STAGE_WALL ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_DASH ||
					map[static_cast<int>(player.pos.x / blockSize)][static_cast<int>(player.pos.y / blockSize)] == BLOCK_BOX)
				{
					player.pos.y -= player.speed;
					robot.pos.y -= robot.speed;
					player.centerPos.y -= player.speed;

					player.frontPos.y -= player.speed;
					player.backPos.y -= player.speed;
					player.leftPos.y -= player.speed;
					player.rightPos.y -= player.speed;

				}

				//ロボット
				robot.pos.y += robot.speed;

				if (map[static_cast<int>(robot.pos.x / blockSize)][static_cast<int>(robot.pos.y / blockSize)] == BLOCK_FLOOR ||
					map[static_cast<int>(robot.pos.x / blockSize)][static_cast<int>(robot.pos.y / blockSize)] == STAGE_WALL ||
					map[static_cast<int>(robot.pos.x / blockSize)][static_cast<int>(robot.pos.y / blockSize)] == BLOCK_ITEM)
				{
					robot.pos.y -= robot.speed;
				}
			}

			if (keys[DIK_D] && !preKeys[DIK_D])
			{
				/*---プレイヤー---*/
				//向いている方向
				player.direction = RIGHT;

				//座標の更新
				player.pos.x += player.speed;
				player.centerPos.x += player.speed;

				player.backPos.x += player.speed;
				player.frontPos.x += player.speed;
				player.leftPos.x += player.speed;
				player.rightPos.x += player.speed;

				//フラグを全てfalseに変える
				player.isFrontReady = false;
				player.isBackReady = false;
				player.isLeftReady = false;
				player.isRightReady = false;


				//プレイヤーが壁とボックスに当たった時、座標を戻す
				if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_GOOL ||
					map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
					map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
					map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH ||
					map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX)
				{
					player.pos.x -= player.speed;
					robot.pos.x -= robot.speed;
					player.centerPos.x -= player.speed;

					player.backPos.x -= player.speed;
					player.frontPos.x -= player.speed;
					player.leftPos.x -= player.speed;
					player.rightPos.x -= player.speed;
				}

				//ロボット
				robot.pos.x += robot.speed;

				if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
					map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
					map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
				{
					robot.pos.x -= robot.speed;
				}
			}


		}

		//ブロックがプレイヤーが向いてる方向にあるからどうかの判定
		if (player.direction == FRONT)
		{
			if (map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_BOX)
			{
				player.isFrontReady = true;
				//向きの固定
				player.direction = FRONT;
			}
		}
		else if (player.direction == BACK)
		{
			if (map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_BOX)
			{
				player.isBackReady = true;
				//向きの固定
				player.direction = BACK;
			}
		}
		else if (player.direction == LEFT)
		{
			if (map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_BOX)
			{
				player.isLeftReady = true;
				//向きの固定
				player.direction = LEFT;
			}
		}
		else if (player.direction == RIGHT)
		{
			if (map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_BOX)
			{
				player.isRightReady = true;
				//向きの固定
				player.direction = RIGHT;
			}
		}


		// スペースを押すとフラグをtrueに変える
		if (player.isFrontReady || player.isBackReady || player.isLeftReady || player.isRightReady)
		{
			if (keys[DIK_SPACE])
			{
				player.grabBlock = true;
			}
			else
			{
				player.grabBlock = false;
			}
		}

		//ブロックを掴んでいる時
		if (player.grabBlock)
		{
			//上下
			if (player.direction == FRONT || player.direction == BACK)
			{
				if (keys[DIK_W] && !preKeys[DIK_W])
				{
					/*---プレイヤー---*/

					//座標の更新
					player.pos.y -= player.speed;
					player.centerPos.y -= player.speed;

					player.backPos.y -= player.speed;
					player.frontPos.y -= player.speed;
					player.leftPos.y -= player.speed;
					player.rightPos.y -= player.speed;

					//プレイヤーが壁とボックスに当たった時、座標を戻す
					if (player.isBackReady)
					{
						if (map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_GOOL ||
							map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_WALL ||
							map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == STAGE_WALL ||
							map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_DASH ||
							map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_BOX)
						{
							player.pos.y += player.speed;
							robot.pos.y += robot.speed;
							player.centerPos.y += player.speed;

							player.backPos.y += player.speed;
							player.frontPos.y += player.speed;
							player.leftPos.y += player.speed;
							player.rightPos.y += player.speed;
						}
					}
					else if (player.isFrontReady)
					{
						if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
							map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL)
						{
							player.pos.y += player.speed;
							robot.pos.y += robot.speed;
							player.centerPos.y += player.speed;

							player.frontPos.y += player.speed;
							player.backPos.y += player.speed;
							player.leftPos.y += player.speed;
							player.rightPos.y += player.speed;
						}
					}

					//ロボット
					robot.pos.y -= robot.speed;

					if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
					{
						robot.pos.y += robot.speed;
					}
				}

				if (keys[DIK_S] && !preKeys[DIK_S])
				{
					/*---プレイヤー---*/

					//座標の更新
					player.pos.y += player.speed;
					player.centerPos.y += player.speed;

					player.frontPos.y += player.speed;
					player.backPos.y += player.speed;
					player.leftPos.y += player.speed;
					player.rightPos.y += player.speed;

					//プレイヤーが壁とボックスに当たった時、座標を戻す
					if (player.isBackReady)
					{
						if (map[static_cast<int>((player.pos.y) / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
							map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL)
						{
							player.pos.y -= player.speed;
							robot.pos.y -= robot.speed;
							player.centerPos.y -= player.speed;

							player.frontPos.y -= player.speed;
							player.backPos.y -= player.speed;
							player.leftPos.y -= player.speed;
							player.rightPos.y -= player.speed;
						}
					}
					else if (player.isFrontReady)
					{
						if (map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_GOOL ||
							map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_WALL ||
							map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == STAGE_WALL ||
							map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_DASH ||
							map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_BOX)
						{
							player.pos.y -= player.speed;
							robot.pos.y -= robot.speed;
							player.centerPos.y -= player.speed;

							player.frontPos.y -= player.speed;
							player.backPos.y -= player.speed;
							player.leftPos.y -= player.speed;
							player.rightPos.y -= player.speed;
						}
					}

					//ロボット
					robot.pos.y += robot.speed;
					if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
					{
						robot.pos.y -= robot.speed;
					}
				}
			}

			if (player.direction == LEFT || player.direction == RIGHT)
			{
				if (keys[DIK_A] && !preKeys[DIK_A])
				{
					/*---プレイヤー---*/

					//座標の更新
					player.pos.x -= player.speed;
					player.centerPos.x -= player.speed;

					player.backPos.x -= player.speed;
					player.frontPos.x -= player.speed;
					player.leftPos.x -= player.speed;
					player.rightPos.x -= player.speed;

					//プレイヤーが壁とボックスに当たった時、座標を戻す
					if (player.isLeftReady)
					{
						if (map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_GOOL ||
							map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_WALL ||
							map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == STAGE_WALL ||
							map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_DASH ||
							map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_BOX)
						{
							player.pos.x += player.speed;
							robot.pos.x += robot.speed;
							player.centerPos.x += player.speed;

							player.backPos.x += player.speed;
							player.frontPos.x += player.speed;
							player.leftPos.x += player.speed;
							player.rightPos.x += player.speed;
						}
					}
					else if (player.isRightReady)
					{
						if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
							map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL)
						{
							player.pos.x += player.speed;
							robot.pos.x += robot.speed;
							player.centerPos.x += player.speed;

							player.backPos.x += player.speed;
							player.frontPos.x += player.speed;
							player.leftPos.x += player.speed;
							player.rightPos.x += player.speed;
						}
					}

					//ロボット
					robot.pos.x -= robot.speed;

					if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
					{
						robot.pos.x += robot.speed;
					}
				}

				if (keys[DIK_D] && !preKeys[DIK_D])
				{
					/*---プレイヤー---*/

					//座標の更新
					player.pos.x += player.speed;
					player.centerPos.x += player.speed;

					player.backPos.x += player.speed;
					player.frontPos.x += player.speed;
					player.leftPos.x += player.speed;
					player.rightPos.x += player.speed;

					//プレイヤーが壁とボックスに当たった時、座標を戻す
					if (player.isLeftReady)
					{
						if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
							map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL)
						{
							player.pos.x -= player.speed;
							robot.pos.x -= robot.speed;
							player.centerPos.x -= player.speed;

							player.backPos.x -= player.speed;
							player.frontPos.x -= player.speed;
							player.leftPos.x -= player.speed;
							player.rightPos.x -= player.speed;
						}
					}
					else if (player.isRightReady)
					{
						if (map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_GOOL ||
							map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_WALL ||
							map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == STAGE_WALL ||
							map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_DASH ||
							map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_BOX)
						{
							player.pos.x -= player.speed;
							robot.pos.x -= robot.speed;
							player.centerPos.x -= player.speed;

							player.backPos.x -= player.speed;
							player.frontPos.x -= player.speed;
							player.leftPos.x -= player.speed;
							player.rightPos.x -= player.speed;
						}
					}


					//ロボット

					robot.pos.x += robot.speed;

					if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
					{
						robot.pos.x -= robot.speed;
					}
				}
			}

			/*---ブロックを動かす処理---*/
			if (player.isBackReady)
			{
				//ブロック後ろに動かす
				map[static_cast<int>((player.backPos.y + player.height) / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = BLOCK_FLOOR;
				map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = BLOCK_BOX;


				//ブロック前に動かす
				if (keys[DIK_S] && !preKeys[DIK_S])
				{
					map[static_cast<int>((player.backPos.y - player.height) / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = BLOCK_FLOOR;
				}

				map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = BLOCK_BOX;

			}
			else if (player.isFrontReady)
			{
				if (keys[DIK_W] && !preKeys[DIK_W])
				{
					//ブロック後ろに動かす
					map[static_cast<int>((player.frontPos.y + player.height) / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] = BLOCK_FLOOR;
				}
				map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] = { BLOCK_BOX };

				//ブロック前に動かす
				map[static_cast<int>((player.frontPos.y - player.height) / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] = BLOCK_FLOOR;
				map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] = BLOCK_BOX;


			}
			else if (player.isLeftReady)
			{
				//ブロックを動かす

				map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>((player.leftPos.x + player.width) / blockSize)] = BLOCK_FLOOR;

				map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] = BLOCK_BOX;

				if (keys[DIK_D] && !preKeys[DIK_D])
				{
					//ブロックを動かす
					map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>((player.leftPos.x - player.width) / blockSize)] = BLOCK_FLOOR;
				}

				map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] = BLOCK_BOX;

			}
			else if (player.isRightReady)
			{
				if (keys[DIK_A] && !preKeys[DIK_A])
				{
					//ブロックを動かす
					map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>((player.rightPos.x + player.width) / blockSize)] = BLOCK_FLOOR;
				}

				map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] = BLOCK_BOX;

				//ブロックを動かす
				map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>((player.rightPos.x - player.width) / blockSize)] = BLOCK_FLOOR;
				map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] = BLOCK_BOX;
			}

		}

		//=========================
		//ステージギミックの更新処理
		//=========================
		/*---アイテム---*/
		if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_ITEM)
		{
			player.isItemGet = true;
		}


		/*---ダッシュパネル---*/

		if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_DASH)
		{
			

		}

		/*---ゴール---*/


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//=====================
		//背景の描画処理
		//=====================



		//=====================
		//マップチップの描画処理
		//=====================

		for (int i = 0; i < kMapSize; i++)
		{
			for (int j = 0; j < kMapSize; j++)
			{
				//床
				if (map[i][j] == BLOCK_FLOOR)
				{

					Novice::DrawSprite(static_cast<int>(blockSize) * i, static_cast<int>(blockSize) * j,
						floorBlockHandle, 1, 1, 0.0f, WHITE);
				}
				//壁
				if (map[i][j] == BLOCK_WALL)
				{

					Novice::DrawSprite(static_cast<int>(blockSize) * i, static_cast<int>(blockSize) * j,
						wallBlockHandle, 1, 1, 0.0f, WHITE);
				}
				//外枠
				if (map[i][j] == STAGE_WALL)
				{
					Novice::DrawBox(static_cast<int>(blockSize) * j, static_cast<int>(blockSize) * i,
						static_cast<int>(blockSize), static_cast<int>(blockSize),
						0.0f, 0xFFFFFFFF, kFillModeSolid);
				}
				//足場ブロック
				if (map[i][j] == BLOCK_BOX)
				{
					Novice::DrawBox(static_cast<int>(blockSize) * j, static_cast<int>(blockSize) * i,
						static_cast<int>(blockSize), static_cast<int>(blockSize),
						0.0f, 0xb50013FF, kFillModeSolid);
				}

				//アイテム
				if (map[i][j] == BLOCK_ITEM)
				{
					if (!player.isItemGet)
					{
						Novice::DrawBox(static_cast<int>(blockSize) * j, static_cast<int>(blockSize) * i,
							static_cast<int>(blockSize), static_cast<int>(blockSize),
							0.0f, 0xaaff3aFF, kFillModeSolid);
					}
					else
					{
						Novice::DrawSprite(static_cast<int>(blockSize)* i, static_cast<int>(blockSize)* j,
							floorBlockHandle, 1, 1, 0.0f, WHITE);
					}
				}

				//ゴール
				if (map[j][i] == BLOCK_GOOL)
				{
					Novice::DrawBox(static_cast<int>(blockSize) * j, static_cast<int>(blockSize) * i,
						static_cast<int>(blockSize), static_cast<int>(blockSize),
						0.0f, 0x92ff44FF, kFillModeSolid);
				}
				//ダッシュブロック
				if (map[j][i] == BLOCK_DASH)
				{
					Novice::DrawBox(static_cast<int>(blockSize) * j, static_cast<int>(blockSize) * i,
						static_cast<int>(blockSize), static_cast<int>(blockSize),
						0.0f, 0xfd7e00FF, kFillModeSolid);
				}
			}
		}

		//=======================
		//プレイヤーの描画処理
		//=======================

		Novice::DrawBox(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y),
			static_cast<int>(player.width), static_cast<int>(player.height), 0.0f, BLUE, kFillModeSolid);

		//=======================
		//ロボットの描画処理
		//=======================

		Novice::DrawBox(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y),
			static_cast<int>(robot.width), static_cast<int>(robot.height), 0.0f, 0xFFFF00FF, kFillModeSolid);

		//=====================
		//デバック表示
		//=====================
		Novice::ScreenPrintf(0, 520, "front:map[%d][%d]",
			static_cast<int>(player.frontPos.y / blockSize),
			static_cast<int>(player.frontPos.x / blockSize));

		Novice::ScreenPrintf(150, 520, "back:map[%d][%d]",
			static_cast<int>(player.backPos.y / blockSize),
			static_cast<int>(player.backPos.x / blockSize));

		Novice::ScreenPrintf(0, 560, "left:map[%d][%d]",
			static_cast<int>(player.leftPos.y / blockSize),
			static_cast<int>(player.leftPos.x / blockSize));

		Novice::ScreenPrintf(150, 560, "right:map[%d][%d]",
			static_cast<int>(player.rightPos.y / blockSize),
			static_cast<int>(player.rightPos.x / blockSize));

		Novice::ScreenPrintf(0, 600, "player.grabBlock:%d", player.grabBlock);

		Novice::ScreenPrintf(0, 620, "player.isFrontReady:%d", player.isFrontReady);
		Novice::ScreenPrintf(300, 620, "player.isBackReady:%d", player.isBackReady);
		Novice::ScreenPrintf(0, 640, "player.isLeftReady:%d", player.isLeftReady);
		Novice::ScreenPrintf(300, 640, "player.isRightReady:%d", player.isRightReady);

		Novice::ScreenPrintf(300, 520, "robot.pos.x:%d", static_cast<int> (robot.pos.x));

		if (!player.grabBlock)
		{
			if (player.direction == FRONT)
			{
				Novice::DrawLine(static_cast<int>(player.pos.x + (player.width / 2)), static_cast<int>(player.pos.y + (player.width / 2)),
					static_cast<int>(player.frontPos.x), static_cast<int>(player.frontPos.y), 0xFFF00FF);
			}
			else if (player.direction == BACK)
			{
				Novice::DrawLine(static_cast<int>(player.pos.x + (player.width / 2)), static_cast<int>(player.pos.y + (player.width / 2)),
					static_cast<int>(player.backPos.x), static_cast<int>(player.backPos.y), 0xFFF00FF);
			}
			else if (player.direction == LEFT)
			{
				Novice::DrawLine(static_cast<int>(player.pos.x + (player.width / 2)), static_cast<int>(player.pos.y + (player.width / 2)),
					static_cast<int>(player.leftPos.x), static_cast<int>(player.leftPos.y), 0xFFF00FF);
			}
			else if (player.direction == RIGHT)
			{
				Novice::DrawLine(static_cast<int>(player.pos.x + (player.width / 2)), static_cast<int>(player.pos.y + (player.width / 2)),
					static_cast<int>(player.rightPos.x), static_cast<int>(player.rightPos.y), 0xFFF00FF);
			}
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}