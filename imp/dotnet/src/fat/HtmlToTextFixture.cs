using System;
using System.IO;
using fit;

namespace fat
{
	public class HtmlToTextFixture : ColumnFixture
	{
		public string HTML;

		public String Text() 
		{
			HTML = HTML.Replace("\\u00a0", "\u00a0");
			return escapeAscii(Parse.htmlToText(HTML));
		}

		private String escapeAscii(String text) 
		{
			text = text.Replace("\x0a", "\\n");
			text = text.Replace("\x0d", "\\r");
			text = text.Replace("\xa0", "\\u00a0");
			return text;
		}
	}
}
