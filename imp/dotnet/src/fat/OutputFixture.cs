using System;
using System.IO;
using System.Text.RegularExpressions;
using fit;

namespace fat
{
	public class OutputFixture : ColumnFixture
	{
		public String Text;

		public String CellOutput() 
		{
			Parse cell = new Parse("td", "", null, null);
			cell.leader = "";
			cell.body = Fixture.escape(Unescape(Text));
			return GenerateOutput(cell);
		}

		private String Unescape(String text) 
		{
			text = Regex.Replace(text, "\\\\n", "\n");
			text = Regex.Replace(text, "\\\\r", "\r");
			return text;
		}
	
		private String GenerateOutput(Parse parse) 
		{
			StringWriter result = new StringWriter();
			parse.print(result);
			return result.ToString();
		}
	}
}
