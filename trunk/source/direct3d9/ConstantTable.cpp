
/*
* Copyright (c) 2007 SlimDX Group
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
#include <d3d9.h>
#include <d3dx9.h>
#include <vcclr.h>
#include <memory>

#include "../DirectXObject.h"
#include "../Utils.h"

#include "Effect.h"
#include "../Direct3D/..\DataStream.h"
#include "ConstantTable.h"
#include "Device.h"

namespace SlimDX
{
namespace Direct3D9
{
	void ConstantDescription::Initialize( const D3DXCONSTANT_DESC& desc )
	{
		Name = gcnew String( desc.Name );
		RegisterSet = (SlimDX::Direct3D9::RegisterSet) desc.RegisterSet;
		RegisterIndex = desc.RegisterIndex;
		RegisterCount = desc.RegisterCount;
		Class = (ParameterClass) desc.Class;
		Type = (ParameterType) desc.Type;
		Rows = desc.Rows;
		Columns = desc.Columns;
		Elements = desc.Elements;
		StructMembers = desc.StructMembers;
		Bytes = desc.Bytes;
	}

	ConstantTable::ConstantTable( ID3DXConstantTable* table ) : DirectXObject( table )
	{
	}

	ConstantTable::ConstantTable( IntPtr table )
	{
		if( table == IntPtr::Zero )
			throw gcnew ArgumentNullException( "table" );

		void* pointer;
		IUnknown* unknown = (IUnknown*) table.ToPointer();
		HRESULT hr = unknown->QueryInterface( IID_ID3DXConstantTable, &pointer );
		if( FAILED( hr ) )
			throw gcnew InvalidCastException( "Failed to QueryInterface on user-supplied pointer." );

		m_Pointer = (ID3DXConstantTable*) pointer;
	}

	ConstantTable::ConstantTable( IDirect3DDevice9* device,ID3DXConstantTable* constantTable )
		: DirectXObject(constantTable), m_Device(device)
	{
		m_Device->AddRef();
	}

	ConstantTable::~ConstantTable()
	{
		Destruct();
		m_Device->Release();
	}
	
	EffectHandle^ ConstantTable::GetConstant(SlimDX::Direct3D9::EffectHandle ^handle, int index)
	{
		D3DXHANDLE parentHandle = handle != nullptr ? handle->InternalHandle : NULL;
		D3DXHANDLE result = m_Pointer->GetConstant( parentHandle, index );
		
		if( result == NULL )
			return nullptr;
		return gcnew EffectHandle( result );
	}
	
	EffectHandle^ ConstantTable::GetConstant(SlimDX::Direct3D9::EffectHandle ^handle, String^ name )
	{
		array<unsigned char>^ rawName = System::Text::ASCIIEncoding::ASCII->GetBytes( name );
		pin_ptr<unsigned char> pinnedName = &rawName[0];
		
		D3DXHANDLE parentHandle = handle != nullptr ? handle->InternalHandle : NULL;
		D3DXHANDLE result = m_Pointer->GetConstantByName( parentHandle, (const char*) pinnedName );
		
		if( result == NULL )
			return nullptr;
		return gcnew EffectHandle( result );
	}
	
	EffectHandle^ ConstantTable::GetConstantElement( EffectHandle^ handle, int index )
	{
		D3DXHANDLE parentHandle = handle != nullptr ? handle->InternalHandle : NULL;
		D3DXHANDLE result = m_Pointer->GetConstantElement( parentHandle, index );
		
		if( result == NULL )
			return nullptr;
		return gcnew EffectHandle( result );
	}

	ConstantDescription ConstantTable::GetConstantDescription( EffectHandle^ handle )
	{
		D3DXCONSTANT_DESC nativeDesc;
		ConstantDescription desc;

		D3DXHANDLE nativeHandle = handle != nullptr ? handle->InternalHandle : NULL;
		unsigned int count = 1;

		HRESULT hr = m_Pointer->GetConstantDesc( nativeHandle, &nativeDesc, &count );
		GraphicsException::CheckHResult( hr );
		if( FAILED( hr ) )
			return desc;

		desc.Initialize( nativeDesc );
		return desc;
	}

	array<ConstantDescription>^ ConstantTable::GetConstantDescriptionArray( EffectHandle^ handle )
	{
		//TODO: Check that the logic here is actually correct. The SDK doesn't bother to explain.
		D3DXHANDLE nativeHandle = handle != nullptr ? handle->InternalHandle : NULL;
		unsigned int count = 0;

		//Determine the count
		HRESULT hr = m_Pointer->GetConstantDesc( nativeHandle, NULL, &count );
		GraphicsException::CheckHResult( hr );
		if( FAILED( hr ) )
			return nullptr;

		//Get the actual data
		std::auto_ptr<D3DXCONSTANT_DESC> nativeDescArray = new D3DXCONSTANT_DESC[count];
		hr = m_Pointer->GetConstantDesc( nativeHandle, nativeDescArray.get(), &count );
		GraphicsException::CheckHResult( hr );
		if( FAILED( hr ) )
			return nullptr;

		//Marshal the data
		array<ConstantDescription>^ descArray = gcnew array<ConstantDescription>( count );
		for( unsigned int i = 0; i < count; ++i )
		{
			const D3DXCONSTANT_DESC* nativeDesc = nativeDescArray.get() + i;
			descArray[i].Initialize( *nativeDesc );
		}

		return descArray;
	}

	int ConstantTable::GetSamplerIndex( EffectHandle^ sampler )
	{
		D3DXHANDLE handle = sampler != nullptr ? sampler->InternalHandle : NULL;
		int result = m_Pointer->GetSamplerIndex( handle );
		return result;
	}

	DataStream^ ConstantTable::GetBuffer()
	{
		DWORD size = m_Pointer->GetBufferSize();
		void* pointer = m_Pointer->GetBufferPointer();
		if( pointer == NULL )
			return nullptr;

		return gcnew DataStream( pointer, size, true, true, false );
	}

	void ConstantTable::SetDefaults()
	{
		HRESULT hr = m_Pointer->SetDefaults( m_Device );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, bool value )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		HRESULT hr = m_Pointer->SetBool( m_Device, handle, value );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ param, array<bool>^ values )
	{
		//implementing set for bool array is REALLY ANNOYING.
		//Win32 uses BOOL, which is an int
		array<BOOL>^ expandedArray = gcnew array<BOOL>( values->Length );
		Array::Copy( values, expandedArray, values->Length );

		D3DXHANDLE handle = param != nullptr ? param->InternalHandle : NULL;
		pin_ptr<BOOL> pinnedValue = &expandedArray[0];
		HRESULT hr = m_Pointer->SetBoolArray( m_Device, handle, pinnedValue, values->Length );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, int value )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		HRESULT hr = m_Pointer->SetInt( m_Device, handle, value );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, array<int>^ values )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		pin_ptr<int> pinned_value = &values[0];
		HRESULT hr = m_Pointer->SetIntArray( m_Device, handle, pinned_value, values->Length );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, float value )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		HRESULT hr = m_Pointer->SetFloat( m_Device, handle, value );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, array<float>^ values )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		pin_ptr<float> pinned_values = &values[0];
		HRESULT hr = m_Pointer->SetFloatArray( m_Device, handle, pinned_values, values->Length );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, Vector4 value )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		HRESULT hr = m_Pointer->SetVector( m_Device, handle, (const D3DXVECTOR4*) &value );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, array<Vector4>^ values )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		pin_ptr<Vector4> pinned_value = &values[0];
		HRESULT hr = m_Pointer->SetVectorArray( m_Device, handle, (const D3DXVECTOR4*) pinned_value, values->Length );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, ColorValue value )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		HRESULT hr = m_Pointer->SetVector( m_Device, handle, (const D3DXVECTOR4*) &value );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, array<ColorValue>^ values )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		pin_ptr<ColorValue> pinned_value = &values[0];
		HRESULT hr = m_Pointer->SetVectorArray( m_Device, handle, (const D3DXVECTOR4*) pinned_value, values->Length );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, Matrix value )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		HRESULT hr = m_Pointer->SetMatrix( m_Device, handle, (const D3DXMATRIX*) &value );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValue( EffectHandle^ constant, array<Matrix>^ values )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		pin_ptr<Matrix> pinned_value = &values[0];
		HRESULT hr = m_Pointer->SetMatrixArray( m_Device, handle, (const D3DXMATRIX*) pinned_value, values->Length );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValueTranspose( EffectHandle^ constant, Matrix value )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		HRESULT hr = m_Pointer->SetMatrixTranspose( m_Device, handle, (const D3DXMATRIX*) &value );
		GraphicsException::CheckHResult( hr );
	}

	void ConstantTable::SetValueTranspose( EffectHandle^ constant, array<Matrix>^ values )
	{
		D3DXHANDLE handle = constant != nullptr ? constant->InternalHandle : NULL;
		pin_ptr<Matrix> pinned_value = &values[0];
		HRESULT hr = m_Pointer->SetMatrixTransposeArray( m_Device, handle, (const D3DXMATRIX*) pinned_value, values->Length );
		GraphicsException::CheckHResult( hr );
	}

	ConstantTableDescription ConstantTable::Description::get()
	{
		D3DXCONSTANTTABLE_DESC nativeDesc;
		ConstantTableDescription desc;

		HRESULT hr = m_Pointer->GetDesc( &nativeDesc );
		GraphicsException::CheckHResult( hr );
		if( FAILED( hr ) )
			return desc;

		desc.Creator = gcnew String( nativeDesc.Creator );
		desc.Version = gcnew Version( D3DSHADER_VERSION_MAJOR( nativeDesc.Version ), D3DSHADER_VERSION_MINOR( nativeDesc.Version ) );
		desc.Constants = nativeDesc.Constants;
		return desc;
	}
}
}