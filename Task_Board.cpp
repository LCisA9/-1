//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Board.h"
#include  "easing.h"

namespace  Board
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		//�w�i�̏�����
		this->render2D_Priority[1] = 0.9f;
		this->BackImg = "BGImg";
		DG::Image_Create(this->BackImg, "./data/image/back.jpg");

		//�J�[�h�̏�����
		this->render2D_Priority[1] = 0.5f;
		this->CardImg = "CardImg";
		DG::Image_Create(this->CardImg, "./data/image/card.png");

		this->cardposx = 0;
		this->cardposy = 0;

		//�J�[�\���̏�����
		this->render2D_Priority[1] = 0.3f;
		this->PlayerImg = "PlayerImg";
		DG::Image_Create(this->PlayerImg, "./data/image/card.png");

		this->posx = 0;
		this->posy = 0;

		this->motion = 0;
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		DG::Image_Erase(this->BackImg);
		DG::Image_Erase(this->CardImg);
		DG::Image_Erase(this->PlayerImg);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto  in = DI::GPad_GetState("P1");

		//�v���C���[�̈ړ�
		if (this->motion == 0) {
			if (in.LStick.L.down) {
				easing::Create(this->PlayerImg, easing::CIRCOUT, this->posx, this->posx - 160, 15);
				easing::Start(this->PlayerImg);
				this->keepx = this->posx;
				this->motion = 1;
			}
			if (in.LStick.R.down) {
				easing::Create(this->PlayerImg, easing::CIRCOUT, this->posx, this->posx + 160, 15);
				easing::Start(this->PlayerImg);
				this->keepx = this->posx;
				this->motion = 1;
			}
			if (in.LStick.U.down) {
				easing::Create(this->PlayerImg, easing::CIRCOUT, this->posy, this->posy - 210, 15);
				easing::Start(this->PlayerImg);
				this->keepy = this->posy;
				this->motion = 2;
			}
			if (in.LStick.D.down) {
				easing::Create(this->PlayerImg, easing::CIRCOUT, this->posy, this->posy + 210, 15);
				easing::Start(this->PlayerImg);
				this->keepy = this->posy;
				this->motion = 2;
			}
		}
		if (this->motion == 1) {
			this->posx = easing::GetPos(this->PlayerImg);
			if (this->keepx + 160 == this->posx || this->keepx - 160 == this->posx) {
				this->motion = 0;
			}
		}
		if (this->motion == 2) {
			this->posy = easing::GetPos(this->PlayerImg);
			if (this->keepy + 210 == this->posy || this->keepy - 210 == this->posy) {
				this->motion = 0;
			}
		}
		easing::UpDate();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�̕`��
		ML::Box2D  bg_draw(0, 0, 1280, 720);
		ML::Box2D  bg_src(0, 0, 620, 439);
		DG::Image_Draw(this->BackImg, bg_draw, bg_src);

		//�J�[�h�̕`��
		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 6; i++) {
				ML::Box2D card_draw(i * 160, j * 210, 150, 200);
				ML::Box2D card_src(6 * 300, 0, 300, 400);
				DG::Image_Draw(this->CardImg, card_draw, card_src);
			}
		}

		//�J�[�\���̕`��
		ML::Box2D draw(0, 0, 150, 200);
		ML::Box2D src(6 * 300, 400, 300, 400);
		draw.Offset(this->posx, this->posy);
		DG::Image_Draw(this->PlayerImg, draw, src);
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}