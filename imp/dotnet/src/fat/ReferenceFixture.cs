using System;
using fit;
using System.IO;

namespace fat
{
	public class ReferenceFixture : ColumnFixture
	{
		public string Description;
		public string Location;
		public string Note;

		public string Result()
		{
			string inputFileName = "../../spec/" + Location;
			string outputFileName = "output/spec/" + Location;
			try 
			{
				FileRunner runner = new FileRunner();
				runner.args(new string[]{inputFileName, outputFileName});
				runner.process();
				runner.output.Close();
			
				Counts counts = runner.fixture.counts;
				if ((counts.exceptions == 0) && (counts.wrong == 0)) 
				{
					return "pass";
				}
				else 
				{
					return "fail: " + counts.right + " right, " + counts.wrong + " wrong, " + counts.exceptions + " exceptions";
				}
			}
			catch (IOException) 
			{
				return "file not found: " + new FileInfo(inputFileName).FullName;
			}		
		}
	}
}
