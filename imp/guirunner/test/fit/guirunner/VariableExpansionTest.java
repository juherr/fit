package fit.guirunner;

import java.util.HashMap;
import java.util.Map;

import junit.framework.TestCase;
import fit.guirunner.logic.VariableExpansion;

public class VariableExpansionTest extends TestCase {
  public void test1() {
    Map m = new HashMap();
    m.put("indir", "c:/pros/temp \\5");
    m.put("in", "aaa bbb $1 $2 $3");
    VariableExpansion sr = new VariableExpansion(m);

    assertEquals("Es war einmal", sr.replace("Es war einmal"));
    assertEquals("Es war aaa bbb $1 $2 $3 einmal", sr.replace("Es war ${in} einmal"));
    assertEquals("Es war }in einmal", sr.replace("Es war }in einmal"));
    assertEquals("Es war ${bla} einmal c:/pros/temp \\5", sr
        .replace("Es war ${bla} einmal ${indir}"));
  }

}
