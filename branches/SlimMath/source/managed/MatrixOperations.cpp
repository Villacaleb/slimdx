/*
* Copyright (c) 2007-2009 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <windows.h>
#include <xnamath.h>

#include "..\native\NativeOperations.h"

#include "Handle.h"
#include "MatrixOperations.h"

using namespace System;

namespace SlimMath
{
	Operation<Handle<Matrix>^>^ MatrixOps::Multiply(Handle<Matrix>^ value1, Handle<Matrix>^ value2)
	{
		return gcnew Operation<Handle<Matrix>^>(gcnew Handle<Matrix>(), gcnew array<BaseHandle^>(2) { value1, value2 }, 
			NativeOperation::Matrix::Multiply);
	}

	Operation<Handle<Matrix>^>^ MatrixOps::Identity()
	{
		return gcnew Operation<Handle<Matrix>^>(gcnew Handle<Matrix>(), gcnew array<BaseHandle^>(0), 
			NativeOperation::Matrix::Identity);
	}

	Operation<Handle<Matrix>^>^ MatrixOps::Translation(Handle<Vector>^ value)
	{
		return gcnew Operation<Handle<Matrix>^>(gcnew Handle<Matrix>(), gcnew array<BaseHandle^>(1) { value }, 
			NativeOperation::Matrix::TranslationFromVector);
	}

	Operation<MatrixInverseHandle^>^ MatrixOps::Inverse(Handle<Matrix>^ matrix)
	{
		return gcnew Operation<MatrixInverseHandle^>(gcnew MatrixInverseHandle(), gcnew array<BaseHandle^>(1) { matrix },
			NativeOperation::Matrix::Inverse);
	}
}