// Copyright (c) 2002, 2003 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

using System;
using NUnit.Framework;
using System.Reflection;
using System.IO;

namespace fit 
{
 	[TestFixture]
	public class FrameworkTest
	{
		[Test]
		public void TestParsing() 
		{
			Parse p = new Parse("leader<Table foo=2>body</table>trailer", new string[] {"table"});
			Assert.AreEqual("leader", p.leader);
			Assert.AreEqual("<Table foo=2>", p.tag);
			Assert.AreEqual("body", p.body);
			Assert.AreEqual("trailer", p.trailer);
		}

		[Test]
		public void testRecursing() 
		{
			Parse p = new Parse("leader<table><TR><Td>body</tD></TR></table>trailer");
			Assert.AreEqual(null, p.body);
			Assert.AreEqual(null, p.parts.body);
			Assert.AreEqual("body", p.parts.parts.body);
		}

		[Test]
		public void testIterating() 
		{
			Parse p = new Parse("leader<table><tr><td>one</td><td>two</td><td>three</td></tr></table>trailer");
			Assert.AreEqual("one", p.parts.parts.body);
			Assert.AreEqual("two", p.parts.parts.more.body);
			Assert.AreEqual("three", p.parts.parts.more.more.body);
		}

		[Test]
		public void testIndexing() 
		{
			Parse p = new Parse("leader<table><tr><td>one</td><td>two</td><td>three</td></tr><tr><td>four</td></tr></table>trailer");
			Assert.AreEqual("one", p.at(0,0,0).body);
			Assert.AreEqual("two", p.at(0,0,1).body);
			Assert.AreEqual("three", p.at(0,0,2).body);
			Assert.AreEqual("three", p.at(0,0,3).body);
			Assert.AreEqual("three", p.at(0,0,4).body);
			Assert.AreEqual("four", p.at(0,1,0).body);
			Assert.AreEqual("four", p.at(0,1,1).body);
			Assert.AreEqual("four", p.at(0,2,0).body);
			Assert.AreEqual(1, p.size());
			Assert.AreEqual(2, p.parts.size());
			Assert.AreEqual(3, p.parts.parts.size());
			Assert.AreEqual("one", p.leaf().body);
			Assert.AreEqual("four", p.parts.last().leaf().body);
		}
    
		public void TestTypeAdapter() 
		{
			TypeAdapterTarget target = new TypeAdapterTarget();
			TypeAdapter adapter = TypeAdapter.on(target, typeof(int));
			object parsedInt = adapter.parse("123");
			Assert.AreEqual(123, (int)parsedInt, "int");
			Assert.IsTrue(adapter.equals(parsedInt, 123), "int should be equal");

			adapter = TypeAdapter.on(target, typeof(float));
			Assert.AreEqual(12.3, (float)adapter.parse("12.3"), .00001, "float");

			adapter = TypeAdapter.on(target, typeof(long));
			Assert.AreEqual(123L, (long)adapter.parse("123"), "long");

			adapter = TypeAdapter.on(target, typeof(string));
			Assert.AreEqual("123", (string)adapter.parse("123"), "string");

			adapter = TypeAdapter.on(target, typeof(string[]));
			AssertArraysEqual(new object[] {"1", "2", "3"}, (object[])adapter.parse("1,2,3"), "string[]");

			adapter = TypeAdapter.on(target, typeof(int[]));
			AssertArraysEqual(new object[] {1, 2, 3}, (object[])adapter.parse("1,2,3"), "int[]");

			MethodInfo twoPi = typeof(TypeAdapterTarget).GetMethod("twoPi");
			adapter = TypeAdapter.on(target, twoPi);
			Assert.AreEqual(2 * Math.PI, adapter.get(), "twoPi");

			FieldInfo pi = target.GetType().GetField("pi");
			adapter = TypeAdapter.on(target, pi);
			adapter.set(3);
			Assert.AreEqual(3, target.pi, .00001, "pi");
			Assert.AreEqual(3, (double)adapter.get(), .00001, "adapted pi");
		}

		private void AssertArraysEqual(object[] expected, object[] actual, string message) 
		{
			Assert.AreEqual(Flatten(expected), Flatten(actual), message);
		}

		private string Flatten(object[] array) 
		{
			string result = "";
			string comma = "";
			foreach (object o in array) 
			{
				result += comma + o;
				comma = ",";
			}
			return result;
		}

		private class TypeAdapterTarget : Fixture 
		{
			public double pi = Math.PI;

			public double twoPi() 
			{
				return 2 * pi;
			}
		}
    
