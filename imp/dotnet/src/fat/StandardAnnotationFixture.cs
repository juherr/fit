using System;
using fit;
using System.IO;

namespace fat
{
	public class StandardAnnotationFixture : ColumnFixture
	{
		public String OriginalHTML = "Text";
		public String Annotation;
		public String Text;
	
		public String Output() 
		{
			Parse parse = new Parse(OriginalHTML, new String[] {"td"});
			Fixture testbed = new Fixture();
		
			if (Annotation.Equals("right")) testbed.right(parse);
			if (Annotation.Equals("wrong")) testbed.wrong(parse, Text);
			if (Annotation.Equals("error")) testbed.error(parse, Text);
			if (Annotation.Equals("info")) testbed.info(parse, Text); 
			if (Annotation.Equals("ignore")) testbed.ignore(parse);
				
			return GenerateOutput(parse); 
		}
	
		public override void doCell(Parse cell, int column) 
		{
			try 
			{
				if (column == 4) 
				{
					cell.body = RenderedOutput();
				}
				else 
				{
					base.doCell(cell, column);
				}
			}
			catch (Exception e) 
			{
				exception(cell, e);
			}	
		}
	
		public String RenderedOutput() 
		{
			return "<table border='1'><tr>" + Output() + "</tr></table>";		
		}
	
		// code smell note: copied from DocumentParseFixture	
		private String GenerateOutput(Parse parse) 
		{
			StringWriter result = new StringWriter();
			parse.print(result);
			return result.ToString().Trim();
		}
	}
}
