#include "MainScene.h"

USING_NS_CC;//namespace

//�t���[�c�̉�ʏ�[����̃}�[�W��
const int FRUIT_TOP_MARGIN = 40;
//�t���[�c�̏o����
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
	//��ʃT�C�Y���o��
	auto size = director->getWinSize();
	//�w�i�̃X�v���C�g�𐶐�����
	auto background = Sprite::create("background.png");
	//�\���ʒu
	background->setPosition(Vec2(size.width / 2.0, size.height / 2.0));
	//�m�[�h�ɃX�v���C�g����
	this->addChild(background);
	
	this->setPlayer(Sprite::create("player.png"));//player�摜��ǂݍ���
	_player->setPosition(Vec2(size.width / 2.0, size.height - 445));//_player�̈ʒu
	this->addChild(_player);//�V�[����_player��z�u

	/*auto scoreLabel = Label::createWithSystemFont(StringUtils::toString(_score),"Marker Felt",16);//_scoreLabel�𕶎����
	scoreLabel->setPosition(Vec2(size.width / 2.0 * 1.5, size.height - 40));
	this->setScoreLabel(_scoreLabel);
	this->addChild(_scoreLabel);*/

	auto listener = EventListenerTouchOneByOne::create();//EventListenerTouchOneByOne�@��ʂ��^�b�`���ꂽ���Ƃ��擾
	listener->onTouchBegan = [](Touch* touch, Event* event)//�^�b�`�C�x���g�̊J�n����
	{
		
		return true;//�C�x���g�����s
	};

	listener->onTouchMoved = [this](Touch* touch, Event* event)//�^�b�`���ɓ������Ƃ��̏���
	{
		//�^�b�`�ʒu���������Ƃ�
		//�O��Ƃ̃^�b�`�ʒu�Ƃ̍����x�N�g���Ŏ擾����
		Vec2 delta = touch->getDelta();

		//���݂̍��W���擾����
		Vec2 position = _player->getPosition();

		//���ݍ��W+ �ړ��ʂ�V���ȍ��W�ɂ���
		Vec2 newPosition = position + delta;

		auto winSize = Director::getInstance()->getWinSize();//��ʃT�C�Y���擾
		newPosition = newPosition.getClampPoint(Vec2(0, position.y), Vec2(winSize.width, position.y));//�V�����|�W�V�������擾
		_player->setPosition(newPosition);
	};

	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleUpdate();//Update�𖈃t���[���s��
	return true;
	};

Sprite* MainScene::addFruit()
{
	auto winSize = Director::getInstance()->getWinSize();//��ʂ̎��o��
														 //�t���[�c�̎�ނ�I������
	int fruitType = rand() % static_cast<int>(FruitType::COUNT);

	//�t���[�c���쐬
	std::string filename = StringUtils::format("fruit%d.png", fruitType);
	auto fruit = Sprite::create(filename);
	fruit->setTag(fruitType);//�t���[�c���^�O�Ƃ��Ĉ���

	auto fruitSize = fruit->getContentSize();//�t���[�c�̃T�C�Y�����o��
	float fruitXPos = rand() % static_cast<int>(winSize.width);//X���̃����_���Ȉʒu

	fruit->setPosition(Vec2(fruitXPos, winSize.height - FRUIT_TOP_MARGIN - fruitSize.height / 2.0));//�t���[�c�̃|�W�V������ݒ�
	this->addChild(fruit);
	_fruits.pushBack(fruit);

	//�t���[�c�ɓ���������
	//�n�ʂ̍��W
	auto ground = Vec2(fruitXPos, 0);
	//3�b�����ė��� MoveTo::create(����,���W)
	auto fall = MoveTo::create(3, ground);

	//removeFruit�𑦍��ɌĂяo���A�N�V����
	auto remove = CallFuncN::create([this](Node *node){
		//Node��Sprite�Ƀ_�E���L���X�g����
		auto sprite = dynamic_cast<Sprite *>(node);

		this->removeFruit(sprite);
	});
	//fall��remove��A�����Ď��s������A�N�V����
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
		//�e�m�[�h����폜����
		fruit->removeFromParent();
		//_fruits�z�񂩂�폜����
		_fruits.eraseObject(fruit);
		return true;
	}
	return false;
}

void MainScene::update(float dt){
	int random = rand() % FRUIT_SPAWN_RATE;
	if (random == 0){//������0�̂Ƃ�
		this->addFruit();
	}
	for (auto& fruit : _fruits){
		Vec2 busketPosition = _player->getPosition() - Vec2(0, 10);
		Rect boundingBox = fruit->getBoundingBox();
		bool isHit = boundingBox.containsPoint(busketPosition);
		// �����蔻��
		if (isHit){
			this->catchFruit(fruit);
			// �����Ńf�X�t���O�𗧂Ă�
			
		}
		// �f�X�t���O�����Ă���������

		// ���������X�g�𖄂߂�

	}

}
//free(ar);//�s�v�ɂȂ����烁�������
void MainScene::catchFruit(cocos2d::Sprite *fruit){
	//�t���[�c���폜
	this->removeFruit(fruit);


}