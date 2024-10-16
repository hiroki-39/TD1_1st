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
	BLOCK_GOOL,
	BLOCK_DASH,
	BLOCK_BOX,
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
	int grabBlock;
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
	int map[kMapSize][kMapSize] = { 0 };

	//マップの外枠
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{

			map[i][0] = { STAGE_WALL };
			map[i][7] = { STAGE_WALL };
			map[0][j] = { STAGE_WALL };
			map[7][j] = { STAGE_WALL };
		}
	}

	//マップの床
	for (int x = 2; x < 6; x++)
	{
		for (int y = 2; y < 6; y++)
		{
			map[x][y] = { BLOCK_FLOOR };
		}
	}

	//マップの壁
	for (int x = 1; x < 7; x++)
	{
		for (int y = 1; y < 7; y++)
		{
			map[x][1] = { BLOCK_WALL };
			map[x][6] = { BLOCK_WALL };
			map[1][y] = { BLOCK_WALL };
			map[6][y] = { BLOCK_WALL };
			map[4][4] = { BLOCK_BOX };
		}
	}



	/*---プレイヤー---*/
	Player player;
	player.pos.x = 128.0f;
	player.pos.y = 128.0f;
	player.centerPos.x = 160.0f;
	player.centerPos.y = 160.0f;
	player.width = blockSize;
	player.height = blockSize;
	player.speed = 64.0f;
	//ブロックの判定
	player.frontPos.x = player.centerPos.x;
	player.frontPos.y = player.centerPos.y + player.height;
	player.backPos.x = player.centerPos.x;
	player.backPos.y = player.centerPos.y - player.height;
	player.rightPos.x = player.centerPos.x + player.width;
	player.rightPos.y = player.centerPos.y;
	player.leftPos.x = player.centerPos.x - player.width;
	player.leftPos.y = player.centerPos.y;

	player.grabBlock = false;
	player.direction = FRONT;


	/*---ロボット---*/
	Robot robot;
	robot.pos.x = 64.0f;
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
		int isPush = false;


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

				//プレイヤーが壁とボックスに当たった時、座標を戻す
				if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] != BLOCK_FLOOR)
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

				if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
					map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL)
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

				//プレイヤーが壁とボックスに当たった時、座標を戻す
				if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] != BLOCK_FLOOR)
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

				if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
					map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL)
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

				//プレイヤーが壁とボックスに当たった時、座標を戻す
				if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] != BLOCK_FLOOR)
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

				if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
					map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL)
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

				//プレイヤーが壁とボックスに当たった時、座標を戻す
				if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] != BLOCK_FLOOR)
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
					map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL)
				{
					robot.pos.x -= robot.speed;
				}
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
					//向いている方向
					player.direction = BACK;

					//座標の更新
					player.pos.y -= player.speed;

					player.backPos.y -= player.speed;
					player.frontPos.y -= player.speed;
					player.leftPos.y -= player.speed;
					player.rightPos.y -= player.speed;

					//プレイヤーが壁とボックスに当たった時、座標を戻す
					if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL)
					{
						player.pos.y += player.speed;
						robot.pos.y += robot.speed;

						player.backPos.y += player.speed;
						player.frontPos.y += player.speed;
						player.leftPos.y += player.speed;
						player.rightPos.y += player.speed;
					}

					//ロボット
					robot.pos.y -= robot.speed;

					if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL)
					{
						robot.pos.y += robot.speed;
					}
				}

				if (keys[DIK_S] && !preKeys[DIK_S])
				{
					/*---プレイヤー---*/
					//向いている方向
					player.direction = FRONT;

					//座標の更新
					player.pos.y += player.speed;

					player.frontPos.y += player.speed;
					player.backPos.y += player.speed;
					player.leftPos.y += player.speed;
					player.rightPos.y += player.speed;

					//プレイヤーが壁とボックスに当たった時、座標を戻す
					if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL)
					{
						player.pos.y -= player.speed;
						robot.pos.y -= robot.speed;

						player.frontPos.y -= player.speed;
						player.backPos.y -= player.speed;
						player.leftPos.y -= player.speed;
						player.rightPos.y -= player.speed;
					}




					//ロボット
					robot.pos.y += robot.speed;
					if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL)
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
					//向いている方向
					player.direction = LEFT;

					//座標の更新
					player.pos.x -= player.speed;

					player.backPos.x -= player.speed;
					player.frontPos.x -= player.speed;
					player.leftPos.x -= player.speed;
					player.rightPos.x -= player.speed;

					//プレイヤーが壁とボックスに当たった時、座標を戻す
					if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL)
					{
						player.pos.x += player.speed;
						robot.pos.x += robot.speed;

						player.backPos.x += player.speed;
						player.frontPos.x += player.speed;
						player.leftPos.x += player.speed;
						player.rightPos.x += player.speed;
					}

					//ロボット
					robot.pos.x -= robot.speed;

					if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL)
					{
						robot.pos.x += robot.speed;
					}
				}

				if (keys[DIK_D] && !preKeys[DIK_D])
				{
					/*---プレイヤー---*/
					//向いている方向
					player.direction = RIGHT;

					//座標の更新
					player.pos.x += player.speed;

					player.backPos.x += player.speed;
					player.frontPos.x += player.speed;
					player.leftPos.x += player.speed;
					player.rightPos.x += player.speed;

					//プレイヤーが壁とボックスに当たった時、座標を戻す
					if (map[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL)
					{
						player.pos.x -= player.speed;
						robot.pos.x -= robot.speed;

						player.backPos.x -= player.speed;
						player.frontPos.x -= player.speed;
						player.leftPos.x -= player.speed;
						player.rightPos.x -= player.speed;
					}

					//ロボット

					robot.pos.x += robot.speed;

					if (map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						map[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL)
					{
						robot.pos.x -= robot.speed;
					}
				}
			}
		}


		/*---ブロックを動かす処理---*/

		 //ブロックがプレイヤーが向いてる方向にあるからどうかの判定
		if (player.direction == FRONT || player.direction == BACK)
		{
			if (map[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_BOX ||
				map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_BOX)
			{

				isPush = true;
			}
			else
			{
				isPush = false;
			}
		}
		else if (player.direction == LEFT || player.direction == RIGHT)
		{
			if (map[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_BOX ||
				map[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_BOX)
			{

				isPush = true;
			}
			else
			{
				isPush = false;
			}
		}

		// ブロックの判定
		if (isPush)
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

		if (keys[DIK_SPACE] && keys[DIK_S] && !preKeys[DIK_S])
		{
			if (player.grabBlock)
			{
				//ブロックを動かす
				map[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = { BLOCK_BOX };
			}

		}


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
				if (map[j][i] == BLOCK_FLOOR)
				{

					Novice::DrawSprite(static_cast<int>(blockSize) * i, static_cast<int>(blockSize) * j,
						floorBlockHandle, 1, 1, 0.0f, WHITE);
				}
				//壁
				if (map[j][i] == BLOCK_WALL)
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
						0.0f, RED, kFillModeSolid);
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
		Novice::ScreenPrintf(0, 620, "player.isPush:%d", isPush);

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

		if (player.grabBlock)
		{
			if (player.direction == FRONT || player.direction == BACK)
			{
				Novice::DrawLine(static_cast<int>(player.pos.x + (player.width / 2)), static_cast<int>(player.pos.y + (player.width / 2)),
					static_cast<int>(player.frontPos.x + (player.width / 2)), static_cast<int>(player.frontPos.y), 0xFFF00FF);

				Novice::DrawLine(static_cast<int>(player.pos.x + (player.width / 2)), static_cast<int>(player.pos.y + (player.width / 2)),
					static_cast<int>(player.backPos.x + (player.width / 2)), static_cast<int>(player.backPos.y), 0xFFF00FF);

			}
			else  if (player.direction == LEFT || player.direction == RIGHT)
			{
				Novice::DrawLine(static_cast<int>(player.pos.x + (player.width / 2)), static_cast<int>(player.pos.y + (player.width / 2)),
					static_cast<int>(player.leftPos.x), static_cast<int>(player.leftPos.y + (player.width / 2)), 0xFFF00FF);

				Novice::DrawLine(static_cast<int>(player.pos.x + (player.width / 2)), static_cast<int>(player.pos.y + (player.width / 2)),
					static_cast<int>(player.rightPos.x), static_cast<int>(player.rightPos.y + (player.width / 2)), 0xFFF00FF);
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