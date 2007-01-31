package fit.guirunner.logic;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * Taken from the allFiles example
 */
public class FileFind {

  FilenameFilter filter;

  public FileFind(String pattern) {
    filter = new SimpleWidcardFilter(pattern);
  }

  public FileFind(FilenameFilter filter) {
    this.filter = filter;
  }

  public List execute(String startDir) {
    return execute(new File(startDir));
  }

  public List execute(File startDir) {
    List result = new LinkedList();
    if (startDir.isDirectory()) {
      try {
        expand(startDir.getCanonicalFile(), result);
      } catch (IOException x) {
        // should not occur, since isDirectory() check
      }
    }
    return result;
  }

  protected void expand(File path, List result) {
    File files[] = path.listFiles();
    if (files != null) {
      for (int i = 0; i < files.length; i++) {
        if (filter.accept(path, files[i].getName())) {
          try {
            result.add(files[i].getCanonicalFile());
          } catch (IOException e) {
            // TODO CommonLogging
            e.printStackTrace(System.err);
          }
        }
        if (files[i].isDirectory()) {
          expand(files[i], result);
        }
      }
    }
  }

  static class SimpleWidcardFilter implements FilenameFilter {
    String prefix;

    String sufix;

    int minimum;

    SimpleWidcardFilter(String pattern) {
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
      return !(name.startsWith(".")) && name.length() >= minimum && name.startsWith(prefix)
          && (sufix == null ? name.length() == minimum : name.endsWith(sufix));
    }
  }
}
