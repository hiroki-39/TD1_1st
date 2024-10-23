#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//========================
//列挙型
//========================
enum Direction
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
};

enum BlockType
{
	STAGE_WALL,
	WALL,
	FLOOR,
	BOX,
	GOOL,
	DASH_UP,
	DASH_DOWN,
	DASH_LEFT,
	DASH_RIGHT,
	ITEM,
	AIR,
};

enum SceneType
{
	TITLE,
	STAGE_SELECT,
	GAME,
	GAME_OVER,
	CLEAR,
};

//========================
//構造体
//========================
typedef struct Vector2
{
	float x;
	float y;
}Vector2;

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

//========================
//グローバル変
//========================
// ウィンドウ
const float kWindowWidth = 896.0f;
const float kWindowHeight = 640.0f;
//640

// ステージ数
const int kMaxStages = 11;

const char kWindowTitle[] = "LC1B_カトウ_ダイドウ_モリ_はたらくハコビヤ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//========================
	//変数の宣言と初期化
	//========================
	/*---マップチップ---*/
	const int kMapSizeX = 12;
	const int kMapSizeY = 10;
	const float kBlockSize = 64.0f;

	/*---シーン---*/
	int nextStage = 0;

	int scene = TITLE;

	/* 0 = ステージの壁(STAGE_WALL)
	   1 = マップの壁 (WALL)
	   2 = マップの床 (FLOOR)
	   3 = 動かせるブロック (BOX)
	   4 = ゴール地点(GOOL)
	   5 = ダッシュパネル上 (DASH_UP)
	   6 = ダッシュパネル下 (DASH_DOWN)
	   7 = ダッシュパネル左 (DASH_LEFT)
	   8 = ダッシュパネル右 (DASH_RIGHT)
	   9 = アイテム (ITEM)
	   10 = 空白 (AIR)*/

	   //ステージ0(チュートリアル)
	const int stage0[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
		{ 10,10,10, 0, 1, 1, 1, 2, 4, 1, 0,10, },
		{ 10,10,10, 0, 1, 1, 1, 2, 1, 1, 0,10, },
		{ 10,10,10, 0, 2, 2, 3, 2, 2, 2, 0,10, },
		{ 10,10,10, 0, 2, 2, 2, 2, 2, 9, 0,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ1
	const int stage1[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },// r:x 256 y128
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },// p:x 256 y192
		{ 10,10,10, 0, 1, 1, 9, 1, 1, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 3, 2, 6, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 2, 2, 4, 0,10,10, },
		{ 10,10,10, 0, 2, 3, 2, 2, 2, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 3, 1, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ2
	const int stage2[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10, 0, 1, 9, 3, 1, 1, 0,10,10, },
		{ 10,10,10, 0, 2, 3, 2, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 2, 2, 4, 0,10,10, },
		{ 10,10,10, 0, 3, 2, 2, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 9, 3, 3, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ3
	const int stage3[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },//p:x 384 y 256
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },//r:x 512 y 192
		{ 10,10,10, 0, 1, 2, 1, 9, 1, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 3, 1, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 3, 1, 0,10,10, },
		{ 10,10,10, 0, 6, 2, 3, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 4, 7, 9, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ4
	const int stage4[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10, 0, 4, 7, 1, 2, 9, 0,10,10, },
		{ 10,10,10, 0, 5, 2, 2, 3, 2, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 2, 2, 3, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 3, 2, 2, 0,10,10, },
		{ 10,10,10, 0, 1, 1, 9, 3, 1, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	/* 0 = ステージの壁(STAGE_WALL)
	   1 = マップの壁 (WALL)
	   2 = マップの床 (FLOOR)
	   3 = 動かせるブロック (BOX)
	   4 = ゴール地点(GOOL)
	   5 = ダッシュパネル上 (DASH_UP)
	   6 = ダッシュパネル下 (DASH_DOWN)
	   7 = ダッシュパネル左 (DASH_LEFT)
	   8 = ダッシュパネル右 (DASH_RIGHT)
	   9 = アイテム (ITEM)
	   10 = 空白 (AIR)*/

	   //ステージ5
	const int stage5[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },//p:x 448 y 192
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },//r:x 512 y 256
		{ 10,10,10, 0, 9, 3, 3, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 6, 2, 2, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 4, 7, 2, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 5, 2, 2, 3, 3, 0,10,10, },
		{ 10,10,10, 0, 1, 1, 2, 3, 9, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ6
	const int stage6[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10, 0, 1, 9, 8, 4, 7, 0,10,10, },
		{ 10,10,10, 0, 1, 3, 2, 3, 1, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 2, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 2, 3, 2, 3, 9, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 1, 2, 2, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ7
	const int stage7[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 1, 3, 9, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 2, 2, 1, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 2, 4, 7, 0,10,10, },
		{ 10,10,10, 0, 3, 2, 3, 3, 2, 0,10,10, },
		{ 10,10,10, 0, 9, 1, 2, 1, 1, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ8
	const int stage8[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10, 0, 1, 1, 1, 1, 1, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 2, 6, 0,10,10, },
		{ 10,10,10, 0, 3, 3, 3, 8, 4, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 3, 2, 2, 0,10,10, },
		{ 10,10,10, 0, 2, 2, 3, 9, 1, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ9
	const int stage9[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 3, 9, 0,10,10, },
		{ 10,10,10, 0, 2, 3, 2, 2, 2, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 2, 2, 0,10,10, },
		{ 10,10,10, 0, 2, 3, 3, 4, 7, 0,10,10, },
		{ 10,10,10, 0, 1, 2, 2, 5, 1, 0,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	/* 0 = ステージの壁(STAGE_WALL)
	   1 = マップの壁 (WALL)
	   2 = マップの床 (FLOOR)
	   3 = 動かせるブロック (BOX)
	   4 = ゴール地点(GOOL)
	   5 = ダッシュパネル上 (DASH_UP)
	   6 = ダッシュパネル下 (DASH_DOWN)
	   7 = ダッシュパネル左 (DASH_LEFT)
	   8 = ダッシュパネル右 (DASH_RIGHT)
	   9 = アイテム (ITEM)
	  10 = 空白 (AIR)*/

	  //ステージ10
	const int stage10[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
		{ 10,10,10, 0, 1, 1, 1, 1, 1, 1, 0,10, },
		{ 10,10,10, 0, 1, 3, 2, 2, 9, 1, 0,10, },
		{ 10,10,10, 0, 1, 2, 1, 3, 3, 1, 0,10, },
		{ 10,10,10, 0, 2, 2, 2, 2, 2, 1, 0,10, },
		{ 10,10,10, 0, 2, 3, 2, 2, 2, 1, 0,10, },
		{ 10,10,10, 0, 1, 1, 8, 4, 7, 1, 0,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	//ステージ11
	const int stage11[kMapSizeY][kMapSizeX] =
	{
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
		{ 10,10,10, 0, 1, 9, 1, 1, 1, 1, 0,10, },
		{ 10,10,10, 0, 1, 3, 3, 2, 2, 1, 0,10, },
		{ 10,10,10, 0, 1, 2, 2, 2, 2, 1, 0,10, },
		{ 10,10,10, 0, 1, 2, 3, 3, 2, 1, 0,10, },
		{ 10,10,10, 0, 6, 2, 3, 9, 3, 1, 0,10, },
		{ 10,10,10, 0, 4, 7, 2, 1, 2, 1, 0,10, },
		{ 10,10,10, 0, 0, 0, 0, 0, 0, 0, 0,10, },
		{ 10,10,10,10,10,10,10,10,10,10,10,10, },
	};

	int currentStage[kMapSizeY][kMapSizeX];

	/*---プレイヤー---*/
	Player player;
	player.pos.x = 0.0f;
	player.pos.y = 0.0f;
	player.width = kBlockSize;
	player.height = kBlockSize;
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
	player.canMove = true;
	player.canMoveBlock = false;
	player.direction = FRONT;

	//アニメーション
	player.animationCount = 0;
	player.frameCount = 0;

	/*---ロボット---*/
	Robot robot;
	robot.pos.x = 0.0f;
	robot.pos.y = 0.0f;
	robot.width = kBlockSize;
	robot.height = kBlockSize;
	robot.centerPos.x = robot.pos.x + robot.width / 2.0f;
	robot.centerPos.y = robot.pos.y + robot.height / 2.0f;
	robot.speed = 64.0f;

	robot.canMove = true;

	/*---制限時間---*/

	int secondFrame = 0;
	//制限時間
	int limitTimer = 180;


	int timer = limitTimer;

	int isTimerStart = false;

	//秒
	int numberArrayTimerSeconds[3] = {};
	//百の位
	numberArrayTimerSeconds[0] = timer / 10;
	timer %= 10;
	//十の位
	numberArrayTimerSeconds[1] = timer / 10;
	timer %= 10;
	//一の位
	numberArrayTimerSeconds[2] = timer;

	/*---クリア報酬---*/
	int stars = 0;

	//アニメーション
	int dashAnimationCount = 0;
	int dashFrameCount = 0;

	//クリアアニメーション
	float time = 0.01f;

	float easedT = 1.0f - powf(1.0f - time, 3.0f);

	float serectBotanPosY = (1.0f - easedT) * 400.0f + easedT * 320.0f;;
	float stageClearPosY = (1.0f - easedT) * 10.0f + easedT * 120.0f;
	float stageGameOverPosY = (1.0f - easedT) * 10.0f + easedT * 120.0f;

	int frameCount = 0;

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

	//タイマー
	int numberHandle[10];
	numberHandle[0] = Novice::LoadTexture("./Resources/images/0.png");
	numberHandle[1] = Novice::LoadTexture("./Resources/images/1.png");
	numberHandle[2] = Novice::LoadTexture("./Resources/images/2.png");
	numberHandle[3] = Novice::LoadTexture("./Resources/images/3.png");
	numberHandle[4] = Novice::LoadTexture("./Resources/images/4.png");
	numberHandle[5] = Novice::LoadTexture("./Resources/images/5.png");
	numberHandle[6] = Novice::LoadTexture("./Resources/images/6.png");
	numberHandle[7] = Novice::LoadTexture("./Resources/images/7.png");
	numberHandle[8] = Novice::LoadTexture("./Resources/images/8.png");
	numberHandle[9] = Novice::LoadTexture("./Resources/images/9.png");

	int secondsHandle = Novice::LoadTexture("./Resources/images/seconds.png");
	int rimitHandle = Novice::LoadTexture("./Resources/images/rimit.png");
	int timerBackGraundHandle = Novice::LoadTexture("./Resources/images/timerBackGraund.png");

	//マップチップ
	int wallBlockHandle = Novice::LoadTexture("./Resources/images/blockWall.png");
	int floorBlockHandle = Novice::LoadTexture("./Resources/images/floor.png");

	int DashPanelUpHandle[4];
	DashPanelUpHandle[0] = Novice::LoadTexture("./Resources/images/dashUp0.png");
	DashPanelUpHandle[1] = Novice::LoadTexture("./Resources/images/dashUp1.png");
	DashPanelUpHandle[2] = Novice::LoadTexture("./Resources/images/dashUp2.png");
	DashPanelUpHandle[3] = Novice::LoadTexture("./Resources/images/dashUp3.png");

	int DashPanelDownHandle[4];
	DashPanelDownHandle[0] = Novice::LoadTexture("./Resources/images/dashDown0.png");
	DashPanelDownHandle[1] = Novice::LoadTexture("./Resources/images/dashDown1.png");
	DashPanelDownHandle[2] = Novice::LoadTexture("./Resources/images/dashDown2.png");
	DashPanelDownHandle[3] = Novice::LoadTexture("./Resources/images/dashDown3.png");

	int DashPanelLeftHandle[4];
	DashPanelLeftHandle[0] = Novice::LoadTexture("./Resources/images/dashLeft0.png");
	DashPanelLeftHandle[1] = Novice::LoadTexture("./Resources/images/dashLeft1.png");
	DashPanelLeftHandle[2] = Novice::LoadTexture("./Resources/images/dashLeft2.png");
	DashPanelLeftHandle[3] = Novice::LoadTexture("./Resources/images/dashLeft3.png");

	int DashPanelRightHandle[4];
	DashPanelRightHandle[0] = Novice::LoadTexture("./Resources/images/dashRight0.png");
	DashPanelRightHandle[1] = Novice::LoadTexture("./Resources/images/dashRight1.png");
	DashPanelRightHandle[2] = Novice::LoadTexture("./Resources/images/dashRight2.png");
	DashPanelRightHandle[3] = Novice::LoadTexture("./Resources/images/dashRight3.png");

	int goalHandle = Novice::LoadTexture("./Resources/images/goal.png");
	int stageWallHandle = Novice::LoadTexture("./Resources/images/stageWall.png");
	int blockBoxHandle = Novice::LoadTexture("./Resources/images/box.png");
	int ItemHandle = Novice::LoadTexture("./Resources/images/item.png");

	//背景
	int gameBackgroundHandle = Novice::LoadTexture("./Resources/images/gameBackground.png");
	int clearBackgroundHandle = Novice::LoadTexture("./Resources/images/background.png");
	int selectCursor[3];
	selectCursor[0] = Novice::LoadTexture("./Resources/images/stageSelectCursor_1.png");
	selectCursor[1] = Novice::LoadTexture("./Resources/images/stageSelectCursor_2.png");
	selectCursor[2] = Novice::LoadTexture("./Resources/images/stageSelectCursor_3.png");
	
	//タイトル
	int titleHandle = Novice::LoadTexture("./Resources/images/Title.png");
	//ステージ選択
	int stageSelectHandle = Novice::LoadTexture("./Resources/images/stageSelectBackGraund.png");
	int selectMojiHandle = Novice::LoadTexture("./Resources/images/stageSelect_char.png");
	int selectHandle[12];
	selectHandle[0] = Novice::LoadTexture("./Resources/images/selectStage0.png");
	selectHandle[1] = Novice::LoadTexture("./Resources/images/selectStage1.png");
	selectHandle[2] = Novice::LoadTexture("./Resources/images/selectStage2.png");
	selectHandle[3] = Novice::LoadTexture("./Resources/images/selectStage3.png");
	selectHandle[4] = Novice::LoadTexture("./Resources/images/selectStage4.png");
	selectHandle[5] = Novice::LoadTexture("./Resources/images/selectStage5.png");
	selectHandle[6] = Novice::LoadTexture("./Resources/images/selectStage6.png");
	selectHandle[7] = Novice::LoadTexture("./Resources/images/selectStage7.png");
	selectHandle[8] = Novice::LoadTexture("./Resources/images/selectStage8.png");
	selectHandle[9] = Novice::LoadTexture("./Resources/images/selectStage9.png");
	selectHandle[10] = Novice::LoadTexture("./Resources/images/selectStage10.png");
	selectHandle[11] = Novice::LoadTexture("./Resources/images/selectStage11.png");

	//ゲームクリア
	int stageClearHandle = Novice::LoadTexture("./Resources/images/stageClear.png");
	//ゲームオーバー
	int gameOverHandle = Novice::LoadTexture("./Resources/images/gameOver.png");
	int gameOverBackgroundHandle = Novice::LoadTexture("./Resources/images/gameOverBGt.png");
	//ボタン
	int botanHandle = Novice::LoadTexture("./Resources/images/botan.png");
	int serectBotanHandle = Novice::LoadTexture("./Resources/images/pressSpaceSerect.png");

	//説明
	int explanationBackGraundHandle = Novice::Novice::LoadTexture("./Resources/images/explanationBackGraund.png");
	int explanationHandle[5];
	explanationHandle[0] = Novice::LoadTexture("./Resources/images/control_wasd.png");
	explanationHandle[1] = Novice::LoadTexture("./Resources/images/control_space.png");
	explanationHandle[2] = Novice::LoadTexture("./Resources/images/tutorial_txt.png");
	explanationHandle[3] = Novice::LoadTexture("./Resources/images/tab.png");
	explanationHandle[4] = Novice::LoadTexture("./Resources/images/reset.png");

	int itemHandle[2];
	itemHandle[0] = Novice::LoadTexture("./Resources/images/pickItem.png");
	itemHandle[1] = Novice::LoadTexture("./Resources/images/pick_Item.png");
	/*---BGM,SE---*/
	//BGM
	int playHandle[5] = { -1 };

	int bgmHandle[5];
	bgmHandle[0] = Novice::LoadAudio("./Resources/Sounds/title.mp3");//title
	bgmHandle[1] = Novice::LoadAudio("./Resources/Sounds/stageSelect.mp3");//stageselect
	bgmHandle[2] = Novice::LoadAudio("./Resources/Sounds/gamePlay.mp3");//gamePlay
	bgmHandle[3] = Novice::LoadAudio("./Resources/Sounds/clear.mp3");//clear
	bgmHandle[4] = Novice::LoadAudio("./Resources/Sounds/sousou4.mp3");//gameover
	//SE
	int soundHandle[8];
	soundHandle[0] = Novice::LoadAudio("./Resources/Sounds/select.mp3");//決定
	soundHandle[1] = Novice::LoadAudio("./Resources/Sounds/cursor.mp3");//選択カーソルの移動時
	soundHandle[2] = Novice::LoadAudio("./Resources/Sounds/player.mp3");//プレイヤー足音
	soundHandle[3] = Novice::LoadAudio("./Resources/Sounds/robot.mp3");//ロボット足音
	soundHandle[4] = Novice::LoadAudio("./Resources/Sounds/grabBlock.mp3");//掴む
	soundHandle[5] = Novice::LoadAudio("./Resources/Sounds/itemget_se.mp3");//アイテム
	soundHandle[6] = Novice::LoadAudio("./Resources/Sounds/clear_se.mp3");//クリア
	//
	int useSoundHandle[2];
	useSoundHandle[0] = Novice::LoadTexture("./Resources/images/useSound_char.png");
	useSoundHandle[1] = Novice::LoadTexture("./Resources/images/useSound_char2.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		switch (scene)
		{
		case TITLE:
#pragma region TITLE
			///
			/// ↓更新処理ここから
			///

			//========================
			//選択の更新処理
			//========================
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				scene = STAGE_SELECT;
				//SE
				Novice::PlayAudio(soundHandle[0], false, 0.2f);
				Novice::StopAudio(playHandle[0]);
				break;
			}

			/*---BGN再生---*/
			if (!Novice::IsPlayingAudio(playHandle[0]) || playHandle[0] == -1)
			{
				playHandle[0] = Novice::PlayAudio(bgmHandle[0], false, 0.1f);
			}

			///
			/// ↑更新処理ここまで
			///


			///
			/// ↓描画処理ここから
			///

			//========================
			//文字の描画処理
			//========================
			Novice::DrawSprite(0, 0, titleHandle, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(260, 400, botanHandle, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(530, 600, useSoundHandle[0], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(750, 600, useSoundHandle[1], 1.0f, 1.0f, 0.0f, WHITE);
			///
			/// ↑描画処理ここまで
			///

#pragma endregion
			break;
		case STAGE_SELECT:
#pragma region STAGE_SELECT
			///
			/// ↓更新処理ここから
			///

			//========================
			//選択の更新処理
			//========================
			if (keys[DIK_A] && !preKeys[DIK_A])
			{
				if (nextStage > 0)
				{
					nextStage--;
				}
				//SE
				Novice::PlayAudio(soundHandle[1], false, 0.2f);
			}
			else if (keys[DIK_D] && !preKeys[DIK_D])
			{
				if (nextStage < kMaxStages)
				{
					nextStage++;
				}

				//SE
				Novice::PlayAudio(soundHandle[1], false, 0.2f);
			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				for (int i = 0; i < kMapSizeY; i++)
				{
					for (int j = 0; j < kMapSizeX; j++)
					{
						if (nextStage == 0)
						{
							currentStage[i][j] = stage0[i][j];
							player.pos.x = 320.0f;
							player.pos.y = 320.0f;
							robot.pos.x = 320.0f;
							robot.pos.y = 192.0f;
						}
						else if (nextStage == 1)
						{
							currentStage[i][j] = stage1[i][j];
							player.pos.x = 256.0f;
							player.pos.y = 192.0f;
							robot.pos.x = 256.0f;
							robot.pos.y = 128.0f;
						}
						else if (nextStage == 2)
						{
							currentStage[i][j] = stage2[i][j];
							player.pos.x = 384.0f;
							player.pos.y = 256.0f;
							robot.pos.x = 448.0f;
							robot.pos.y = 128.0f;

						}
						else if (nextStage == 3)
						{
							currentStage[i][j] = stage3[i][j];
							player.pos.x = 384.0f;
							player.pos.y = 256.0f;
							robot.pos.x = 512.0f;
							robot.pos.y = 192.0f;
						}
						else if (nextStage == 4)
						{
							currentStage[i][j] = stage4[i][j];
							player.pos.x = 448.0f;
							player.pos.y = 256.0f;
							robot.pos.x = 320.0f;
							robot.pos.y = 384.0f;
						}
						else if (nextStage == 5)
						{
							currentStage[i][j] = stage5[i][j];
							player.pos.x = 448.0f;
							player.pos.y = 192.0f;
							robot.pos.x = 512.0f;
							robot.pos.y = 256.0f;
						}
						else if (nextStage == 6)
						{
							currentStage[i][j] = stage6[i][j];
							player.pos.x = 256.0f;
							player.pos.y = 320.0f;
							robot.pos.x = 384.0f;
							robot.pos.y = 384.0f;
						}
						else if (nextStage == 7)
						{
							currentStage[i][j] = stage7[i][j];
							player.pos.x = 256.0f;
							player.pos.y = 256.0f;
							robot.pos.x = 320.0f;
							robot.pos.y = 384.0f;
						}
						else if (nextStage == 8)
						{
							currentStage[i][j] = stage8[i][j];
							player.pos.x = 320.0f;
							player.pos.y = 384.0f;
							robot.pos.x = 256.0f;
							robot.pos.y = 192.0f;
						}
						else if (nextStage == 9)
						{
							currentStage[i][j] = stage9[i][j];
							player.pos.x = 384.0f;
							player.pos.y = 256.0f;
							robot.pos.x = 256.0f;
							robot.pos.y = 256.0f;
						}
						else if (nextStage == 10)
						{
							currentStage[i][j] = stage10[i][j];
							player.pos.x = 384.0f;
							player.pos.y = 320.0f;
							robot.pos.x = 320.0f;
							robot.pos.y = 256.0f;
						}
						else if (nextStage == 11)
						{
							currentStage[i][j] = stage11[i][j];
							player.pos.x = 448.0f;
							player.pos.y = 192.0f;
							robot.pos.x = 512.0f;
							robot.pos.y = 128.0f;
						}
					}
				}

				//タイマーの起動
				limitTimer = 180;
				isTimerStart = true;
				//各ステージの座標に更新
				player.centerPos.x = player.pos.x + player.width / 2.0f;
				player.centerPos.y = player.pos.y + player.height / 2.0f;

				player.frontPos.x = player.centerPos.x;
				player.frontPos.y = player.centerPos.y + player.height;
				player.backPos.x = player.centerPos.x;
				player.backPos.y = player.centerPos.y - player.height;
				player.rightPos.x = player.centerPos.x + player.width;
				player.rightPos.y = player.centerPos.y;
				player.leftPos.x = player.centerPos.x - player.width;
				player.leftPos.y = player.centerPos.y;

				robot.centerPos.x = robot.pos.x + robot.width / 2.0f;
				robot.centerPos.y = robot.pos.y + robot.height / 2.0f;
				//フラグの初期化
				player.canMove = true;
				player.isItemGet = false;

				scene = GAME;
				//SE
				Novice::PlayAudio(soundHandle[0], false, 0.2f);
				Novice::StopAudio(playHandle[1]);
				break;
			}


			/*---BGN再生---*/
			if (!Novice::IsPlayingAudio(playHandle[1]) || playHandle[1] == -1)
			{
				playHandle[1] = Novice::PlayAudio(bgmHandle[1], false, 0.1f);
			}

			//フレームのカウント
			frameCount++;
			if (frameCount == 60)
			{
				frameCount = 0;
			}

			///
			/// ↑更新処理ここまで
			///

			///
			/// ↓描画処理ここから
			///

			//========================
			//文字の描画処理
			//========================
			Novice::DrawSprite(0, 0, stageSelectHandle, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(280, 20, selectMojiHandle, 1.0f, 1.0f, 0.0f, WHITE);

			for (int i = 0; i <= kMaxStages; i++)
			{
				// ステージ名
				if (i <= 5)
				{
					// 印
					Novice::DrawSprite(70 + 130 * (i), 100, selectHandle[i], 0.4f, 0.4f, 0.0f, WHITE);
				}
				else if (5 <= i || i >= 10)
				{
					// 印
					Novice::DrawSprite(70 + 130 * (i - 6), 240, selectHandle[i], 0.4f, 0.4f, 0.0f, WHITE);
				}


				if (nextStage == i)
				{
					if (i <= 5)
					{
						// 印
						Novice::DrawSprite(70 + 130 * i, 100, selectCursor[frameCount / 20], 0.4f, 0.4f, 0.0f, WHITE);
					}
					else if (5 <= i || i >= 10)
					{
						// 印
						Novice::DrawSprite(70 + 130 * (i - 6), 240, selectCursor[frameCount / 20], 0.4f, 0.4f, 0.0f, WHITE);
					}
				}
			}


			///
			/// ↑描画処理ここまで
			///
#pragma endregion
			break;
		case GAME:
#pragma region GAME
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

			if (player.canMove)
			{
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
						if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == GOOL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == WALL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == STAGE_WALL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_UP ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_DOWN ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_LEFT ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_RIGHT ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == BOX)
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
						if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == FLOOR ||
							currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == STAGE_WALL ||
							currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == ITEM)
						{
							robot.pos.y += robot.speed;
							robot.centerPos.y += robot.speed;
						}
						//SE
						Novice::PlayAudio(soundHandle[2], false, 0.2f);
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
						if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == GOOL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == WALL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == STAGE_WALL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_UP ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_DOWN ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_LEFT ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_RIGHT ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == BOX)
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
						if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == FLOOR ||
							currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == STAGE_WALL ||
							currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == ITEM)
						{
							robot.pos.x += robot.speed;
							robot.centerPos.x += robot.speed;
						}
						//SE
						Novice::PlayAudio(soundHandle[2], false, 0.2f);
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
						if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == GOOL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == WALL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == STAGE_WALL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_UP ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_DOWN ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_LEFT ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_RIGHT ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == BOX)
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
						if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == FLOOR ||
							currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == STAGE_WALL ||
							currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == ITEM)
						{
							robot.pos.y -= robot.speed;
							robot.centerPos.y -= robot.speed;

						}

						//SE
						Novice::PlayAudio(soundHandle[2], false, 0.2f);
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
						if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == GOOL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == WALL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == STAGE_WALL ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_UP ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_DOWN ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_LEFT ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_RIGHT ||
							currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == BOX)
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
						if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == FLOOR ||
							currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == STAGE_WALL ||
							currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == ITEM)
						{
							robot.pos.x -= robot.speed;
							robot.centerPos.x -= robot.speed;
						}
						//SE
						Novice::PlayAudio(soundHandle[2], false, 0.2f);
					}
					//ロボットが動くこと出来るかの判定
					robot.canMove = true;
				}

				//足場ブロックがプレイヤーが向いてる方向にあるからどうかの判定とロボットが足場ブロックの上にいないかの判定
				if (player.direction == FRONT)
				{
					if (currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == BOX)
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
					if (currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == BOX)
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
					if (currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == BOX)
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
					if (currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == BOX)
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
								if (currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == GOOL ||
									currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == WALL ||
									currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == STAGE_WALL ||
									currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == DASH_UP ||
									currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == DASH_DOWN ||
									currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == DASH_LEFT ||
									currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == DASH_RIGHT ||
									currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == BOX ||
									currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] == ITEM)
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
								if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == WALL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == GOOL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == STAGE_WALL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_UP ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_DOWN ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_LEFT ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_RIGHT ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == BOX ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == ITEM)
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
							if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == FLOOR ||
								currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == STAGE_WALL ||
								currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == ITEM)
							{
								robot.pos.y += robot.speed;
								robot.centerPos.y += robot.speed;
							}
							//SE
							Novice::PlayAudio(soundHandle[2], false, 0.2f);
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
								if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == WALL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == GOOL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == STAGE_WALL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_UP ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_DOWN ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_LEFT ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_RIGHT ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == BOX ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == ITEM)
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
								if (currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == GOOL ||
									currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == WALL ||
									currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == STAGE_WALL ||
									currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == DASH_UP ||
									currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == DASH_DOWN ||
									currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == DASH_LEFT ||
									currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == DASH_RIGHT ||
									currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == BOX ||
									currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] == ITEM)
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
							if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == FLOOR ||
								currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == STAGE_WALL ||
								currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == ITEM)
							{
								robot.pos.y -= robot.speed;
								robot.centerPos.y -= robot.speed;
							}
							//SE
							Novice::PlayAudio(soundHandle[2], false, 0.2f);
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
								if (currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == GOOL ||
									currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == WALL ||
									currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == STAGE_WALL ||
									currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == DASH_UP ||
									currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == DASH_DOWN ||
									currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == DASH_LEFT ||
									currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == DASH_RIGHT ||
									currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == BOX ||
									currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] == ITEM)
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
								if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == WALL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == GOOL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == STAGE_WALL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_UP ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_DOWN ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_LEFT ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_RIGHT ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == BOX ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == ITEM)
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
							if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == FLOOR ||
								currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == STAGE_WALL ||
								currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == ITEM)
							{
								robot.pos.x += robot.speed;
								robot.centerPos.x += robot.speed;
							}
							//SE
							Novice::PlayAudio(soundHandle[2], false, 0.2f);
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
								if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == WALL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == GOOL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == STAGE_WALL ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_UP ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_DOWN ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_LEFT ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == DASH_RIGHT ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == BOX ||
									currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == ITEM)
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
								if (currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == GOOL ||
									currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == WALL ||
									currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == STAGE_WALL ||
									currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == DASH_UP ||
									currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == DASH_DOWN ||
									currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == DASH_LEFT ||
									currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == DASH_RIGHT ||
									currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == BOX ||
									currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] == ITEM)
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
							if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == FLOOR ||
								currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == STAGE_WALL ||
								currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == ITEM)
							{
								robot.pos.x -= robot.speed;
								robot.centerPos.x -= robot.speed;

							}
							//SE
							Novice::PlayAudio(soundHandle[2], false, 0.2f);
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
							currentStage[static_cast<int>((player.backPos.y + player.height) / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] = FLOOR;
							currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] = BOX;


							//ブロック前に動かす
							if (keys[DIK_S] && !preKeys[DIK_S])
							{
								currentStage[static_cast<int>((player.backPos.y - player.height) / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] = FLOOR;
							}

							currentStage[static_cast<int>(player.backPos.y / kBlockSize)][static_cast<int>(player.backPos.x / kBlockSize)] = BOX;
						}


					}
					else if (player.isFrontReady)
					{
						if (player.canMoveBlock)
						{
							if (keys[DIK_W] && !preKeys[DIK_W])
							{
								//ブロック後ろに動かす
								currentStage[static_cast<int>((player.frontPos.y + player.height) / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] = FLOOR;
							}

							currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] = { BOX };

							//ブロック前に動かす
							currentStage[static_cast<int>((player.frontPos.y - player.height) / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] = FLOOR;
							currentStage[static_cast<int>(player.frontPos.y / kBlockSize)][static_cast<int>(player.frontPos.x / kBlockSize)] = BOX;
						}

					}
					else if (player.isLeftReady)
					{
						if (player.canMoveBlock)
						{
							//ブロックを動かす
							currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>((player.leftPos.x + player.width) / kBlockSize)] = FLOOR;

							currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] = BOX;

							if (keys[DIK_D] && !preKeys[DIK_D])
							{
								//ブロックを動かす
								currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>((player.leftPos.x - player.width) / kBlockSize)] = FLOOR;
							}

							currentStage[static_cast<int>(player.leftPos.y / kBlockSize)][static_cast<int>(player.leftPos.x / kBlockSize)] = BOX;
						}

					}
					else if (player.isRightReady)
					{
						if (player.canMoveBlock)
						{
							if (keys[DIK_A] && !preKeys[DIK_A])
							{
								//ブロックを動かす
								currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>((player.rightPos.x + player.width) / kBlockSize)] = FLOOR;
							}

							currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] = BOX;

							//ブロックを動かす
							currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>((player.rightPos.x - player.width) / kBlockSize)] = FLOOR;
							currentStage[static_cast<int>(player.rightPos.y / kBlockSize)][static_cast<int>(player.rightPos.x / kBlockSize)] = BOX;
						}
					}
				}
			}

			/*---リスタート---*/
			if (keys[DIK_R] && !preKeys[DIK_R])
			{
				if (stage1)
				{
					player.pos.x = 256.0f;
					player.pos.y = 192.0f;
					robot.pos.x = 256.0f;
					robot.pos.y = 128.0f;
				}
				else if (stage2)
				{
					player.pos.x = 384.0f;
					player.pos.y = 256.0f;
					robot.pos.x = 448.0f;
					robot.pos.y = 128.0f;
				}
				else if (stage3)
				{
					player.pos.x = 384.0f;
					player.pos.y = 256.0f;
					robot.pos.x = 512.0f;
					robot.pos.y = 192.0f;
				}
				else if (stage4)
				{
					player.pos.x = 448.0f;
					player.pos.y = 256.0f;
					robot.pos.x = 320.0f;
					robot.pos.y = 384.0f;
				}
				else if (stage5)
				{
					player.pos.x = 448.0f;
					player.pos.y = 192.0f;
					robot.pos.x = 512.0f;
					robot.pos.y = 256.0f;
				}

				//タイマーの起動
				isTimerStart = true;
				//各ステージの座標に更新
				player.centerPos.x = player.pos.x + player.width / 2.0f;
				player.centerPos.y = player.pos.y + player.height / 2.0f;

				player.frontPos.x = player.centerPos.x;
				player.frontPos.y = player.centerPos.y + player.height;
				player.backPos.x = player.centerPos.x;
				player.backPos.y = player.centerPos.y - player.height;
				player.rightPos.x = player.centerPos.x + player.width;
				player.rightPos.y = player.centerPos.y;
				player.leftPos.x = player.centerPos.x - player.width;
				player.leftPos.y = player.centerPos.y;

				robot.centerPos.x = robot.pos.x + robot.width / 2.0f;
				robot.centerPos.y = robot.pos.y + robot.height / 2.0f;
			}

			/*---ステージセレクトに戻る---*/
			if (keys[DIK_TAB] && !preKeys[DIK_TAB])
			{
				scene = STAGE_SELECT;
				//SE
				Novice::PlayAudio(soundHandle[0], false, 0.2f);
				Novice::StopAudio(playHandle[2]);
				break;
			}

			//==============================
			//フレームタイマーのカウント
			//==============================
			//プレイヤー
			player.frameCount++;

			if (player.frameCount > 59)
			{
				player.frameCount = 0;
			}

			player.animationCount = player.frameCount / 15;

			//その他

			dashFrameCount++;

			if (dashFrameCount > 59)
			{
				dashFrameCount = 0;
			}

			dashAnimationCount = dashFrameCount / 15;

			//==============================
			//ステージギミックの更新処理
			//==============================

			/*---アイテムの更新処理---*/
			if (currentStage[static_cast<int>(player.pos.y / kBlockSize)][static_cast<int>(player.pos.x / kBlockSize)] == ITEM)
			{
				//アイテムが置いてあった場所を床に変える
				for (int y = 0; y < kMapSizeY; y++)
				{
					for (int x = 0; x < kMapSizeX; x++)
					{
						if (currentStage[y][x] == ITEM)
						{
							currentStage[y][x] = FLOOR;
						}
					}
				}

				player.isItemGet = true;

				//SE
				Novice::PlayAudio(soundHandle[5], false, 0.2f);
			}

			/*---ゴールの更新処理---*/
			if (player.isItemGet)
			{
				if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == GOOL)
				{
					isTimerStart = false;
					scene = CLEAR;
					Novice::StopAudio(playHandle[2]);
					Novice::PlayAudio(soundHandle[6], false, 0.4f);
					break;
				}
			}

			/*---ダッシュパネルの更新処理---*/
			if (player.isItemGet)
			{
				if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == DASH_UP)
				{
					player.canMove = false;
					robot.pos.y -= 0.5f;
				}
				else if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == DASH_DOWN)
				{
					player.canMove = false;
					robot.pos.y += 0.5f;
				}
				else if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == DASH_LEFT)
				{
					player.canMove = false;
					robot.pos.x -= 0.5f;
				}
				else if (currentStage[static_cast<int>(robot.pos.y / kBlockSize)][static_cast<int>(robot.pos.x / kBlockSize)] == DASH_RIGHT)
				{
					player.canMove = false;
					robot.pos.x += 0.5f;
				}
			}

			//===========================
			// 制限時間タイマー
			//===========================

			if (isTimerStart)
			{
				if (limitTimer > 0)
				{
					secondFrame++;

					if (secondFrame >= 60)
					{
						limitTimer--;

						secondFrame = 0;
					}
				}
			}

			timer = limitTimer;

			//百の位
			numberArrayTimerSeconds[0] = timer / 100;
			timer %= 100;

			//十の位
			numberArrayTimerSeconds[1] = timer / 10;
			timer %= 10;

			//一の位
			numberArrayTimerSeconds[2] = timer;

			//制限時間が0秒になったらシーンをゲームオーバーに切り替え
			if (limitTimer <= 0)
			{
				scene = GAME_OVER;
				Novice::StopAudio(playHandle[2]);
				break;
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

			//===========================
			// BGM
			//===========================
			/*---BGN再生---*/
			if (!Novice::IsPlayingAudio(playHandle[2]) || playHandle[2] == -1)
			{
				playHandle[2] = Novice::PlayAudio(bgmHandle[2], false, 0.1f);
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

			Novice::DrawSprite(0, 0, gameBackgroundHandle, 1, 1, 0.0f, WHITE);

			//=====================
			//マップチップの描画処理
			//=====================

			for (int y = 0; y < kMapSizeY; y++)
			{
				for (int x = 0; x < kMapSizeX; x++)
				{
					//床
					if (currentStage[y][x] == FLOOR)
					{

						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							floorBlockHandle, 1.0f, 1.0f, 0.0f, WHITE);
					}

					//壁
					if (currentStage[y][x] == WALL)
					{

						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							wallBlockHandle, 1.0f, 1.0f, 0.0f, WHITE);
					}

					//外枠
					if (currentStage[y][x] == STAGE_WALL)
					{
						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							stageWallHandle, 1.0f, 1.0f, 0.0f, WHITE);
					}

					//足場ブロック
					if (currentStage[y][x] == BOX)
					{
						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							blockBoxHandle, 1.0f, 1.0f, 0.0f, WHITE);
					}

					//アイテム
					if (currentStage[y][x] == ITEM)
					{
						if (!player.isItemGet)
						{
							Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
								ItemHandle, 1.0f, 1.0f, 0.0f, WHITE);
						}
						else
						{
							Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
								floorBlockHandle, 1.0f, 1.0f, 0.0f, WHITE);
						}
					}

					//ゴール
					if (currentStage[y][x] == GOOL)
					{
						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							goalHandle, 1.0f, 1.0f, 0.0f, WHITE);
					}

					//ダッシュブロック
					if (currentStage[y][x] == DASH_UP)
					{
						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							DashPanelUpHandle[dashAnimationCount], 1.0f, 1.0f, 0.0f, WHITE);
					}

					if (currentStage[y][x] == DASH_DOWN)
					{
						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							DashPanelDownHandle[dashAnimationCount], 1.0f, 1.0f, 0.0f, WHITE);
					}

					if (currentStage[y][x] == DASH_LEFT)
					{
						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							DashPanelLeftHandle[dashAnimationCount], 1.0f, 1.0f, 0.0f, WHITE);
					}

					if (currentStage[y][x] == DASH_RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(kBlockSize) * x, static_cast<int>(kBlockSize) * y,
							DashPanelRightHandle[dashAnimationCount], 1.0f, 1.0f, 0.0f, WHITE);
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
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[2], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == BACK)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[0], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == LEFT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[1], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[3], 1.0f, 1.0f, 0.0f, WHITE);
					}
				}
				else
				{
					if (player.direction == FRONT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[1], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == BACK)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[0], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == LEFT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[2], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[3], 1.0f, 1.0f, 0.0f, WHITE);
					}
				}
			}
			else
			{
				if (!player.grabBlock)
				{
					if (player.direction == FRONT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[2], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == BACK)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[0], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == LEFT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[1], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerHandle[3], 1.0f, 1.0f, 0.0f, WHITE);
					}
				}
				else
				{
					if (player.direction == FRONT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[1], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == BACK)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[0], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == LEFT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[2], 1.0f, 1.0f, 0.0f, WHITE);
					}
					else if (player.direction == RIGHT)
					{
						Novice::DrawSprite(static_cast<int>(player.pos.x), static_cast<int>(player.pos.y), playerGradHandle[3], 1.0f, 1.0f, 0.0f, WHITE);
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
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHandle[2], 1.0f, 1.0f, 0.0f, WHITE);
				}
				else if (player.direction == BACK)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHandle[0], 1.0f, 1.0f, 0.0f, WHITE);
				}
				else if (player.direction == LEFT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHandle[1], 1.0f, 1.0f, 0.0f, WHITE);
				}
				else if (player.direction == RIGHT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHandle[3], 1.0f, 1.0f, 0.0f, WHITE);
				}
			}
			else
			{
				if (player.direction == FRONT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHoldItemHandle[2], 1.0f, 1.0f, 0.0f, WHITE);
				}
				else if (player.direction == BACK)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHoldItemHandle[0], 1.0f, 1.0f, 0.0f, WHITE);
				}
				else if (player.direction == LEFT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHoldItemHandle[1], 1.0f, 1.0f, 0.0f, WHITE);
				}
				else if (player.direction == RIGHT)
				{
					Novice::DrawSprite(static_cast<int>(robot.pos.x), static_cast<int>(robot.pos.y), robotHoldItemHandle[3], 1.0f, 1.0f, 0.0f, WHITE);
				}
			}

			//=======================
			//制限時間の描画処理
			//=======================
			//背景
			Novice::DrawSprite(10, 60, timerBackGraundHandle, 0.7f, 0.6f, 0.0f, WHITE);

			//時間
			for (int i = 0; i < 3; i++)
			{
				Novice::DrawSprite(10 + 40 * i, 100, numberHandle[numberArrayTimerSeconds[i]], 0.4f, 0.4f, 0.0f, WHITE);
			}

			//残り
			Novice::DrawSprite(20, 65, rimitHandle, 0.5f, 0.5f, 0.0f, WHITE);
			//秒
			Novice::DrawSprite(140, 110, secondsHandle, 0.5f, 0.5f, 0.0f, WHITE);

			//=======================
			//操作説明の描画処理
			//=======================

			Novice::DrawSprite(50, 300, itemHandle[0], 1.3f, 1.3f, 0.0f, WHITE);

			if (player.isItemGet)
			{
				Novice::DrawSprite(50, 300, itemHandle[1], 1.3f, 1.3f, 0.0f, WHITE);
			}

			Novice::DrawSprite(660, 90, explanationBackGraundHandle, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(100, 510, explanationHandle[0], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(250, 510, explanationHandle[1], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(530, 540, explanationHandle[2], 1.3f, 1.3f, 0.0f, WHITE);
			Novice::DrawSprite(670, 100, explanationHandle[3], 1.3f, 1.3f, 0.0f, WHITE);
			Novice::DrawSprite(670, 160, explanationHandle[4], 1.3f, 1.3f, 0.0f, WHITE);

			///
			/// ↑描画処理ここまで
			///

			/*Novice::ScreenPrintf(20, 40, "%d:%02d", limitTimer / 60, limitTimer % 60);
			Novice::ScreenPrintf(20, 60, "stars:%d", stars);

			Novice::ScreenPrintf(0, 600, "front:stage1[%d][%d]",
				static_cast<int>(player.frontPos.y / kBlockSize),
				static_cast<int>(player.frontPos.x / kBlockSize));

			Novice::ScreenPrintf(170, 600, "back:stage1[%d][%d]",
				static_cast<int>(player.backPos.y / kBlockSize),
				static_cast<int>(player.backPos.x / kBlockSize));

			Novice::ScreenPrintf(0, 620, "left:stage1[%d][%d]",
				static_cast<int>(player.leftPos.y / kBlockSize),
				static_cast<int>(player.leftPos.x / kBlockSize));

			Novice::ScreenPrintf(170, 620, "right:stage1[%d][%d]",
				static_cast<int>(player.rightPos.y / kBlockSize),
				static_cast<int>(player.rightPos.x / kBlockSize));

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
			}*/

#pragma endregion
			break;
		case GAME_OVER:
#pragma region GAME_OVER

			///
			///↓更新処理ここから
			///

		    //========================
			//イージングの更新処理
			//========================

			time += 0.01f;
			if (time >= 1.0f)
			{
				time = 1.0f;
			}

			easedT = 1.0f - powf(1.0f - time, 3.0f);

			//SPACEを押す文
			serectBotanPosY = (1.0f - easedT) * 400.0f + easedT * 320.0f;

			//ステージクリアの文
			stageGameOverPosY = (1.0f - easedT) * 10.0f + easedT * 120.0f;

			//========================
			//選択の更新処理
			//========================

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				scene = STAGE_SELECT;
				Novice::StopAudio(playHandle[4]);
				break;
			}

			/*---BGN再生---*/
			if (!Novice::IsPlayingAudio(playHandle[4]) || playHandle[4] == -1)
			{
				playHandle[4] = Novice::PlayAudio(bgmHandle[4], false, 0.1f);
			}

			///
			/// ↓描画処理ここから
			///

			Novice::DrawSprite(0, 0, gameOverBackgroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(150, static_cast<int>(stageGameOverPosY), gameOverHandle, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(100, static_cast<int>(serectBotanPosY), serectBotanHandle, 1.0f, 1.0f, 0.0f, WHITE);


#pragma endregion
			break;
		case CLEAR:
#pragma region CLEAR

			///
			///↓更新処理ここから
			///
			

			//========================
			//イージングの更新処理
			//========================

			time += 0.01f;
			if (time >= 1.0f)
			{
				time = 1.0f;
			}

			easedT = 1.0f - powf(1.0f - time, 3.0f);

			//SPACEを押す文
			serectBotanPosY = (1.0f - easedT) * 400.0f + easedT * 320.0f;

			//ステージクリアの文
			stageClearPosY = (1.0f - easedT) * 10.0f + easedT * 120.0f;


			//========================
			//選択の更新処理
			//========================

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
			{
				limitTimer = 180;
				scene = STAGE_SELECT;
			}

			///
			/// ↓描画処理ここから
			///

			Novice::DrawSprite(0, 0, clearBackgroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(150, static_cast<int>(stageClearPosY), stageClearHandle, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(110, static_cast<int>(serectBotanPosY), serectBotanHandle, 1.0f, 1.0f, 0.0f, WHITE);

#pragma endregion
			break;
		}


		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
		{
			break;
		}

	}
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}