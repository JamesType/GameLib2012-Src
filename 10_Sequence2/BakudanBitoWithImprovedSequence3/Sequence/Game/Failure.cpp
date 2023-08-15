#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/GameOver.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Failure::Failure() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Failure::~Failure(){
	SAFE_DELETE( mImage );
}

Base* Failure::update( Parent* parent ){
	Base* next = this;
	if ( mCount == 60 ){ //1秒待つ 
		if ( parent->lifeNumber() == 0 ){
			next = new GameOver;
		}else{
			next = new Ready;
		}
	}
	//描画
	//まずゲーム画面を描画
	parent->drawState();
	//上に祝いのメッセージを描画
	mImage->draw();
	//クリアーと字を描く
	Framework::instance().drawDebugString( 0, 0, "ｱｰｯ!" );

	++mCount;

	return next;
}

} //namespace Game
} //namespace Sequence
