﻿#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"
#include "State.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

void Clear::update( Parent* parent ){
	if ( mCount == 60 ){ //60フレームで移動
		parent->moveTo( Parent::SEQ_STAGE_SELECT );
	}

	//描画
	//まずゲーム画面を描画
	parent->state()->draw();
	//上に祝いのメッセージを描画
	mImage->draw();
	//クリアーと字を描く
	Framework::instance().drawDebugString( 0, 0, "ｸﾘｱｰ!" );

	++mCount;
}

} //namespace Game
} //namespace Sequence
