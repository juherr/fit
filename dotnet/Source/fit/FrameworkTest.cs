// Copyright (c) 2002, 2003 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

using System;
using NUnit.Framework;
using System.Reflection;
using System.IO;

namespace fit {
    public class FrameworkTest : TestCase {
        public FrameworkTest(string name) : base(name) {
        }

        public void TestParsing() {
            Parse p = new Parse("leader<Table foo=2>body</table>trailer", new string[] {"table"});
            AssertEquals("leader", p.leader);
            AssertEquals("<Table foo=2>", p.tag);
            AssertEquals("body", p.body);
            AssertEquals("trailer", p.trailer);
        }

        public void testRecursing() {
            Parse p = new Parse("leader<table><TR><Td>body</tD></TR></table>trailer");
            AssertEquals(null, p.body);
            AssertEquals(null, p.parts.body);
            AssertEquals("body", p.parts.parts.body);
        }

        public void testIterating() {
            Parse p = new Parse("leader<table><tr><td>one</td><td>two</td><td>three</td></tr></table>trailer");
            AssertEquals("one", p.parts.parts.body);
            AssertEquals("two", p.parts.parts.more.body);
            AssertEquals("three", p.parts.parts.more.more.body);
        }

        public void testIndexing() {
            Parse p = new Parse("leader<table><tr><td>one</td><td>two</td><td>three</td></tr><tr><td>four</td></tr></table>trailer");
            AssertEquals("one", p.at(0,0,0).body);
            AssertEquals("two", p.at(0,0,1).body);
            AssertEquals("three", p.at(0,0,2).body);
            AssertEquals("three", p.at(0,0,3).body);
            AssertEquals("three", p.at(0,0,4).body);
            AssertEquals("four", p.at(0,1,0).body);
            AssertEquals("four", p.at(0,1,1).body);
            AssertEquals("four", p.at(0,2,0).body);
            AssertEquals(1, p.size());
            AssertEquals(2, p.parts.size());
            AssertEquals(3, p.parts.parts.size());
            AssertEquals("one", p.leaf().body);
            AssertEquals("four", p.parts.last().leaf().body);
        }

        public void testParseException() {
            try {
                Parse p = new Parse("leader<table><tr><th>one</th><th>two</th><th>three</th></tr><tr><td>four</td></tr></table>trailer");
            } 
            catch (ApplicationException e) {
                //				AssertEquals(17, e.getErrorOffset());
                AssertEquals("Can't find tag: td", e.Message);
                return;
            }
            Fail("exptected exception not thrown");
        }

        public void testText() {
            string[] tags ={"td"};
            Parse p = new Parse("<td>a&lt;b</td>", tags);
            AssertEquals("a&lt;b", p.body);
            AssertEquals("a<b", p.text());
            p = new Parse("<td>\ta&gt;b&nbsp;&amp;&nbsp;b>c &&&nbsp;</td>", tags);
            AssertEquals("a>b & b>c &&", p.text());
            p = new Parse("<td>\ta&gt;b&nbsp;&amp;&nbsp;b>c &&nbsp;</td>", tags);
            AssertEquals("a>b & b>c &", p.text());
            p = new Parse("<TD><P><FONT FACE=\"Arial\" SIZE=2>GroupTestFixture</FONT></TD>", tags);
            AssertEquals("GroupTestFixture",p.text());
        }

        public void testUnescape () {
            AssertEquals("a<b", Parse.unescape("a&lt;b"));
            AssertEquals("a>b & b>c &&", Parse.unescape("a&gt;b&nbsp;&amp;&nbsp;b>c &&"));
            AssertEquals("&amp;&amp;", Parse.unescape("&amp;amp;&amp;amp;"));
            AssertEquals("a>b & b>c &&", Parse.unescape("a&gt;b&nbsp;&amp;&nbsp;b>c &&"));
        }

