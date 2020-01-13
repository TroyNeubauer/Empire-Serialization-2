//===================================================================================
// MIT Liscense
// Copyright (c) 2020, Troy Neubauer
//
//	File: EmpierSerialization.h
//	This file exports all of EmpierSerialization public symbols
// 

#pragma once


#include "EmpireSerialization/Format.h"
#include "EmpireSerialization/ForwardTypes.h"
#include "EmpireSerialization/Error.h"
#include "EmpireSerialization/Charsets.h"
#include "EmpireSerialization/Conversions.h"
#include "EmpireSerialization/ESString.h"

namespace ES {

	using AllocErrorHandler = std::add_pointer<void()>::type;

	void SetAllocErrorHandler(AllocErrorHandler handler);
	void Init();

}