package fit.guirunner.logic;

import java.util.HashMap;
import java.util.Map;

/**
 * Expansion of variable references (properties-style) withot using the regexp engine (quoting of $
 * backslashes etc.)
 * 
 * @author Martin Busik
 */
public class VariableExpansion {
  private Map map;

  private VariableExpansion() {
    this.map = new HashMap();
  }

  public VariableExpansion(Map map) {
    this.map = map;
  }

  public VariableExpansion(String key1, String value1) {
    this();
    map.put(key1, value1);
  }

  public VariableExpansion(String key1, String value1, String key2, String value2) {
    this(key1, value1);
    map.put(key2, value2);
  }

  public VariableExpansion(String key1, String value1, String key2, String value2, String key3,
      String value3) {
    this(key1, value1, key2, value2);
    map.put(key3, value3);
  }
  public VariableExpansion(String key1, String value1, String key2, String value2, String key3,
	      String value3, String key4, String value4) {
	  this(key1,value1,key2,value2,key3,value3);
	  map.put(key4,value4);
  }

  public String replace(String in) {
    StringBuffer result = new StringBuffer();
    int lastIdx = 0;
    int nextIdx;
    while (true) {
      nextIdx = in.indexOf("${", lastIdx);
      if (nextIdx < 0) {
        break;
      }
      int endIdx = in.indexOf('}', nextIdx);
      if (endIdx < 0) {
        break;
      }
      result.append(in.substring(lastIdx, nextIdx));
      String varname = in.substring(nextIdx + 2, endIdx);
      if (map.containsKey(varname)) {
        result.append(map.get(varname));
      } else {
        result.append(in.substring(nextIdx, endIdx + 1));
      }
      lastIdx = endIdx + 1;
    }
    result.append(in.substring(lastIdx));
    return result.toString();
  }

}
