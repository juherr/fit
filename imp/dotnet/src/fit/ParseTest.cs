//Copyright (c) 2002 Cunningham & Cunningham, Inc.
//Released under the terms of the GNU General Public License version 2 or later.

using System;
using NUnit.Framework;
using fit;

[TestFixture]
public class ParseTest {

	[Test]	
	public void testParsing () {
		Parse p = new Parse("leader<Table foo=2>body</table>trailer", new string[] {"table"});
		Assert.AreEqual("leader", p.leader);
		Assert.AreEqual("<Table foo=2>", p.tag);
		Assert.AreEqual("body", p.body);
		Assert.AreEqual("trailer", p.trailer);
	}
	    
	[Test]
	public void testRecursing () {
		Parse p = new Parse("leader<table><TR><Td>body</tD></TR></table>trailer");
		Assert.AreEqual(null, p.body);
		Assert.AreEqual(null, p.parts.body);
		Assert.AreEqual("body", p.parts.parts.body);
	}
    
	[Test]
	public void testIterating () {
		Parse p = new Parse("leader<table><tr><td>one</td><td>two</td><td>three</td></tr></table>trailer");
		Assert.AreEqual("one", p.parts.parts.body);
		Assert.AreEqual("two", p.parts.parts.more.body);
		Assert.AreEqual("three", p.parts.parts.more.more.body);
	}
    
	[Test]
	public void testIndexing () {
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
	
	[Test]
	public void testParseException() 
	{
		try 
		{
			Parse p = new Parse("leader<table><tr><th>one</th><th>two</th><th>three</th></tr><tr><td>four</td></tr></table>trailer");
		} 
		catch (ApplicationException e) 
		{
			//				AssertEquals(17, e.getErrorOffset());
			Assert.AreEqual("Can't find tag: td", e.Message);
			return;
		}
		Assert.Fail("exptected exception not thrown");
	}

	[Test]
	public void testText () {
		string[] tags ={"td"};
		Parse p = new Parse("<td>a&lt;b</td>", tags);
		Assert.AreEqual("a&lt;b", p.body);
		Assert.AreEqual("a<b", p.text());
		p = new Parse("<td>\ta&gt;b&nbsp;&amp;&nbsp;b>c &&&lt;</td>", tags);
		Assert.AreEqual("a>b & b>c &&<", p.text());
		p = new Parse("<td>\ta&gt;b&nbsp;&amp;&nbsp;b>c &&lt;</td>", tags);
		Assert.AreEqual("a>b & b>c &<", p.text());
		p = new Parse("<TD><P><FONT FACE=\"Arial\" SIZE=2>GroupTestFixture</FONT></TD>", tags);
		Assert.AreEqual("GroupTestFixture",p.text());
		
		Assert.AreEqual("", Parse.htmlToText("&nbsp;"));
		Assert.AreEqual("a b", Parse.htmlToText("a <tag /> b"));
		Assert.AreEqual("a", Parse.htmlToText("a &nbsp;"));
		Assert.AreEqual("a", Parse.htmlToText("\u00a0 a \u00a0"));
		Assert.AreEqual("&nbsp;", Parse.htmlToText("&amp;nbsp;"));
		Assert.AreEqual("1     2", Parse.htmlToText("1 &nbsp; &nbsp; 2"));
		Assert.AreEqual("1     2", Parse.htmlToText("1 \u00a0\u00a0\u00a0\u00a02"));
		Assert.AreEqual("a", Parse.htmlToText("  <tag />a"));
		Assert.AreEqual("a\nb", Parse.htmlToText("a<br />b"));

		Assert.AreEqual("ab", Parse.htmlToText("<font size=+1>a</font>b"));
		Assert.AreEqual("ab", Parse.htmlToText("a<font size=+1>b</font>"));
		Assert.AreEqual("a<b", Parse.htmlToText("a<b"));

		Assert.AreEqual("a\nb\nc\nd", Parse.htmlToText("a<br>b<br/>c<  br   /   >d"));
		Assert.AreEqual("a\nb", Parse.htmlToText("a</p><p>b"));
		Assert.AreEqual("a\nb", Parse.htmlToText("a< / p >   <   p  >b"));
	}

	[Test]
	public void testUnescape () {
		Assert.AreEqual("a<b", Parse.unescape("a&lt;b"));
		Assert.AreEqual("a>b & b>c &&", Parse.unescape("a&gt;b&nbsp;&amp;&nbsp;b>c &&"));
		Assert.AreEqual("&amp;&amp;", Parse.unescape("&amp;amp;&amp;amp;"));
		Assert.AreEqual("a>b & b>c &&", Parse.unescape("a&gt;b&nbsp;&amp;&nbsp;b>c &&"));
		Assert.AreEqual("\"\"'", Parse.unescape("“”’"));
	}

	[Test]
	public void testWhitespaceIsCondensed() {
		Assert.AreEqual("a b", Parse.condenseWhitespace(" a  b  "));
		Assert.AreEqual("a b", Parse.condenseWhitespace(" a  \n\tb  "));
		Assert.AreEqual("", Parse.condenseWhitespace(" "));
		Assert.AreEqual("", Parse.condenseWhitespace("  "));
		Assert.AreEqual("", Parse.condenseWhitespace("   "));
		Assert.AreEqual("", Parse.condenseWhitespace(new string(new char[]{(char) 160})));
	}
}
