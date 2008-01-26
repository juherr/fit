package fit;

//Copyright (c) 2008 Cunningham & Cunningham, Inc.
//Released under the terms of the GNU General Public License version 2 or later.
//Contributed by James Shore with inspiration from Martin Busik

import junit.framework.TestCase;

public class ParametersTest extends TestCase {

	public void testInputAndOutput() throws Exception {
		Parameters p = new Parameters(new String[] {"in", "out"});
		assertEquals("in", p.input());
		assertEquals("out", p.output());
	}

	public void testLegacyArguments_NoteThatThisMethodSucksAndWillGoAwayAsSoonAsPossible() throws Exception {
		Parameters p = new Parameters(new String[] {"--encoding=foo", "in", "out"});
		String[] args = p.legacyArguments();
		assertEquals(2, args.length);
		assertEquals("in", args[0]);
		assertEquals("out", args[1]);
	}

	public void testDefaultEncoding() throws Exception {
		Parameters p = new Parameters(new String[] {"in", "out"});
		assertEquals(System.getProperty("file.encoding"), p.encoding());
		assertEquals("encoding unspecified", false, p.encodingSpecified());
		assertEquals("encoding specified", true, new Parameters(new String[] {"in", "out", "--encoding=foo"}).encodingSpecified());
	}	

	public void testEncodingWorksInAnyPosition() throws Exception {
		assertParseWorked("--encoding=foo", "in", "out");
		assertParseWorked("in", "--encoding=foo", "out");
		assertParseWorked("in", "out", "--encoding=foo");
	}

	private void assertParseWorked(String parm1, String parm2, String parm3) throws CommandLineParseException {
		Parameters p = new Parameters(new String[] {parm1, parm2, parm3});
		assertEquals("in", p.input());
		assertEquals("out", p.output());
		assertEquals("foo", p.encoding());
	}

	public void testParameterErrorHandling() throws Exception {
		assertException("missing input-file and output-file", new String[] {});
		assertException("missing output-file", new String[] {"in"});
		assertException("too many file parameters", new String[] {"in", "out", "quack"});
		assertException("duplicated parameter: --encoding", new String[] {"in", "out", "--encoding=a", "--encoding=b"});
		assertException("unrecognized parameter: --notparm", new String[] {"in", "out", "--encoding=a", "--notparm=b"});
		assertException("unrecognized parameter: --notparm", new String[] {"in", "out", "--notparm=a", "--notparm=b"});
	}

	public void testEncodingParser() throws Exception {
		assertEquals("foo", newParameters("--encoding=foo").encoding());
		assertEquals("foo bar", newParameters("--encoding=foo bar").encoding());
		assertEquals("foo=bar", newParameters("--encoding=foo=bar").encoding());
		assertEquals("foo-bar", newParameters("--encoding=foo-bar").encoding());
	}

	public void testEncodingErrorHandling() throws Exception {
		assertException("unrecognized parameter: -encoding=foo", "-encoding=foo");
		assertException("unrecognized parameter: --notparm", "--notparm=blah");
		assertException("unrecognized parameter: --notparm", "--notparm");
		assertException("unrecognized parameter: -", "-");
		assertException("unrecognized parameter: --", "--");
		assertException("missing value: --encoding", "--encoding");
		assertException("missing value: --encoding", "--encoding=");
	}
	
	public void testHelp() throws Exception {
		try {
			newParameters("--help");
			fail("expected exception");
		}
		catch (CommandLineParseException e) {
			String expected =
				"unrecognized parameter: --help\n" +
				"usage: java fit.FileRunner input-file output-file --encoding=charset\n"
			;
			assertEquals(expected, e.getUserMessage());
		}
	}
	
	private Parameters newParameters(String encoding) throws CommandLineParseException {
		return new Parameters(new String[] {"in", "out", encoding});
	}

	private void assertException(String expectedErrorMessage, String encodingParm) {
		assertException(expectedErrorMessage, new String[] {"in", "out", encodingParm});
	}

	private void assertException(String expectedErrorMessage, String[] args) {
		try {
			Parameters p = new Parameters(args);
			fail("expected exception");
		}
		catch (CommandLineParseException e) {
			assertEquals(expectedErrorMessage, e.getMessage());
		}
	}
}
