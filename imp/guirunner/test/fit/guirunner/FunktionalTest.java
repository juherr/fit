package fit.guirunner;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import fit.guirunner.util.Fold;
import fit.guirunner.util.FoldFunction;
import fit.guirunner.util.GrepFunction;
import fit.guirunner.util.IteratorGrep;
import fit.guirunner.util.IteratorMap;
import fit.guirunner.util.MapFunction;

import junit.framework.TestCase;

public class FunktionalTest extends TestCase {

  List al;

  protected void setUp() throws Exception {
    super.setUp();
    al = new ArrayList();
    al.add("a");
    al.add("b");
    al.add("C");
    al.add("d");
    al.add("E");

  }

  public void testMap() {
    MapFunction mf = new MapFunction() {
      public Object f(Object current) {
        return ((String)current).toUpperCase() + "x";
      }
    };
    Iterator i = new IteratorMap(al.iterator(), mf);
    assertEquals("Ax", i.next());
    assertEquals("Bx", i.next());
  }

  public void testGrep() {
    GrepFunction gf = new GrepFunction() {
      public boolean f(Object current) {
        return Character.isUpperCase(((String)current).charAt(0));
      }
    };
    Iterator i = new IteratorGrep(al.iterator(), gf);
    assertEquals("C", i.next());
    assertEquals(true, i.hasNext());
    assertEquals(true, i.hasNext());
    assertEquals("E", i.next());
    assertEquals(false, i.hasNext());

    i = new IteratorGrep(new ArrayList().iterator(), gf);
    assertEquals(false, i.hasNext());
  }

  public void testGrep2() {
    GrepFunction gf = new GrepFunction() {
      public boolean f(Object current) {
        return true;
      }
    };
    Iterator i = new IteratorGrep(al.iterator(), gf);
    assertEquals("a", i.next());
    assertEquals("b", i.next());
    assertEquals("C", i.next());
    assertEquals(true, i.hasNext());
    assertEquals(true, i.hasNext());
    assertEquals("d", i.next());
    assertEquals("E", i.next());
    assertEquals(false, i.hasNext());
  }

  public void testFold() {
    FoldFunction ff = new FoldFunction() {
      public Object f(Object previous, Object current) {
        return ((StringBuffer)previous).append(current).append(" ");
      }
    };
    StringBuffer result = (StringBuffer)Fold.doFold(ff, new StringBuffer(), al.iterator());
    assertEquals("a b C d E ", result.toString());
  }

  public static void main(String[] args) {
    junit.textui.TestRunner.run(FunktionalTest.class);
  }

}
