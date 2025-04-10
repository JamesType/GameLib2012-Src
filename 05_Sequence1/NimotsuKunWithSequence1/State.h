﻿#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "GameLib/GameLib.h"
#include "Array2D.h"

class Image; //宣言だけしておけばいい。インクルードしなくていい。

//状態クラス
class State{
public:
	State( const char* stageData, int size );
	~State();
	void update( int moveX, int moveY );
	void draw() const;
	bool hasCleared() const;
	static const int MAX_MOVE_COUNT = 15;
private:
	class Object;
	void setSize( const char* stageData, int size );

	int mWidth;
	int mHeight;
	Array2D< Object > mObjects;
	Image* mImage; //画像
	int mMoveCount;
};

#endif
