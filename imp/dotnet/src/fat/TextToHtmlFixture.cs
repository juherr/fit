using System;
using System.IO;
using fit;

namespace fat
{
	public class TextToHtmlFixture : ColumnFixture
	{
		public string Text;

		public string HTML() 
		{
			Text = unescapeAscii(Text);
			return Fixture.escape(Text);
		}

		private string unescapeAscii(string text) 
		{
			text = text.Replace("\\n", "\n");
			text = text.Replace("\\r", "\r");
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