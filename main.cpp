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
	BLOCK_DASH_UP,
	BLOCK_DASH_DOWN,
	BLOCK_DASH_LEFT,
	BLOCK_DASH_RIGHT,
	BLOCK_ITEM,
	BLOCK_AIR,
};

enum Scene
{
	TITLE,
	STAGE1,
	STAGE2,
	STAGE3,
	STAGE4,
	STAGE5,
	IN_STAGE1,
	IN_STAGE2,
	GAME_OVER,
	GAME_CLEAR,
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
	int animationCount;
	int frameCount;
	int isMoving;
	int canMoveBlock;
	int canMove;
}Player;

typedef struct Robot
{
	Vector2 pos;
	Vector2 centerPos;
	float speed;
	float width;
	float height;
	int canMove;
}Robot;

Scene scene = TITLE;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 896, 760);

	//640

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//========================
	//変数の宣言と初期化
	//========================

	/*---マップチップ---*/

	const int kMapSizeX = 12;
	const int kMapSizeY = 9;
	float blockSize = 64.0f;

	//map

	// 0 = ステージの壁 (STAGE_WALL)
	// 1 = マップの壁 (BLOCK_WALL)
	// 2 = マップの床 (BLOCK_FLOOR)
	// 3 = 動かせるブロック (BLOCK_BOX)
	// 4 = ゴール地点(BLOCK_GOOL)
	// 5 = ダッシュパネル上 (BLOCK_DASH_UP)
	// 6 = ダッシュパネル下 (BLOCK_DASH_DOWN)
	// 7 = ダッシュパネル左 (BLOCK_DASH_LEFT)
	// 8 = ダッシュパネル右 (BLOCK_DASH_RIGHT)
	// 9 = アイテム (BLOCK_ITEM)
	// 10 = 空白 (BLOCK_AIR)

	//ステージ1
	int stage1[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
		{ 10,10,10, 0, 1, 1, 1, 1, 1, 1, 0,10, },
		{ 10,10,10, 0, 1, 2, 2, 2, 2, 1, 0,10, },
		{ 10,10,10, 0, 1, 1, 3, 2, 2, 1, 0,10, },
		{ 10,10,10, 0, 1, 2, 2, 2, 2, 1, 0,10, },
		{ 10,10,10, 0, 1, 2, 3, 2, 2, 6, 0,10, },
		{ 10,10,10, 0, 1, 1, 1, 1, 9, 4, 0,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
	};

	//ステージ2
	int stage2[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
		{ 10,10,10, 0, 1, 1, 1, 1, 1, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 3, 6, 0,10,10, },
		{ 10,10,10, 0, 1, 9, 2, 2, 4, 0,10,10, },
		{ 10,10,10, 0, 1, 1, 1, 1, 5, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};



	/*---プレイヤー---*/
	Player player;
	player.pos.x = 320.0f;
	player.pos.y = 192.0f;
	player.width = blockSize;
	player.height = blockSize;
	player.centerPos.x = player.pos.x + player.width / 2.0f;
	player.centerPos.y = player.pos.y + player.height / 2.0f;
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
	player.isMoving = false;
	player.canMove = false;
	player.canMoveBlock = false;
	player.direction = FRONT;

	//アニメーション
	player.animationCount = 0;
	player.frameCount = 0;

	/*---ロボット---*/
	Robot robot;
	robot.pos.x = 256.0f;
	robot.pos.y = 128.0f;
	robot.width = blockSize;
	robot.height = blockSize;
	robot.centerPos.x = robot.pos.x + robot.width / 2.0f;
	robot.centerPos.y = robot.pos.y + robot.height / 2.0f;
	robot.speed = 64.0f;

	robot.canMove = true;

	/*---制限時間---*/
	int secondTimer = 0;
	int limitTimer = 180;
	int isTimerStart = false;

	/*---クリア報酬---*/
	int stars = 0;

	/*---画像の読み込み---*/
	//プレイヤー
	int playerHandle[4];
	playerHandle[0] = Novice::LoadTexture("./Resources/images/player_front.png");
	playerHandle[1] = Novice::LoadTexture("./Resources/images/player_left.png");
	playerHandle[2] = Novice::LoadTexture("./Resources/images/player_back.png");
	playerHandle[3] = Novice::LoadTexture("./Resources/images/player_right.png");

	int playerGradHandle[4];
	playerGradHandle[0] = Novice::LoadTexture("./Resources/images/player_grab_front.png");
	playerGradHandle[1] = Novice::LoadTexture("./Resources/images/player_grab_back.png");
	playerGradHandle[2] = Novice::LoadTexture("./Resources/images/player_grab_left.png");
	playerGradHandle[3] = Novice::LoadTexture("./Resources/images/player_grab_right.png");

	//ロボット
	int robotHandle[4];
	robotHandle[0] = Novice::LoadTexture("./Resources/images/robot_front.png");
	robotHandle[1] = Novice::LoadTexture("./Resources/images/robot_left.png");
	robotHandle[2] = Novice::LoadTexture("./Resources/images/robot_back.png");
	robotHandle[3] = Novice::LoadTexture("./Resources/images/robot_right.png");

	int robotHoldItemHandle[4];
	robotHoldItemHandle[0] = Novice::LoadTexture("./Resources/images/robot_item_front.png");
	robotHoldItemHandle[1] = Novice::LoadTexture("./Resources/images/robot_item_left.png");
	robotHoldItemHandle[2] = Novice::LoadTexture("./Resources/images/robot_item_back.png");
	robotHoldItemHandle[3] = Novice::LoadTexture("./Resources/images/robot_item_right.png");


	//マップチップ
	int wallBlockHandle = Novice::LoadTexture("./Resources/images/blockWall.png");
	int floorBlockHandle = Novice::LoadTexture("./Resources/images/floor.png");
	int DashPanelHandle = Novice::LoadTexture("./Resources/images/conveyor.png");
	int goalHandle = Novice::LoadTexture("./Resources/images/goal.png");
	int stageWallHandle = Novice::LoadTexture("./Resources/images/stageWall.png");
	int blockBoxHandle = Novice::LoadTexture("./Resources/images/box.png");
	int ItemHandle = Novice::LoadTexture("./Resources/images/item.png");

	//背景
	int backgroundHandle = Novice::LoadTexture("./Resources/images/background.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		switch (scene)
		{
		case TITLE:
#pragma region TITLE
			///
			///↓更新処理ここから
			///

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				scene = STAGE1;
			}

			///
			/// ↓描画処理ここから
			///


			Novice::ScreenPrintf(340, 360, ">GAMESTART");

			break;
#pragma endregion
		case STAGE1:
#pragma region STAGE1

			///
			///↓更新処理ここから
			///

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				//マップをステージ1に変える
				isTimerStart = true;
				scene = IN_STAGE1;

			}

			if (keys[DIK_S] && !preKeys[DIK_S])
			{
				scene = STAGE2;
			}

			// 描画処理
			Novice::ScreenPrintf(340, 360, ">STAGE1");
			Novice::ScreenPrintf(340, 380, "STAGE2");
			Novice::ScreenPrintf(340, 400, "STAGE3");
			Novice::ScreenPrintf(340, 420, "STAGE4");
			Novice::ScreenPrintf(340, 440, "STAGE5");

			break;
#pragma endregion
		case STAGE2:
#pragma region STAGE2
			///
			///↓更新処理ここから
			///

			if (keys[DIK_W] && !preKeys[DIK_W])
			{
				scene = STAGE1;
			}

			if (keys[DIK_S] && !preKeys[DIK_S])
			{
				scene = STAGE3;
			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				scene = IN_STAGE2;
			}

			///
			/// ↓描画処理ここから
			///

			Novice::ScreenPrintf(340, 360, "STAGE1");
			Novice::ScreenPrintf(340, 380, ">STAGE2");
			Novice::ScreenPrintf(340, 400, "STAGE3");
			Novice::ScreenPrintf(340, 420, "STAGE4");
			Novice::ScreenPrintf(340, 440, "STAGE5");

			break;
#pragma endregion
		case STAGE3:
#pragma region STAGE3
			///
			///↓更新処理ここから
			///

			if (keys[DIK_W] && !preKeys[DIK_W])
			{
				scene = STAGE2;
			}

			if (keys[DIK_S] && !preKeys[DIK_S])
			{
				scene = STAGE4;
			}

			///
			/// ↓描画処理ここから
			///

			Novice::ScreenPrintf(340, 360, "STAGE1");
			Novice::ScreenPrintf(340, 380, "STAGE2");
			Novice::ScreenPrintf(340, 400, ">STAGE3");
			Novice::ScreenPrintf(340, 420, "STAGE4");
			Novice::ScreenPrintf(340, 440, "STAGE5");

			break;
#pragma endregion
		case STAGE4:
#pragma region STAGE4
			///
			///↓更新処理ここから
			///

			if (keys[DIK_W] && !preKeys[DIK_W])
			{
				scene = STAGE3;
			}

			if (keys[DIK_S] && !preKeys[DIK_S])
			{
				scene = STAGE5;
			}

			///
			/// ↓描画処理ここから
			///

			Novice::ScreenPrintf(340, 360, "STAGE1");
			Novice::ScreenPrintf(340, 380, "STAGE2");
			Novice::ScreenPrintf(340, 400, "STAGE3");
			Novice::ScreenPrintf(340, 420, ">STAGE4");
			Novice::ScreenPrintf(340, 440, "STAGE5");

			break;
#pragma endregion
		case STAGE5:
#pragma region STAGE5
			///
			///↓更新処理ここから
			///

			if (keys[DIK_W] && !preKeys[DIK_W])
			{
				scene = STAGE4;
			}

			///
			/// ↓描画処理ここから
			///

			Novice::ScreenPrintf(340, 360, "STAGE1");
			Novice::ScreenPrintf(340, 380, "STAGE2");
			Novice::ScreenPrintf(340, 400, "STAGE3");
			Novice::ScreenPrintf(340, 420, "STAGE4");
			Novice::ScreenPrintf(340, 440, ">STAGE5");

			break;
#pragma endregion
		case IN_STAGE1:
#pragma region IN_STAGE1

			///
			/// ↓更新処理ここから
			///

			//==========================
			//プレイヤーとロボットの更新処理
			//==========================

			//アニメーションの切り替え
			if (!keys[DIK_W] || !keys[DIK_A] || !keys[DIK_S] || !keys[DIK_D])
			{
				player.isMoving = false;
			}

			/*---移動処理---*/
			if (!player.grabBlock)
			{
				//上
				if (keys[DIK_W] && !preKeys[DIK_W])
				{

					/*---プレイヤー---*/
					player.isMoving = true;

					//向いている方向
					player.direction = BACK;

					//座標の更新
					player.pos.y -= player.speed;
					player.centerPos.y -= player.speed;
					robot.centerPos.y -= robot.speed;

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
					if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_GOOL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_UP ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_DOWN ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_LEFT ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_RIGHT ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX)
					{
						player.pos.y += player.speed;
						robot.pos.y += robot.speed;
						player.centerPos.y += player.speed;
						robot.centerPos.y += robot.speed;

						player.backPos.y += player.speed;
						player.frontPos.y += player.speed;
						player.leftPos.y += player.speed;
						player.rightPos.y += player.speed;

					}

					/*---ロボット---*/
					robot.pos.y -= robot.speed;

					//ロボットが壁以外に当たった時、座標を戻す
					if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
						stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
					{
						robot.pos.y += robot.speed;
						robot.centerPos.y += robot.speed;
					}
				}


				//左
				if (keys[DIK_A] && !preKeys[DIK_A])
				{
					//プレイヤー

					player.isMoving = true;
					player.direction = LEFT;

					//座標の更新
					player.pos.x -= player.speed;
					player.centerPos.x -= player.speed;
					robot.centerPos.x -= robot.speed;

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
					if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_GOOL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_UP ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_DOWN ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_LEFT ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_RIGHT ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX)
					{
						player.pos.x += player.speed;
						robot.pos.x += robot.speed;
						player.centerPos.x += player.speed;
						robot.centerPos.x += robot.speed;

						player.backPos.x += player.speed;
						player.frontPos.x += player.speed;
						player.leftPos.x += player.speed;
						player.rightPos.x += player.speed;
					}

					/*---ロボット---*/
					robot.pos.x -= robot.speed;

					//ロボットが壁以外に当たった時、座標を戻す
					if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
						stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
					{
						robot.pos.x += robot.speed;
						robot.centerPos.x += robot.speed;
					}
				}

				//下
				if (keys[DIK_S] && !preKeys[DIK_S])
				{
					/*---プレイヤー---*/
					//向いている方向
					player.isMoving = true;
					player.direction = FRONT;

					//座標の更新
					player.pos.y += player.speed;
					player.centerPos.y += player.speed;
					robot.centerPos.y += robot.speed;

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
					if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_GOOL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_UP ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_DOWN ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_LEFT ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_RIGHT ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX)
					{
						player.pos.y -= player.speed;
						robot.pos.y -= robot.speed;
						player.centerPos.y -= player.speed;
						robot.centerPos.y -= robot.speed;

						player.frontPos.y -= player.speed;
						player.backPos.y -= player.speed;
						player.leftPos.y -= player.speed;
						player.rightPos.y -= player.speed;

					}

					/*---ロボット---*/
					robot.pos.y += robot.speed;


					//ロボットが壁以外に当たった時、座標を戻す
					if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
						stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
					{
						robot.pos.y -= robot.speed;
						robot.centerPos.y -= robot.speed;

					}
				}

				//下
				if (keys[DIK_D] && !preKeys[DIK_D])
				{
					/*---プレイヤー---*/
					//向いている方向
					player.direction = RIGHT;
					player.isMoving = true;
					//座標の更新
					player.pos.x += player.speed;
					player.centerPos.x += player.speed;
					robot.centerPos.x += robot.speed;

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
					if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_GOOL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_UP ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_DOWN ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_LEFT ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_RIGHT ||
						stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX)
					{
						player.pos.x -= player.speed;
						robot.pos.x -= robot.speed;
						player.centerPos.x -= player.speed;
						robot.centerPos.x -= robot.speed;

						player.backPos.x -= player.speed;
						player.frontPos.x -= player.speed;
						player.leftPos.x -= player.speed;
						player.rightPos.x -= player.speed;
					}

					/*---ロボット---*/
					robot.pos.x += robot.speed;


					//ロボットが壁以外に当たった時、座標を戻す
					if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
						stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
						stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
					{
						robot.pos.x -= robot.speed;
						robot.centerPos.x -= robot.speed;
					}
				}

				//ロボットが動くこと出来るかの判定
				robot.canMove = true;

			}

			//足場ブロックがプレイヤーが向いてる方向にあるからどうかの判定とロボットが足場ブロックの上にいないかの判定
			if (player.direction == FRONT)
			{
				if (stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_BOX)
				{
					if (player.frontPos.x == robot.centerPos.x && player.frontPos.y == robot.centerPos.y)
					{
						player.isFrontReady = false;
					}
					else
					{
						player.isFrontReady = true;
						//向きの固定
						player.direction = FRONT;
					}
				}
			}
			else if (player.direction == BACK)
			{
				if (stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_BOX)
				{
					if (player.backPos.x == robot.centerPos.x && player.backPos.y == robot.centerPos.y)
					{
						player.isBackReady = false;

					}
					else
					{
						player.isBackReady = true;
						//向きの固定
						player.direction = BACK;
					}
				}
			}
			else if (player.direction == LEFT)
			{
				if (stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_BOX)
				{
					if (player.leftPos.y == robot.centerPos.y && player.leftPos.x == robot.centerPos.x)
					{
						player.isLeftReady = false;
						//向きの固定
						player.direction = LEFT;
					}
					else
					{
						player.isLeftReady = true;
						//向きの固定
						player.direction = LEFT;
					}
				}
			}
			else if (player.direction == RIGHT)
			{
				if (stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_BOX)
				{
					if (player.rightPos.y == robot.centerPos.y && player.rightPos.x == robot.centerPos.x)
					{
						player.isRightReady = false;
					}
					else
					{
						player.isRightReady = true;
						//向きの固定
						player.direction = RIGHT;
					}
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
					robot.speed = 64.0f;
					player.grabBlock = false;
				}

			}

			//stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] != BLOCK_BOX

			//ブロックを掴んでいる時
			if (player.grabBlock)
			{

				if (player.pos.x == robot.pos.x || player.pos.y == robot.pos.y)
				{
					robot.speed = 0.0f;
				}

				//上下
				if (player.direction == FRONT || player.direction == BACK)
				{
					//上
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

						//プレイヤーがブロックを動かせるかどうかの判定
						player.canMoveBlock = true;

						//プレイヤーが壁とボックスに当たった時、座標を戻す
						if (player.isBackReady)
						{
							if (stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_GOOL ||
								stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_WALL ||
								stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_DASH_UP ||
								stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_DASH_DOWN ||
								stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_DASH_LEFT ||
								stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_DASH_RIGHT ||
								stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_BOX ||
								stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] == BLOCK_ITEM)
							{
								player.pos.y += player.speed;
								robot.pos.y += robot.speed;
								player.centerPos.y += player.speed;

								player.backPos.y += player.speed;
								player.frontPos.y += player.speed;
								player.leftPos.y += player.speed;
								player.rightPos.y += player.speed;

								player.canMoveBlock = false;
							}
						}
						else if (player.isFrontReady)
						{
							if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_UP ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_DOWN ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_LEFT ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_RIGHT ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_ITEM)
							{
								player.pos.y += player.speed;
								robot.pos.y += robot.speed;
								player.centerPos.y += player.speed;

								player.frontPos.y += player.speed;
								player.backPos.y += player.speed;
								player.leftPos.y += player.speed;
								player.rightPos.y += player.speed;

								player.canMoveBlock = false;
							}
						}

						/*---ロボット---*/
						robot.pos.y -= robot.speed;

						if (robot.canMove)
						{
							robot.centerPos.y -= robot.speed;
						}


						//ロボットが壁以外に当たった時、座標を戻す
						if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
							stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
							stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
						{
							robot.pos.y += robot.speed;
							robot.centerPos.y += robot.speed;
						}
					}

					//下
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

						//プレイヤーがブロックを動かせるかどうかの判定
						player.canMoveBlock = true;

						//プレイヤーが壁とボックスに当たった時、座標を戻す
						if (player.isBackReady)
						{
							if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_UP ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_DOWN ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_LEFT ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_RIGHT ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_ITEM)
							{
								player.pos.y -= player.speed;
								robot.pos.y -= robot.speed;
								player.centerPos.y -= player.speed;

								player.frontPos.y -= player.speed;
								player.backPos.y -= player.speed;
								player.leftPos.y -= player.speed;
								player.rightPos.y -= player.speed;

								player.canMoveBlock = false;
							}
						}
						else if (player.isFrontReady)
						{
							if (stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_GOOL ||
								stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_WALL ||
								stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_DASH_UP ||
								stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_DASH_DOWN ||
								stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_DASH_LEFT ||
								stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_DASH_RIGHT ||
								stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_BOX ||
								stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] == BLOCK_ITEM)
							{
								player.pos.y -= player.speed;
								robot.pos.y -= robot.speed;
								player.centerPos.y -= player.speed;

								player.frontPos.y -= player.speed;
								player.backPos.y -= player.speed;
								player.leftPos.y -= player.speed;
								player.rightPos.y -= player.speed;

								player.canMoveBlock = false;
							}
						}

						/*---ロボット---*/
						robot.pos.y += robot.speed;
						robot.centerPos.y += robot.speed;

						//ロボットが壁以外に当たった時、座標を戻す
						if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
							stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
							stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
						{
							robot.pos.y -= robot.speed;
							robot.centerPos.y -= robot.speed;
						}
					}
				}
				else if (player.direction == LEFT || player.direction == RIGHT)
				{
					//左
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

						//プレイヤーがブロックを動かせるかどうかの判定
						player.canMoveBlock = true;

						//プレイヤーが壁とボックスに当たった時、座標を戻す
						if (player.isLeftReady)
						{
							if (stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_GOOL ||
								stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_WALL ||
								stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_DASH_UP ||
								stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_DASH_DOWN ||
								stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_DASH_LEFT ||
								stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_DASH_RIGHT ||
								stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_BOX ||
								stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] == BLOCK_ITEM)
							{
								player.pos.x += player.speed;
								robot.pos.x += robot.speed;
								player.centerPos.x += player.speed;

								player.backPos.x += player.speed;
								player.frontPos.x += player.speed;
								player.leftPos.x += player.speed;
								player.rightPos.x += player.speed;

								player.canMoveBlock = false;
							}
						}
						else if (player.isRightReady)
						{
							if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_UP ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_DOWN ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_LEFT ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_RIGHT ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_ITEM)
							{
								player.pos.x += player.speed;
								robot.pos.x += robot.speed;
								player.centerPos.x += player.speed;

								player.backPos.x += player.speed;
								player.frontPos.x += player.speed;
								player.leftPos.x += player.speed;
								player.rightPos.x += player.speed;

								player.canMoveBlock = false;
							}
						}

						/*---ロボット---*/
						robot.pos.x -= robot.speed;
						robot.centerPos.x -= robot.speed;

						//ロボットが壁以外に当たった時、座標を戻す
						if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
							stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
							stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
						{
							robot.pos.x += robot.speed;
							robot.centerPos.x += robot.speed;
						}
					}

					//右
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

						//プレイヤーがブロックを動かせるかどうかの判定
						player.canMoveBlock = true;

						//プレイヤーが壁とボックスに当たった時、座標を戻す
						if (player.isLeftReady)
						{
							if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_UP ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_DOWN ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_LEFT ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_DASH_RIGHT ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_BOX ||
								stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_ITEM)
							{
								player.pos.x -= player.speed;
								robot.pos.x -= robot.speed;
								player.centerPos.x -= player.speed;

								player.backPos.x -= player.speed;
								player.frontPos.x -= player.speed;
								player.leftPos.x -= player.speed;
								player.rightPos.x -= player.speed;

								player.canMoveBlock = false;
							}
						}
						else if (player.isRightReady)
						{
							if (stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_GOOL ||
								stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_WALL ||
								stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == STAGE_WALL ||
								stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_DASH_UP ||
								stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_DASH_DOWN ||
								stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_DASH_LEFT ||
								stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_DASH_RIGHT ||
								stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_BOX ||
								stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] == BLOCK_ITEM)
							{
								player.pos.x -= player.speed;
								robot.pos.x -= robot.speed;
								player.centerPos.x -= player.speed;

								player.backPos.x -= player.speed;
								player.frontPos.x -= player.speed;
								player.leftPos.x -= player.speed;
								player.rightPos.x -= player.speed;

								player.canMoveBlock = false;
							}
						}

						/*---ロボット---*/
						robot.pos.x += robot.speed;
						robot.centerPos.x += robot.speed;

						//ロボットが壁以外に当たった時、座標を戻す
						if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_FLOOR ||
							stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == STAGE_WALL ||
							stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_ITEM)
						{
							robot.pos.x -= robot.speed;
							robot.centerPos.x -= robot.speed;

						}
					}
				}

			}

			/*---ブロックを動かす処理---*/
			if (player.grabBlock)
			{
				if (player.isBackReady)
				{
					if (player.canMoveBlock)
					{
						//ブロック後ろに動かす
						stage1[static_cast<int>((player.backPos.y + player.height) / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = BLOCK_FLOOR;
						stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = BLOCK_BOX;


						//ブロック前に動かす
						if (keys[DIK_S] && !preKeys[DIK_S])
						{
							stage1[static_cast<int>((player.backPos.y - player.height) / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = BLOCK_FLOOR;
						}

						stage1[static_cast<int>(player.backPos.y / blockSize)][static_cast<int>(player.backPos.x / blockSize)] = BLOCK_BOX;
					}


				}
				else if (player.isFrontReady)
				{
					if (player.canMoveBlock)
					{
						if (keys[DIK_W] && !preKeys[DIK_W])
						{
							//ブロック後ろに動かす
							stage1[static_cast<int>((player.frontPos.y + player.height) / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] = BLOCK_FLOOR;
						}

						stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] = { BLOCK_BOX };

						//ブロック前に動かす
						stage1[static_cast<int>((player.frontPos.y - player.height) / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] = BLOCK_FLOOR;
						stage1[static_cast<int>(player.frontPos.y / blockSize)][static_cast<int>(player.frontPos.x / blockSize)] = BLOCK_BOX;
					}

				}
				else if (player.isLeftReady)
				{
					if (player.canMoveBlock)
					{
						//ブロックを動かす
						stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>((player.leftPos.x + player.width) / blockSize)] = BLOCK_FLOOR;

						stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] = BLOCK_BOX;

						if (keys[DIK_D] && !preKeys[DIK_D])
						{
							//ブロックを動かす
							stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>((player.leftPos.x - player.width) / blockSize)] = BLOCK_FLOOR;
						}

						stage1[static_cast<int>(player.leftPos.y / blockSize)][static_cast<int>(player.leftPos.x / blockSize)] = BLOCK_BOX;

					}

				}
				else if (player.isRightReady)
				{
					if (player.canMoveBlock)
					{
						if (keys[DIK_A] && !preKeys[DIK_A])
						{
							//ブロックを動かす
							stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>((player.rightPos.x + player.width) / blockSize)] = BLOCK_FLOOR;
						}

						stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] = BLOCK_BOX;

						//ブロックを動かす
						stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>((player.rightPos.x - player.width) / blockSize)] = BLOCK_FLOOR;
						stage1[static_cast<int>(player.rightPos.y / blockSize)][static_cast<int>(player.rightPos.x / blockSize)] = BLOCK_BOX;
					}

				}


			}


			//==============================
			//フレームタイマーのカウント
			//==============================

			player.frameCount++;

			if (player.frameCount > 59)
			{
				player.frameCount = 0;
			}

			player.animationCount = player.frameCount / 15;


			//==============================
			//ステージギミックの更新処理
			//==============================

			/*---アイテムの更新処理---*/
			if (stage1[static_cast<int>(player.pos.y / blockSize)][static_cast<int>(player.pos.x / blockSize)] == BLOCK_ITEM)
			{
				player.isItemGet = true;
			}

			/*---ゴールの更新処理---*/
			if (player.isItemGet)
			{
				if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_GOOL)
				{
					scene = GAME_CLEAR;
				}
			}

			/*---ダッシュパネルの更新処理---*/
			if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_DASH_UP)
			{
				robot.pos.y -= 0.5f;
			}

			if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_DASH_DOWN)
			{
				robot.pos.y += 0.5f;
			}

			if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_DASH_LEFT)
			{
				robot.pos.x -= 0.5f;
			}

			if (stage1[static_cast<int>(robot.pos.y / blockSize)][static_cast<int>(robot.pos.x / blockSize)] == BLOCK_DASH_RIGHT)
			{
				robot.pos.x += 0.5f;
			}


			//===========================
			// 制限時間タイマー
			//===========================

			if (isTimerStart)
			{
				if (limitTimer > 0)
				{
					secondTimer++;

					if (secondTimer >= 60)
					{
						limitTimer--;
						secondTimer = 0;
					}
				}
			}

			//制限時間が0秒になったらシーンをゲームオーバーに切り替え
			if (limitTimer <= 0)
			{
				scene = GAME_OVER;
			}

			//===========================
			// クリア報酬
			//===========================

			if (player.isItemGet)
			{
				if (limitTimer > 90)
				{
					//星3にする
					stars = 3;
				}
				else if (limitTimer > 30)
				{
					stars = 2;
				}
				else
				{
					stars = 1;
				}
			}
			else
			{
				stars = 0;
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

			Novice::DrawSprite(0, 0, backgroundHandle, 1, 1, 0.0f, WHITE);

			//=====================
			//マップチップの描画処理
			//=====================

			for (int y = 0; y < kMapSizeY; y++)
			{
				for (int x = 0; x < kMapSizeX; x++)
				{
					//床
					if (stage1[y][x] == BLOCK_FLOOR)
					{

						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							floorBlockHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}

					//壁
					if (stage1[y][x] == BLOCK_WALL)
					{

						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							wallBlockHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}

					//外枠
					if (stage1[y][x] == STAGE_WALL)
					{
						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							stageWallHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}

					//足場ブロック
					if (stage1[y][x] == BLOCK_BOX)
					{
						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							blockBoxHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}

					//アイテム
					if (stage1[y][x] == BLOCK_ITEM)
					{
						if (!player.isItemGet)
						{
							Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
								ItemHandle, 0.5f, 0.5f, 0.0f, WHITE);
						}
						else
						{
							Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
								floorBlockHandle, 0.5f, 0.5f, 0.0f, WHITE);
						}
					}

					//ゴール
					if (stage1[y][x] == BLOCK_GOOL)
					{
						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							goalHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}

					//ダッシュブロック
					if (stage1[y][x] == BLOCK_DASH_UP)
					{
						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							DashPanelHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}

					if (stage1[y][x] == BLOCK_DASH_DOWN)
					{
						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							DashPanelHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}

					if (stage1[y][x] == BLOCK_DASH_LEFT)
					{
						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							DashPanelHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}

					if (stage1[y][x] == BLOCK_DASH_RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(blockSize) * x, static_cast<int>(blockSize) * y,
							DashPanelHandle, 0.5f, 0.5f, 0.0f, WHITE);
					}
				}
			}

			//=======================
			//プレイヤーの描画処理
			//=======================

			if (player.isMoving)
			{
				if (!player.grabBlock)
				{
					if (player.direction == FRONT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[2], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == BACK)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[0], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == LEFT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[1], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[3], 0.5f, 0.5f, 0.0f, WHITE);
					}
				}
				else
				{
					if (player.direction == FRONT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[1], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == BACK)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[0], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == LEFT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[2], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[3], 0.5f, 0.5f, 0.0f, WHITE);
					}
				}
			}
			else
			{
				if (!player.grabBlock)
				{
					if (player.direction == FRONT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[2], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == BACK)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[0], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == LEFT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[1], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[3], 0.5f, 0.5f, 0.0f, WHITE);
					}
				}
				else
				{
					if (player.direction == FRONT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[1], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == BACK)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[0], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == LEFT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[2], 0.5f, 0.5f, 0.0f, WHITE);
					}
					else if (player.direction == RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[3], 0.5f, 0.5f, 0.0f, WHITE);
					}
				}
			}


			//=======================
			//ロボットの描画処理
			//=======================

			if (!player.isItemGet)
			{
				if (player.direction == FRONT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHandle[2], 0.5f, 0.5f, 0.0f, WHITE);
				}
				else if (player.direction == BACK)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHandle[0], 0.5f, 0.5f, 0.0f, WHITE);
				}
				else if (player.direction == LEFT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHandle[1], 0.5f, 0.5f, 0.0f, WHITE);
				}
				else if (player.direction == RIGHT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHandle[3], 0.5f, 0.5f, 0.0f, WHITE);
				}
			}
			else
			{
				if (player.direction == FRONT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHoldItemHandle[2], 0.5f, 0.5f, 0.0f, WHITE);
				}
				else if (player.direction == BACK)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHoldItemHandle[0], 0.5f, 0.5f, 0.0f, WHITE);
				}
				else if (player.direction == LEFT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHoldItemHandle[1], 0.5f, 0.5f, 0.0f, WHITE);
				}
				else if (player.direction == RIGHT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHoldItemHandle[3], 0.5f, 0.5f, 0.0f, WHITE);
				}
			}

			//=====================
			//デバック表示
			//=====================

			Novice::ScreenPrintf(20, 40, "%d:%02d", limitTimer / 60, limitTimer % 60);
			Novice::ScreenPrintf(20, 60, "stars:%d", stars);

			Novice::ScreenPrintf(0, 600, "front:stage1[%d][%d]",
				static_cast<int>(player.frontPos.y / blockSize),
				static_cast<int>(player.frontPos.x / blockSize));

			Novice::ScreenPrintf(170, 600, "back:stage1[%d][%d]",
				static_cast<int>(player.backPos.y / blockSize),
				static_cast<int>(player.backPos.x / blockSize));

			Novice::ScreenPrintf(0, 620, "left:stage1[%d][%d]",
				static_cast<int>(player.leftPos.y / blockSize),
				static_cast<int>(player.leftPos.x / blockSize));

			Novice::ScreenPrintf(170, 620, "right:stage1[%d][%d]",
				static_cast<int>(player.rightPos.y / blockSize),
				static_cast<int>(player.rightPos.x / blockSize));

			Novice::ScreenPrintf(350, 600, "player.backPos.x.:%d", static_cast<int>(player.backPos.x));
			Novice::ScreenPrintf(540, 600, "player.backPos.y:%d", static_cast<int>(player.backPos.y));
			Novice::ScreenPrintf(350, 620, "robot.centerPos.x:%d", static_cast<int>(robot.centerPos.x));
			Novice::ScreenPrintf(540, 620, "robot.centerPos.y:%d", static_cast<int>(robot.centerPos.y));
			Novice::ScreenPrintf(350, 660, "player.isItemGet:%d", player.isItemGet);

			Novice::ScreenPrintf(0, 660, "player.grabBlock:%d", player.grabBlock);

			Novice::ScreenPrintf(0, 700, "player.isFrontReady:%d", player.isFrontReady);
			Novice::ScreenPrintf(300, 700, "player.isBackReady:%d", player.isBackReady);
			Novice::ScreenPrintf(0, 720, "player.isLeftReady:%d", player.isLeftReady);
			Novice::ScreenPrintf(300, 720, "player.isRightReady:%d", player.isRightReady);

			if (!player.grabBlock)
			{
				if (player.direction == FRONT)
				{
					Novice::DrawLine(static_cast<int>(player.centerPos.x), static_cast<int>(player.centerPos.y),
						static_cast<int>(player.frontPos.x), static_cast<int>(player.frontPos.y), 0xFFF00FF);
				}
				else if (player.direction == BACK)
				{
					Novice::DrawLine(static_cast<int>(player.centerPos.x), static_cast<int>(player.centerPos.y),
						static_cast<int>(player.backPos.x), static_cast<int>(player.backPos.y), 0xFFF00FF);
				}
				else if (player.direction == LEFT)
				{
					Novice::DrawLine(static_cast<int>(player.centerPos.x), static_cast<int>(player.centerPos.y),
						static_cast<int>(player.leftPos.x), static_cast<int>(player.leftPos.y), 0xFFF00FF);
				}
				else if (player.direction == RIGHT)
				{
					Novice::DrawLine(static_cast<int>(player.centerPos.x), static_cast<int>(player.centerPos.y),
						static_cast<int>(player.rightPos.x), static_cast<int>(player.rightPos.y), 0xFFF00FF);
				}
			}

			///
			/// ↑描画処理ここまで
			///

			break;
#pragma endregion
		case IN_STAGE2:
#pragma region IN_STAGE2

			break;
#pragma endregion
		case GAME_OVER:
#pragma region GAME_OVER
			///
			///↓更新処理ここから
			///

			///
			/// ↓描画処理ここから
			///

			Novice::ScreenPrintf(350, 360, "GAMEOVER");

			break;
#pragma endregion
		case GAME_CLEAR:
#pragma region GAME_CLEAR
			///
			///↓更新処理ここから
			///


			///
			/// ↓描画処理ここから
			///

			Novice::ScreenPrintf(350, 360, "GAMECLEAR");
			Novice::ScreenPrintf(350, 560, "stars:%d", stars);

			break;
#pragma endregion
		}


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