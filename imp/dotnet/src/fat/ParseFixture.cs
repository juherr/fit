using System;
using System.Text.RegularExpressions;
using System.IO;
using fit;

namespace fat
{
	public class ParseFixture : ColumnFixture
	{
		public string Html;
		public string TableCell;
		public string Entity;
		public string Note;

		private Parse GenerateParse() 
		{
			int inputColumns = 0;
			String html = null;
			if (Html != null) 
			{
				inputColumns++;
				html = Html;
			}
			if (TableCell != null) 
			{
				inputColumns++;
				html = "<table><tr>" + TableCell + "</tr></table>";
			}
			if (Entity != null) 
			{
				inputColumns++;
				html = "<table><tr><td>" + Entity + "</td></tr></table>";
			}
		
			if (inputColumns != 1) 
			{
				throw new ApplicationException("Exactly ONE of the following columns is needed: 'Html', 'TableCell', or 'Entity'");
			}

			html = html.Replace("\\u00a0", "\u00a0");
			return new Parse(html);
		}
	
		public String Output() 
		{
			return GenerateOutput(GenerateParse());
		}
	
		private String GenerateOutput(Parse parse) 
		{
			StringWriter result = new StringWriter();
			parse.print(result);
			return result.ToString();
		}
	
		public String Parse() 
		{
			return dumpTables(GenerateParse());		
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
				result += "[" + escapeAscii(cell.text()) + "]";
				separator = " ";
				cell = cell.more;
			}
			return result;
		}

		private String escapeAscii(String text) 
		{
			text = Regex.Replace(text, "\\x0a", "\\n");
			text = Regex.Replace(text, "\\x0d", "\\r");
			text = Regex.Replace(text, "\\xa0", "\\u00a0");
			return text;
		}
	}
}
