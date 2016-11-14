#include "MainScene.h"

USING_NS_CC;//namespace

//フルーツの画面上端からのマージン
const int FRUIT_TOP_MARGIN = 40;
//フルーツの出現率
const int FRUIT_SPAWN_RATE = 20;

MainScene::MainScene() 
:_player(NULL)
//,_scoreLabel(NULL)
{

}

MainScene::~MainScene()
{
	CC_SAFE_RELEASE_NULL(_player);
	
}



Scene* MainScene::createScene() 
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);

	return scene;
}

bool MainScene::init() {
	if (!Layer::init()) 
	{
		return false;
	}

	auto director = Director::getInstance();
	//画面サイズを出す
	auto size = director->getWinSize();
	//背景のスプライトを生成する
	auto background = Sprite::create("background.png");
	//表示位置
	background->setPosition(Vec2(size.width / 2.0, size.height / 2.0));
	//ノードにスプライトする
	this->addChild(background);
	
	this->setPlayer(Sprite::create("player.png"));//player画像を読み込み
	_player->setPosition(Vec2(size.width / 2.0, size.height - 445));//_playerの位置
	this->addChild(_player);//シーンに_playerを配置

	/*auto scoreLabel = Label::createWithSystemFont(StringUtils::toString(_score),"Marker Felt",16);//_scoreLabelを文字列に
	scoreLabel->setPosition(Vec2(size.width / 2.0 * 1.5, size.height - 40));
	this->setScoreLabel(_scoreLabel);
	this->addChild(_scoreLabel);*/

	auto listener = EventListenerTouchOneByOne::create();//EventListenerTouchOneByOne　画面がタッチされたことを取得
	listener->onTouchBegan = [](Touch* touch, Event* event)//タッチイベントの開始処理
	{
		
		return true;//イベントを実行
	};

	listener->onTouchMoved = [this](Touch* touch, Event* event)//タッチ中に動いたときの処理
	{
		//タッチ位置が動いたとき
		//前回とのタッチ位置との差をベクトルで取得する
		Vec2 delta = touch->getDelta();

		//現在の座標を取得する
		Vec2 position = _player->getPosition();

		//現在座標+ 移動量を新たな座標にする
		Vec2 newPosition = position + delta;

		auto winSize = Director::getInstance()->getWinSize();//画面サイズを取得
		newPosition = newPosition.getClampPoint(Vec2(0, position.y), Vec2(winSize.width, position.y));//新しいポジションを取得
		_player->setPosition(newPosition);
	};

	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleUpdate();//Updateを毎フレーム行う
	return true;
	};

Sprite* MainScene::addFruit()
{
	auto winSize = Director::getInstance()->getWinSize();//画面の取り出し
														 //フルーツの種類を選択する
	int fruitType = rand() % static_cast<int>(FruitType::COUNT);

	//フルーツを作成
	std::string filename = StringUtils::format("fruit%d.png", fruitType);
	auto fruit = Sprite::create(filename);
	fruit->setTag(fruitType);//フルーツをタグとして扱う

	auto fruitSize = fruit->getContentSize();//フルーツのサイズを取り出す
	float fruitXPos = rand() % static_cast<int>(winSize.width);//X軸のランダムな位置

	fruit->setPosition(Vec2(fruitXPos, winSize.height - FRUIT_TOP_MARGIN - fruitSize.height / 2.0));//フルーツのポジションを設定
	this->addChild(fruit);
	_fruits.pushBack(fruit);

	//フルーツに動きをつける
	//地面の座標
	auto ground = Vec2(fruitXPos, 0);
	//3秒かけて落下 MoveTo::create(時間,座標)
	auto fall = MoveTo::create(3, ground);

	//removeFruitを即座に呼び出すアクション
	auto remove = CallFuncN::create([this](Node *node){
		//NodeをSpriteにダウンキャストする
		auto sprite = dynamic_cast<Sprite *>(node);

		this->removeFruit(sprite);
	});
	//fallとremoveを連続して実行させるアクション
	auto sequence = Sequence::create(fall, remove, NULL);
	fruit->runAction(sequence);
	return fruit;
}

void MainScene::release(FruitType)
{

}

bool MainScene::removeFruit(cocos2d::Sprite *fruit) 
{
	if(_fruits.contains(fruit)){
		//親ノードから削除する
		fruit->removeFromParent();
		//_fruits配列から削除する
		_fruits.eraseObject(fruit);
		return true;
	}
	return false;
}

void MainScene::update(float dt){
	int random = rand() % FRUIT_SPAWN_RATE;
	if (random == 0){//乱数が0のとき
		this->addFruit();
	}
	for (auto& fruit : _fruits){
		Vec2 busketPosition = _player->getPosition() - Vec2(0, 10);
		Rect boundingBox = fruit->getBoundingBox();
		bool isHit = boundingBox.containsPoint(busketPosition);
		// 当たり判定
		if (isHit){
			this->catchFruit(fruit);
			// ここでデスフラグを立てる
			
		}
		// デスフラグ立っているやつを消す

		// 消したリストを埋める

	}

}
//free(ar);//不要になったらメモリ解放
void MainScene::catchFruit(cocos2d::Sprite *fruit){
	//フルーツを削除
	this->removeFruit(fruit);


}