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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		//背景の初期化
		this->render2D_Priority[1] = 0.9f;
		this->BackImg = "BGImg";
		DG::Image_Create(this->BackImg, "./data/image/back.jpg");

		//カードの初期化
		this->render2D_Priority[1] = 0.5f;
		this->CardImg = "CardImg";
		DG::Image_Create(this->CardImg, "./data/image/card.png");

		this->cardposx = 0;
		this->cardposy = 0;

		//カーソルの初期化
		this->render2D_Priority[1] = 0.3f;
		this->PlayerImg = "PlayerImg";
		DG::Image_Create(this->PlayerImg, "./data/image/card.png");

		this->posx = 0;
		this->posy = 0;

		this->motion = 0;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		DG::Image_Erase(this->BackImg);
		DG::Image_Erase(this->CardImg);
		DG::Image_Erase(this->PlayerImg);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto  in = DI::GPad_GetState("P1");

		//プレイヤーの移動
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
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景の描画
		ML::Box2D  bg_draw(0, 0, 1280, 720);
		ML::Box2D  bg_src(0, 0, 620, 439);
		DG::Image_Draw(this->BackImg, bg_draw, bg_src);

		//カードの描画
		for (int j = 0; j < 3; j++) {
			for (int i = 0; i < 6; i++) {
				ML::Box2D card_draw(i * 160, j * 210, 150, 200);
				ML::Box2D card_src(6 * 300, 0, 300, 400);
				DG::Image_Draw(this->CardImg, card_draw, card_src);
			}
		}

		//カーソルの描画
		ML::Box2D draw(0, 0, 150, 200);
		ML::Box2D src(6 * 300, 400, 300, 400);
		draw.Offset(this->posx, this->posy);
		DG::Image_Draw(this->PlayerImg, draw, src);
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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