﻿// Copyright (c) 2007-2011 SlimDX Group
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

using System;
using System.Runtime.InteropServices;
using System.Security;

namespace SlimDX.DXGI
{
	/// <summary>
	/// The access point for the Direct3D 11 API.
	/// </summary>
	public static class Direct3D11
	{
		#region Interface

		public static ID3D11Device CreateDevice()
		{
			IntPtr devicePointer = IntPtr.Zero;
			IntPtr contextPointer = IntPtr.Zero;
			IntPtr featureLevel = IntPtr.Zero;
			int result = CreateD3D11Device(
				IntPtr.Zero,
				1, /* hardware */
				IntPtr.Zero,
				2,
				IntPtr.Zero,
				0,
				sdkVersion,
				out devicePointer,
				out featureLevel,
				out contextPointer
			);

			return new ID3D11Device(devicePointer);
		}

		#endregion
		#region Implementation

		const int sdkVersion = 7;

		[DllImport("d3d11.dll", EntryPoint = "D3D11CreateDevice", CallingConvention = CallingConvention.StdCall, PreserveSig = true), SuppressUnmanagedCodeSecurity]
		private extern static int CreateD3D11Device(IntPtr adapter, int driverType, IntPtr softwareRasterizer, int flags, IntPtr featureLevels, int featureLevelCount, int sdkVersion, out IntPtr device, out IntPtr featureLevel, out IntPtr immediateContext);

		#endregion
	}
}