        public void testUnformat () {
            AssertEquals("ab",Parse.unformat("<font size=+1>a</font>b"));
            AssertEquals("ab",Parse.unformat("a<font size=+1>b</font>"));
            AssertEquals("a<b",Parse.unformat("a<b"));
        }

        public void TestTypeAdapter() {
            TypeAdapterTarget target = new TypeAdapterTarget();
            TypeAdapter adapter = TypeAdapter.on(target, typeof(int));
            object parsedInt = adapter.parse("123");
            AssertEquals("int", 123, (int)parsedInt);
            Assert("int should be equal", adapter.equals(parsedInt, 123));

            adapter = TypeAdapter.on(target, typeof(float));
            AssertEquals("float", 12.3, (float)adapter.parse("12.3"), .00001);

            adapter = TypeAdapter.on(target, typeof(long));
            AssertEquals("long", 123L, (long)adapter.parse("123"));

            adapter = TypeAdapter.on(target, typeof(string));
            AssertEquals("string", "123", (string)adapter.parse("123"));

            adapter = TypeAdapter.on(target, typeof(string[]));
            AssertArraysEqual("string[]", new object[] {"1", "2", "3"}, (object[])adapter.parse("1,2,3"));

            adapter = TypeAdapter.on(target, typeof(int[]));
            AssertArraysEqual("int[]", new object[] {1, 2, 3}, (object[])adapter.parse("1,2,3"));

            MethodInfo twoPi = typeof(TypeAdapterTarget).GetMethod("twoPi");
            adapter = TypeAdapter.on(target, twoPi);
            AssertEquals("twoPi", 2 * Math.PI, adapter.get());

            FieldInfo pi = target.GetType().GetField("pi");
            adapter = TypeAdapter.on(target, pi);
            adapter.set(3);
            AssertEquals("pi", 3, target.pi, .00001);
            AssertEquals("adapted pi", 3, (double)adapter.get(), .00001);
        }

        private void AssertArraysEqual(string message, object[] expected, object[] actual) {
            AssertEquals(message, Flatten(expected), Flatten(actual));
        }

        private string Flatten(object[] array) {
            string result = "";
            string comma = "";
            foreach (object o in array) {
                result += comma + o;
                comma = ",";
            }
            return result;
        }

        private class TypeAdapterTarget : Fixture {
            public double pi = Math.PI;

            public double twoPi() {
                return 2 * pi;
            }
        }
		
        public void testTypeAdapter () {
            TestFixture f = new TestFixture ();
            TypeAdapter a = TypeAdapter.on(f, f.GetType().GetField("sampleInt"));
            a.set(a.parse("123456"));
            AssertEquals(123456, f.sampleInt);
            AssertEquals("-234567", a.parse("-234567").ToString());
            a = TypeAdapter.on(f, f.GetType().GetMethod("pi", new Type[] {}));
            AssertEquals(3.14159, (double)a.get(), 0.00001);
            a = TypeAdapter.on(f, f.GetType().GetField("ch"));
            a.set(a.parse("a"));
            AssertEquals('a', f.ch);
            a = TypeAdapter.on(f, f.GetType().GetField("name"));
            a.set(a.parse("xyzzy"));
            AssertEquals("xyzzy", f.name);
            a = TypeAdapter.on(f, f.GetType().GetField("sampleFloat"));
            a.set(a.parse("6.02e23"));
            AssertEquals(6.02e23, f.sampleFloat, 1e17);
            //			a = TypeAdapter.on(f, f.GetType().GetField("sampleArray"));
            //			a.set(a.parse("1,2,3"));
            //			AssertEquals(1, f.sampleArray[0]);
            //			AssertEquals(2, f.sampleArray[1]);
            //			AssertEquals(3, f.sampleArray[2]);
            //			AssertEquals("1,2,3", f.sampleArray.ToString());
            //			AssertEquals(new int[] {1, 2, 3}, f.sampleArray);
            //			a = TypeAdapter.on(f, f.GetType().GetField("sampleDate"));
            //			DateTime date = new DateTime(1949,4,26);
            //			a.set(a.parse(DateFormat.getDateInstance().format(date)));
            //			AssertEquals(date, f.sampleDate);
            a = TypeAdapter.on(f, f.GetType().GetField("sampleByte"));
            a.set(a.parse("123"));
            AssertEquals((byte)123, f.sampleByte);
            a = TypeAdapter.on(f, f.GetType().GetField("sampleShort"));
            a.set(a.parse("12345"));
            AssertEquals((short)12345, f.sampleShort);
        }

