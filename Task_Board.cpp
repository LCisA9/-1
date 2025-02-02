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
	//\[Xฬ๚ป
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//\[Xฬ๐๚
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//u๚ปv^XNถฌษP๑พฏsค
	bool  Object::Initialize()
	{
		//X[p[NX๚ป
		__super::Initialize(defGroupName, defName, true);
		//\[XNXถฌor\[XคL
		this->res = Resource::Create();

		//f[^๚ป
		//wiฬ๚ป
		this->render2D_Priority[1] = 0.9f;
		this->BackImg = "BGImg";
		DG::Image_Create(this->BackImg, "./data/image/back.jpg");

		//J[hฬ๚ป
		this->render2D_Priority[1] = 0.5f;
		this->CardImg = "CardImg";
		DG::Image_Create(this->CardImg, "./data/image/card.png");

		this->cardposx = 0;
		this->cardposy = 0;

		//J[\ฬ๚ป
		this->render2D_Priority[1] = 0.3f;
		this->PlayerImg = "PlayerImg";
		DG::Image_Create(this->PlayerImg, "./data/image/card.png");

		this->posx = 0;
		this->posy = 0;

		this->motion = 0;
		//^XNฬถฌ

		return  true;
	}
	//-------------------------------------------------------------------
	//uIนv^XNมลษP๑พฏsค
	bool  Object::Finalize()
	{
		//f[^^XN๐๚
		DG::Image_Erase(this->BackImg);
		DG::Image_Erase(this->CardImg);
		DG::Image_Erase(this->PlayerImg);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//๘ซpฌ^XNฬถฌ
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[ษsค
	void  Object::UpDate()
	{
		auto  in = DI::GPad_GetState("P1");

		//vC[ฬฺฎ
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
	//uQc`ๆvPt[ษsค
	void  Object::Render2D_AF()
	{
		//wiฬ`ๆ
		ML::Box2D  bg_draw(0, 0, 1280, 720);
		ML::Box2D  bg_src(0, 0, 620, 439);
		DG::Image_Draw(this->BackImg, bg_draw, bg_src);

		//J[hฬ`ๆ
		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 6; i++) {
				ML::Box2D card_draw(i * 160, j * 210, 150, 200);
				ML::Box2D card_src(6 * 300, 0, 300, 400);
				DG::Image_Draw(this->CardImg, card_draw, card_src);
			}
		}

		//J[\ฬ`ๆ
		ML::Box2D draw(0, 0, 150, 200);
		ML::Box2D src(6 * 300, 400, 300, 400);
		draw.Offset(this->posx, this->posy);
		DG::Image_Draw(this->PlayerImg, draw, src);
	}

	//
	//ศบอ๎{IษฯXsvศ\bh
	//
	//-------------------------------------------------------------------
	//^XNถฌ๛
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWษo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYษธsตฝ็Kill
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
	//\[XNXฬถฌ
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