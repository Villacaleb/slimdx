﻿// Copyright (c) 2007-2010 SlimDX Group
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
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Generator.ObjectModel
{
	abstract class BaseElement
	{
		string name;

		public SourceModel Model
		{
			get;
			private set;
		}

		public string Name
		{
			get { return name; }
			protected set
			{
				name = value;
				RebuildName();
			}
		}

		public string NiceName
		{
			get;
			private set;
		}

		protected BaseElement(SourceModel model)
		{
			Model = model;
		}

		protected BaseElement(SourceModel model, string name)
			: this(model)
		{
			Name = name;
		}

		public override string ToString()
		{
			return Name;
		}

		protected virtual string BuildNiceName(string name)
		{
			return Model.NameRules.PascalCaseFromUnderscores(Model.NameRules.RemovePrefixes(name));
		}

		protected void RebuildName()
		{
			NiceName = BuildNiceName(Name);
		}

		protected void RegisterType()
		{
			Model.TypeMap.Add(Name, NiceName);
		}
	}
}
