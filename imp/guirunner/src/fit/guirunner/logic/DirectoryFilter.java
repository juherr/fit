package fit.guirunner.logic;

import java.io.File;
import java.io.FilenameFilter;

public class DirectoryFilter implements FilenameFilter {
  public boolean accept(File dir, String name) {
    return new File(dir, name).isDirectory();
  }
}