        class TestFixture : ColumnFixture {
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
        
        public void testScientificDouble() {
            double pi = 3.141592865;
            AssertEquals(ScientificDouble.valueOf("3.14"), pi);
            AssertEquals(ScientificDouble.valueOf("3.142"), pi);
            AssertEquals(ScientificDouble.valueOf("3.1416"), pi);
            AssertEquals(ScientificDouble.valueOf("3.14159"), pi);
            AssertEquals(ScientificDouble.valueOf("3.141592865"), pi);
            Assert(!ScientificDouble.valueOf("3.140").Equals(pi));
            Assert(!ScientificDouble.valueOf("3.144").Equals(pi));
            Assert(!ScientificDouble.valueOf("3.1414").Equals(pi));
            Assert(!ScientificDouble.valueOf("3.141592863").Equals(pi));
            AssertEquals(ScientificDouble.valueOf("6.02e23"), 6.02e23d);
            AssertEquals(ScientificDouble.valueOf("6.02E23"), 6.024E23d);
            AssertEquals(ScientificDouble.valueOf("6.02e23"), 6.016e23d);
            Assert(!ScientificDouble.valueOf("6.02e23").Equals(6.026e23d));
            Assert(!ScientificDouble.valueOf("6.02e23").Equals(6.014e23d));
            AssertEquals(ScientificDouble.valueOf("3.14"), ScientificDouble.valueOf("3.14"));
        }

        public void testEscape() {
            String junk = "!@#$%^*()_-+={}|[]\\:\";',./?`";
            AssertEquals(junk, Fixture.escape(junk));
            AssertEquals("", Fixture.escape(""));
            AssertEquals("&lt;", Fixture.escape("<"));
            AssertEquals("&lt;&lt;", Fixture.escape("<<"));
            AssertEquals("x&lt;", Fixture.escape("x<"));
            AssertEquals("&amp;", Fixture.escape("&"));
            AssertEquals("&lt;&amp;&lt;", Fixture.escape("<&<"));
            AssertEquals("&amp;&lt;&amp;", Fixture.escape("&<&"));
            AssertEquals("a &lt; b &amp;&amp; c &lt; d", Fixture.escape("a < b && c < d"));
        }

        public void testRuns() {
            run("arithmetic", 37, 10, 0, 2);
            run("CalculatorExample", 75, 9, 0, 0);
            run("MusicExample", 95, 0, 0, 0);
        }

        protected void run(string file, int right, int wrong, int ignores, int exceptions) {
            string input = read("../Documents/"+file+".html");
            Fixture fixture = new Fixture();
            Parse tables;
            if (input.IndexOf("<wiki>") >= 0) {
                tables = new Parse(input, new String[]{"wiki", "table", "tr", "td"});
                fixture.doTables(tables.parts);
            } else {
                tables = new Parse(input, new String[]{"table", "tr", "td"});
                fixture.doTables(tables);
            }
            StringWriter output = new StringWriter();
            tables.print(output);
            output.Close();

            AssertEquals(file+" right", right, fixture.counts.right);
            AssertEquals(file+" wrong", wrong, fixture.counts.wrong);
            AssertEquals(file+" ignores", ignores, fixture.counts.ignores);
            AssertEquals(file+" exceptions", exceptions, fixture.counts.exceptions);
        }

        protected String read(string filename) {
            StreamReader input = new StreamReader(filename);
            string result = input.ReadToEnd();
            input.Close();
            return result;
        }
    }
}