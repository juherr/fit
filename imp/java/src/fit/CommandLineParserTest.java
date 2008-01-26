package fit;

//Copyright (c) 2008 Cunningham & Cunningham, Inc.
//Released under the terms of the GNU General Public License version 2 or later.
//Contributed by Martin Busik

import junit.framework.TestCase;

public class CommandLineParserTest extends TestCase {

	public CommandLineParserTest(String name){
		super(name);
	}
	public void testQuotes() {
		assertEquals("abc", CommandLineParser.removeQuotes("abc"));
		assertEquals("", CommandLineParser.removeQuotes(""));
		assertEquals("", CommandLineParser.removeQuotes("''"));
		assertEquals("'", CommandLineParser.removeQuotes("'"));
		assertEquals("\"", CommandLineParser.removeQuotes("\""));
		assertEquals("", CommandLineParser.removeQuotes("\"\""));
		assertEquals("abc", CommandLineParser.removeQuotes("\"abc\""));
		assertEquals("\"'", CommandLineParser.removeQuotes("\"'"));
	}
	public void testName() {
		assertEquals("abc",CommandLineParser.extractName("--abc"));
		assertEquals("abc",CommandLineParser.extractName("--abc="));
	}
	public void testValue() {
		assertEquals("abc",CommandLineParser.extractValue("--abc=abc"));
		assertEquals("abc",CommandLineParser.extractValue("=abc"));
		assertEquals("abc",CommandLineParser.extractValue("='abc'"));
		assertEquals("'",CommandLineParser.extractValue("='"));
	}
	public void testParser1() throws CommandLineParserException {
		String[] args = new String[] { "abc", "def" };
		CommandLineParser p = CommandLineParser.fileRunnerParser();
		p.parse(args);
		assertEquals(2, p.getArguments().length);
		assertEquals("def",p.getArguments()[1]);
		assertFalse(p.isParameterSet(CommandLineParser.PARAMETER_ENCODING));
	}
	
//BUG--not yet fixed
//	public void testMixOfArgumentsAndParameter_WhenParameterIsAtEnd() throws CommandLineParserException {
//		CommandLineParser p = CommandLineParser.fileRunnerParser();
//		p.parse(new String[] { "abc", "def", "--encoding=xyz" });
//		assertEquals("xyz", p.getStringParameterValue(CommandLineParser.PARAMETER_ENCODING));
//		assertEquals(2, p.getArguments().length);
//	}		
	
	public void testParser2() throws CommandLineParserException {
		String[] args = new String[] { "--", "--", "def" };
		CommandLineParser p = CommandLineParser.fileRunnerParser();
		p.parse(args);
		assertEquals(2, p.getArguments().length);
		assertFalse(p.isParameterSet(CommandLineParser.PARAMETER_ENCODING));
	}
	public void testParser3() throws CommandLineParserException {
		String[] args = new String[] { "--encoding=xyz", "--", "--", "def" };
		CommandLineParser p = CommandLineParser.fileRunnerParser();
		p.parse(args);
		assertEquals(2, p.getArguments().length);
		assertTrue(p.isParameterSet(CommandLineParser.PARAMETER_ENCODING));
		assertEquals("xyz",p.getStringParameterValue(CommandLineParser.PARAMETER_ENCODING));
	}
	public void testParser4() throws CommandLineParserException {
		String[] args = new String[] { "--encoding=xyz", "--encoding=def" };
		CommandLineParser p = CommandLineParser.fileRunnerParser();
		try {
			p.parse(args);
			fail();
		} catch(CommandLineParserException e) {
			assertEquals("Duplicate parameter encoding found!", e.getMessage());
		}
	}
}
