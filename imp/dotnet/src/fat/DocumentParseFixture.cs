using System;
using System.IO;
using fit;

namespace fat
{
	public class DocumentParseFixture : ColumnFixture
	{
		public string HTML;
		public string Note;
	
		public String Output() 
		{
			return GenerateOutput(new Parse(HTML));
		}

		public String Structure() 
		{
			return dumpTables(new Parse(HTML));
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
				result += "[" + cell.body + "]";
				separator = " ";
				cell = cell.more;
			}
			return result;
		}
	}
}
