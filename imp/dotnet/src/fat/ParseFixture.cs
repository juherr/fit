using System;
using System.IO;
using fit;

namespace fat
{
	public class ParseFixture : ColumnFixture
	{
		public string Html;
		public string Note;
	
		public String Output() 
		{
			return GenerateOutput(new Parse(Html));
		}

		public String Parse() 
		{
			return dumpTables(new Parse(Html));
		}
				
		private String GenerateOutput(Parse parse) 
		{
			StringWriter result = new StringWriter();
			parse.print(result);
			return result.ToString();
		}
	
		private String dumpTables(Parse table) 
		{
			String result = "";
			String separator = "";
			while (table != null) 
			{
				result += separator;
				result += dumpRows(table.parts);
				separator = "\n----\n";
				table = table.more;
			}
			return result;
		}
	
		private String dumpRows(Parse row) 
		{
			String result = "";
			String separator = "";
			while (row != null) 
			{
				result += separator;
				result += dumpCells(row.parts);
				separator = "\n";
				row = row.more;
			}
			return result;
		}
	
		private String dumpCells(Parse cell) 
		{
			String result = "";
			String separator = "";
			while (cell != null) 
			{
				result += separator;
				result += "[" + cell.text() + "]";
				separator = " ";
				cell = cell.more;
			}
			return result;
		}
	}
}