		public void testTypeAdapter () 
		{
			TestFixture f = new TestFixture ();
			TypeAdapter a = TypeAdapter.on(f, f.GetType().GetField("sampleInt"));
			a.set(a.parse("123456"));
			Assert.AreEqual(123456, f.sampleInt);
			Assert.AreEqual("-234567", a.parse("-234567").ToString());
			a = TypeAdapter.on(f, f.GetType().GetMethod("pi", new Type[] {}));
			Assert.AreEqual(3.14159, (double)a.get(), 0.00001);
			a = TypeAdapter.on(f, f.GetType().GetField("ch"));
			a.set(a.parse("a"));
			Assert.AreEqual('a', f.ch);
			a = TypeAdapter.on(f, f.GetType().GetField("name"));
			a.set(a.parse("xyzzy"));
			Assert.AreEqual("xyzzy", f.name);
			a = TypeAdapter.on(f, f.GetType().GetField("sampleFloat"));
			a.set(a.parse("6.02e23"));
			Assert.AreEqual(6.02e23, f.sampleFloat, 1e17);
			//      a = TypeAdapter.on(f, f.GetType().GetField("sampleArray"));
			//      a.set(a.parse("1,2,3"));
			//      Assert.AreEqual(1, f.sampleArray[0]);
			//      Assert.AreEqual(2, f.sampleArray[1]);
			//      Assert.AreEqual(3, f.sampleArray[2]);
			//      Assert.AreEqual("1,2,3", f.sampleArray.ToString());
			//      Assert.AreEqual(new int[] {1, 2, 3}, f.sampleArray);
			//      a = TypeAdapter.on(f, f.GetType().GetField("sampleDate"));
			//      DateTime date = new DateTime(1949,4,26);
			//      a.set(a.parse(DateFormat.getDateInstance().format(date)));
			//      Assert.AreEqual(date, f.sampleDate);
			a = TypeAdapter.on(f, f.GetType().GetField("sampleByte"));
			a.set(a.parse("123"));
			Assert.AreEqual((byte)123, f.sampleByte);
			a = TypeAdapter.on(f, f.GetType().GetField("sampleShort"));
			a.set(a.parse("12345"));
			Assert.AreEqual((short)12345, f.sampleShort);
		}

		class TestFixture : ColumnFixture 
		{
			// used in testTypeAdapter
			public byte sampleByte = 0;
			public short sampleShort =0;
			public int sampleInt = 0;
			public float sampleFloat = 0;
			public double pi () {return 3.14159862;}
			public char ch = '\0';
			public string name = null;
			public int[] sampleArray = null;
			public DateTime sampleDate = DateTime.Now;
		}
        
		public void testScientificDouble() 
		{
			double pi = 3.141592865;
			Assert.AreEqual(ScientificDouble.valueOf("3.14"), pi);
			Assert.AreEqual(ScientificDouble.valueOf("3.142"), pi);
			Assert.AreEqual(ScientificDouble.valueOf("3.1416"), pi);
			Assert.AreEqual(ScientificDouble.valueOf("3.14159"), pi);
			Assert.AreEqual(ScientificDouble.valueOf("3.141592865"), pi);
			Assert.IsTrue(!ScientificDouble.valueOf("3.140").Equals(pi));
			Assert.IsTrue(!ScientificDouble.valueOf("3.144").Equals(pi));
			Assert.IsTrue(!ScientificDouble.valueOf("3.1414").Equals(pi));
			Assert.IsTrue(!ScientificDouble.valueOf("3.141592863").Equals(pi));
			Assert.AreEqual(ScientificDouble.valueOf("6.02e23"), 6.02e23d);
			Assert.AreEqual(ScientificDouble.valueOf("6.02E23"), 6.024E23d);
			Assert.AreEqual(ScientificDouble.valueOf("6.02e23"), 6.016e23d);
			Assert.IsTrue(!ScientificDouble.valueOf("6.02e23").Equals(6.026e23d));
			Assert.IsTrue(!ScientificDouble.valueOf("6.02e23").Equals(6.014e23d));
			Assert.AreEqual(ScientificDouble.valueOf("3.14"), ScientificDouble.valueOf("3.14"));
		}

		public void testEscape() 
		{
			String junk = "!@#$%^*()_-+={}|[]\\:\";',./?`";
			Assert.AreEqual(junk, Fixture.escape(junk));
			Assert.AreEqual("", Fixture.escape(""));
			Assert.AreEqual("&lt;", Fixture.escape("<"));
			Assert.AreEqual("&lt;&lt;", Fixture.escape("<<"));
			Assert.AreEqual("x&lt;", Fixture.escape("x<"));
			Assert.AreEqual("&amp;", Fixture.escape("&"));
			Assert.AreEqual("&lt;&amp;&lt;", Fixture.escape("<&<"));
			Assert.AreEqual("&amp;&lt;&amp;", Fixture.escape("&<&"));
			Assert.AreEqual("a &lt; b &amp;&amp; c &lt; d", Fixture.escape("a < b && c < d"));
		}

		public void testRuns() 
		{
			run("arithmetic.html", 37, 10, 0, 2);
			run("CalculatorExample.html", 75, 9, 0, 0);
			//            run("MusicExample.html", 95, 0, 0, 0);
		}

		protected void run(string file, int right, int wrong, int ignores, int exceptions) 
		{
			string root = findRoot(file);
			string input = readExample(root + file);
			Fixture fixture = new Fixture();
			Fixture.assemblyDirs = new string[] {".", @"..\..\..\eg\bin\Debug"};
			Parse tables;
			if (input.IndexOf("<wiki>") >= 0) 
			{
				tables = new Parse(input, new String[]{"wiki", "table", "tr", "td"});
				fixture.doTables(tables.parts);
			} 
			else 
			{
				tables = new Parse(input, new String[]{"table", "tr", "td"});
				fixture.doTables(tables);
			}
			StringWriter output = new StringWriter();
			tables.print(output);
			output.Close();

			Assert.AreEqual(right, fixture.counts.right, file+" right");
			Assert.AreEqual(wrong, fixture.counts.wrong, file+" wrong");
			Assert.AreEqual(ignores, fixture.counts.ignores, file+" ignores");
			Assert.AreEqual(exceptions, fixture.counts.exceptions, file+" exceptions");
		}

		protected String findRoot(string file) 
		{
			string root = "../../../../examples/";
			if (!File.Exists(root + file)) root = "../../../../../../examples/";
			return root;
		}

		protected String readExample(string path) 
		{
			StreamReader input = new StreamReader(path);
			string result = input.ReadToEnd();
			input.Close();
			return result;
		}
	}
}