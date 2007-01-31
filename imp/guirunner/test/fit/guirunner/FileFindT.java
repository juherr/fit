package fit.guirunner;

import java.io.File;
import java.util.Iterator;
import java.util.List;

import fit.guirunner.logic.FileFind;

// no junit test
public class FileFindT {
  public static void main(String[] args) {
    String startPath = ".";
    String pattern = "*.html";

    if (args.length == 2) {
      startPath = args[0];
      pattern = args[1];
    } else if (args.length == 1) {
      pattern = args[0];
    }
    FileFind ff = new FileFind(pattern);
    List files = ff.execute(startPath);
    System.out.println("FileFind starting in " + startPath + " searching for " + pattern + ":");
    for (Iterator i = files.iterator(); i.hasNext();) {
      System.out.println(((File)i.next()).getAbsolutePath());
    }
  }

}
