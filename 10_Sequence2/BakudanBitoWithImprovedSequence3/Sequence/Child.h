﻿#ifndef INCLUDED_SEQUENCE_CHILD_H
#define INCLUDED_SEQUENCE_CHILD_H

#include "Sequence/Base.h"

namespace Sequence{
class Parent;

class Child : public Base{
public:
	virtual ~Child();
	Base* update( Base* ); //これは実体を作ってやる
	virtual Base* update( Parent* ) = 0;
};

} //namespace Sequence

#endif