﻿#include "GameLib/Framework.h"
#include "GameLib/Math.h"
#include "Vector2.h"
#include <sstream>
#include <list>
#include <algorithm>
using namespace std;
using namespace GameLib;

struct Circle{
	Vector2 mPosition;
	Vector2 mVelocity;
};
Circle* gCircles = 0;
const int N = 40; //これの二乗個でやる
const double R = 2.0; //半径2ね
const double RSUM2 = ( R + R ) * ( R + R ); //半径和の二乗
const Vector2 gMinimum( -160.0, -160.0 );
const Vector2 gMaximum( 160.0, 160.0 );
const int gDivision = 30;
const int gBoxListBlockSize = 100; //てきとう
const int gHitListBlockSize = 300; //てきとう
bool testCircles( int index0, int index1 ); //1個づつの判定関数(注:中で力は与えない)
void addForce( int i0, int i1 ); //力を足すのはこっち

//----------------------ここがこの章のミソ--------------Z--------------------------

//当たったペア
struct HitPair{ 
	bool operator<( const HitPair& a ) const { //ソートに必要な不等号
		if ( mI0 < a.mI0 ){
			return true;
		}else if ( mI0 > a.mI0 ){
			return false;
		}else{
			return ( mI1 < a.mI1 );
		}
	}
	bool operator==( const HitPair& a ) const {//uniqueに必要な統合
		return ( ( mI0 == a.mI0 ) && ( mI1 == a.mI1 ) );
	}
	int mI0;
	int mI1;
};

void processCollision( int* test, int* hit ){
	*test = 0;
	*hit = 0;
#if 1
	int n = N*N; //個数ね
	//箱のサイズを計算
	Vector2 boxSize; //箱サイズ
	boxSize.setSub( gMaximum, gMinimum );
	boxSize *= 1.0 / static_cast< float >( gDivision );
	//リスト用意
	list< int* >* boxList = new list< int* >[ gDivision * gDivision ];
	int* boxListBlockPos = new int[ gDivision * gDivision ];
	for ( int i = 0; i < gDivision * gDivision; ++i ){ //リスト位置を初期化。
		boxListBlockPos[ i ] = gBoxListBlockSize;
	}
	for ( int i = 0; i < n; ++i ){
		Vector2 t;
		t.setSub( gCircles[ i ].mPosition, gMinimum );
		double minX = t.x - R;
		double maxX = t.x + R;
		double minY = t.y - R;
		double maxY = t.y + R;
		//箱番号に変換(除算は高速化できるね？)
		int minXBox = static_cast< int >( minX / boxSize.x );
		int maxXBox = static_cast< int >( maxX / boxSize.x );
		int minYBox = static_cast< int >( minY / boxSize.y );
		int maxYBox = static_cast< int >( maxY / boxSize.y );
		ASSERT( minXBox >= 0 && maxXBox < gDivision && minYBox >=0 && maxYBox < gDivision );
		for ( int j = minXBox; j <= maxXBox; ++j ){
			for ( int k = minYBox; k <= maxYBox; ++k ){
				int boxIndex = k * gDivision + j; //箱番号は「Y * (分割数) + X」
				if ( boxListBlockPos[ boxIndex ] == gBoxListBlockSize ){
					boxList[ boxIndex ].push_back( new int[ gBoxListBlockSize ] );
					boxListBlockPos[ boxIndex ] = 0;
				}
				int* a = boxList[ boxIndex ].back();
				a[ boxListBlockPos[ boxIndex ] ] = i;
				++boxListBlockPos[ boxIndex ];
			}
		}
	}
	//箱ごとに総当り。ここのコードはひどく複雑なので、少しでも短くするよう別名を多用する。
	//理想的にはlistと同じように使える形でlist< int* >を整備してから作るべきである。
	//それを怠るとどうなるのかを示すために敢えてベタのまま書こう。非常に苦労したとだけは言っておく。
	list< HitPair* > hitList;
	int hitListBlockPos = gHitListBlockSize;
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		//別名準備
		typedef list< int* >::const_iterator It; //別名
		const list< int* >& l = boxList[ i ]; //別名
		int blockPos = boxListBlockPos[ i ];
		int blockNumber = static_cast< int >( l.size() );
		int lastBlock = blockNumber - 1;
		It it0 = l.begin();
		int blockProcessed0 = 0;
		while ( it0 != l.end() ){
			//最後のブロックならブロック内位置。それ以外はブロックサイズ
			int n0 = ( blockProcessed0 != lastBlock ) ? gBoxListBlockSize : blockPos;
			for ( int j = 0; j < n0; ++j ){
				It it1 = it0;
				int blockProcessed1 = blockProcessed0; //1個目のブロックから開始。この前は不要。
				while ( it1 != l.end() ){
					//最後のブロックならブロック内位置。それ以外はブロックサイズ
					int n1 = ( blockProcessed1 != lastBlock ) ? gBoxListBlockSize : blockPos;
					int kBegin = ( it0 == it1 ) ? ( j + 1 ) : 0; //同じブロック間ならk=j+1。それ以外は0から。
					for ( int k = kBegin; k < n1; ++k ){
						int i0 = ( *it0 )[ j ];
						int i1 = ( *it1 )[ k ];
						++( *test );
						if ( testCircles( i0, i1 ) ){
							++( *hit );
							HitPair hit;
							//同じペアは同じでないと困るので、小さい方を前にする
							if ( i0 < i1 ){
								hit.mI0 = i0;
								hit.mI1 = i1;
							}else{
								hit.mI0 = i0;
								hit.mI1 = i1;
							}
							if ( hitListBlockPos == gHitListBlockSize ){
								hitList.push_back( new HitPair[ gHitListBlockSize ] );
								hitListBlockPos = 0;
							}
							hitList.back()[ hitListBlockPos ] = hit;
							++hitListBlockPos;
						}
					}
					++blockProcessed1;
					++it1; //もうどの括弧がどこなのかさっぱりわからない。
				}
			}
			++blockProcessed0;
			++it0;
		}
	}
	//当たったリストを配列にコピー。ソートしたいからだ。
	int blockNumber = static_cast< int >( hitList.size() );
	HitPair* hitArray = 0;
	if ( blockNumber > 0 ){
		int hitArraySize = ( blockNumber - 1 ) * gHitListBlockSize + hitListBlockPos;
		hitArray = new HitPair[ hitArraySize ];
		int blockCopied = 0;
		list< HitPair* >::iterator hitListIt = hitList.begin();
		while ( blockCopied < ( blockNumber - 1 ) ){
			for ( int i = 0; i < gHitListBlockSize; ++i ){
				hitArray[ blockCopied * gHitListBlockSize + i ] = ( *hitListIt )[ i ];
			}
			++blockCopied;
			++hitListIt;
		}
		//最終ブロック
		for ( int i = 0; i < hitListBlockPos; ++i ){
			hitArray[ blockCopied * gHitListBlockSize + i ] = hitList.back()[ i ];
		}
		//ソート
		sort( hitArray, hitArray + hitArraySize );
		//重複を除く
		size_t hitN = unique( hitArray, hitArray + hitArraySize ) - hitArray;
		//力をかける
		for ( size_t i = 0; i < hitN; ++i ){
			addForce( hitArray[ i ].mI0, hitArray[ i ].mI1 );
		}
		//後始末
		for ( list< HitPair* >::iterator i = hitList.begin(); i != hitList.end(); ++i ){
			SAFE_DELETE_ARRAY( *i );
		}
		SAFE_DELETE_ARRAY( hitArray );
	}
	//箱リストはもう要らない。削除は結構面倒である。
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		for ( list< int* >::iterator j = boxList[ i ].begin(); j != boxList[ i ].end(); ++j ){
			SAFE_DELETE_ARRAY( *j );
		}
	}
	SAFE_DELETE_ARRAY( boxList );
	SAFE_DELETE_ARRAY( boxListBlockPos );

