// Copyright (c) 2003 Cunningham & Cunningham, Inc.
// Read license.txt in this directory.

package fat;

import junit.framework.TestCase;
import fit.*;

import java.text.ParseException;

public class FatTest extends TestCase {

    public FatTest (String name) {
        super (name);
    }

    public void testCopy () throws ParseException {
        Parse in = new Parse("<table><tr><td>foo</td></tr></table>");
        Parse out = Table.copy(in);
        assertEquals("foo", out.at(0,0,0).body);
        assertTrue("references shouldn't be equal", in != out);
        assertTrue("deep references shouldn't be equal", in.parts != out.parts);
    }

    public void testTable () throws ParseException {
        Parse in = new Parse("<table><tr><td>eg.Arithmetic</td></tr><tr><td>labels</td></tr></table>");
        Fixture fixture = new Table();
        fixture.doTable(in);
        assertTrue("non-null global table", Table.table != null);
    }

    public void testColor() throws ParseException {
        Color fixture = new fat.Color();
        Parse white = new Parse("td", "foo", null, null);
        Parse red = Table.copy(white);
        fixture.wrong(red);
        Parse green = Table.copy(white);
        fixture.right(green);
        Parse gray = Table.copy(white);
        fixture.ignore(gray);
        Parse yellow = Table.copy(white);
        fixture.exception(yellow, new Exception("big trouble"));

        assertEquals("white", fixture.color(white));
        assertEquals("red", fixture.color(red));
        assertEquals("green", fixture.color(green));
        assertEquals("gray", fixture.color(gray));
        assertEquals("yellow", fixture.color(yellow));
    }


}
