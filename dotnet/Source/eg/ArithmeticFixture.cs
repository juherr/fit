// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

using fit;

namespace eg
{

	public class ArithmeticFixture : PrimitiveFixture 
	{

		int x=0;
		int y=0;

		public override void doRows(Parse rows) 
		{
			base.doRows(rows.more);    // skip column heads
		}

		public override void doCell(Parse cell, int column) 
		{
			switch (column) 
			{
				case 0: x = (int)parseLong(cell); break;
				case 1: y = (int)parseLong(cell); break;
				case 2: check(cell, x+y); break;
				case 3: check(cell, x-y); break;
				case 4: check(cell, x*y); break;
				case 5: check(cell, x/y); break;
				default: ignore(cell); break;
			}
		}
	}
}