#else //デバグ用総当り版
	for ( int i = 0; i < N*N; ++i ){
		for ( int j = i + 1; j < N*N; ++j ){
			++( *test );
			if ( testCircles( i, j ) ){
				++( *hit );
			}
		}
	}
#endif
}

//----------------------ここから下はこの章の本筋ではないコード---------------------------

//2個のcircleを処理する中身。当たるとtrue
bool testCircles( int i0, int i1 ){
	Circle& c0 = gCircles[ i0 ];
	const Vector2& p0 = c0.mPosition;
	Circle& c1 = gCircles[ i1 ];
	const Vector2& p1 = c1.mPosition;
	//距離は？
	Vector2 t;
	t.setSub( p1, p0 );
	double sql = t.squareLength();
	if ( sql < RSUM2 ){ //半径は4でいいだろ
		return true;
	}else{
		return false;
	}
}

void addForce( int i0, int i1 ){
	Vector2 t;
	t.setSub( gCircles[ i0 ].mPosition, gCircles[ i1 ].mPosition );
	double l = t.length();
	t *= 0.25 / l; //適当に長さを調整
	//はじき返す。tはp0->p1のベクタだから、これをc1に足し、c0から引く。
	gCircles[ i0 ].mVelocity += t;
	gCircles[ i1 ].mVelocity -= t;
}

namespace GameLib{
	void Framework::update(){
		if ( !gCircles ){
			gCircles = new Circle[ N*N ];
			//初期配置
			for ( int i = 0; i < N*N; ++i ){
				gCircles[ i ].mPosition.set( 
					static_cast< double >( ( ( i % N ) - N/2 ) * 4 ) + 0.001 * i, //ちょっとずらす
					static_cast< double >( ( ( i / N ) - N/2 ) * 4 ) );
			}
		}
		//加速度初期化
		for ( int i = 0;i < N*N; ++i ){
			//加速度を原点への引力で初期化
			gCircles[ i ].mVelocity.setMul( gCircles[ i ].mPosition, -0.001 );
		}
		int test;
		int hit;
		processCollision( &test, &hit ); //衝突検出関数

		//更新
		for ( int i = 0;i < N*N; ++i ){
			gCircles[ i ].mPosition += gCircles[ i ].mVelocity;

			//描画
			double p[ 4 ][ 2 ];
			p[ 0 ][ 0 ] = p[ 1 ][ 0 ] = gCircles[ i ].mPosition.x - 0.5 + 160.0;
			p[ 2 ][ 0 ] = p[ 3 ][ 0 ] = gCircles[ i ].mPosition.x + 0.5 + 160.0;
			p[ 0 ][ 1 ] = p[ 2 ][ 1 ] = gCircles[ i ].mPosition.y - 0.5 + 120.0;
			p[ 1 ][ 1 ] = p[ 3 ][ 1 ] = gCircles[ i ].mPosition.y + 0.5 + 120.0;
			drawTriangle2D( p[ 0 ], p[ 1 ], p[ 2 ] );
			drawTriangle2D( p[ 3 ], p[ 1 ], p[ 2 ] );
		}
		ostringstream oss;
		oss << frameRate() << " " << test << " " << hit;
		drawDebugString( 0, 0, oss.str().c_str() );
	}
}
