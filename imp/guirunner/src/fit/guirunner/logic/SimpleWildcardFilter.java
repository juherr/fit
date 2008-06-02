package fit.guirunner.logic;

import java.io.File;
import java.io.FilenameFilter;

public class SimpleWildcardFilter implements FilenameFilter {
  String prefix;

  String sufix;

  int minimum;

  public SimpleWildcardFilter(String pattern) {
    int star = pattern.indexOf("*");
    if (star >= 0) {
      prefix = pattern.substring(0, star);
      sufix = pattern.substring(star + 1);
      minimum = prefix.length() + sufix.length();
    } else {
      prefix = pattern;
      sufix = null;
      minimum = prefix.length();
    }
  }

  public boolean accept(File dir, String name) {
    return !(name.startsWith(".")) // hidden files
        && !(name.startsWith("~")) // MS Word open file backup
        && name.length() >= minimum
        && name.startsWith(prefix)
        && (sufix == null ? name.length() == minimum : name.endsWith(sufix));
  }
}
