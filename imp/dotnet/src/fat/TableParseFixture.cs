using System;
using fit;

namespace fat
{
	public class TableParseFixture : ColumnFixture
	{
		public String HTML;
		public int Row;
		public int Column;
			
		public String CellBody() 
		{
			return cell().body;
		}
			
		public String CellTag() 
		{
			return cell().tag;
		}
			
		public String RowTag() 
		{
			return row().tag;
		}
			
		public String TableTag() 
		{
			return table().tag;
		}
				
		private Parse table() 
		{
			return new Parse(HTML);
		}
			
		private Parse row() 
		{
			return table().at(0, Row - 1);
		}
			
		private Parse cell() 
		{
			return row().at(0, Column - 1);
		}
	}
}
