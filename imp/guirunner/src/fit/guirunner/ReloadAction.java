package fit.guirunner;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
import java.util.List;

import fit.guirunner.logic.FileFind;
import fit.guirunner.util.ListDiff;
import fit.guirunner.util.ListToList;
import fit.guirunner.util.MapFunction;

public class ReloadAction extends AbstractAsyncAction {

  RunnerTableModel model;

  Resources resources;

  public ReloadAction(RunnerTableModel model, Resources resources) {
    this.model = model;
    this.resources = resources;
    setLockCoordinator(resources.getLockCoordinator());
  }

  public void doActionPerformed(ActionEvent e) {
    getLockCoordinator().setReadingFilesystem(true);
    Configuration config = resources.getConfiguration();
    FileFind fileFind = new FileFind(config.getPattern());
    try {
      EnvironmentContext ctx = new EnvironmentContext(config);
      MapFunction mapper = fileToRunnerEntry(ctx);
      List files = fileFind.execute(ctx.getInDir());
      List latest = ListToList.ListMapList(mapper, files);
      List previous = model.getEntries();
      ListDiff diff = new ListDiff(previous, latest);
      if (diff.hasDiffs()) {
        model.addEntries(diff.getRightOnly());
        model.removeEntries(diff.getLeftOnly());
      }
    } catch (IOException e1) {
      // TODO Gui-Message
      e1.printStackTrace();
    } finally {
      getLockCoordinator().setReadingFilesystem(false);
    }
  }

  public static String generateRelativePath(File inDir, File inFile) {
	  String hlp = inFile.getParentFile().getAbsolutePath();
	  int dirnameLen = inDir.getAbsolutePath().length();
	  int startIdx = (dirnameLen == hlp.length()) ? dirnameLen : dirnameLen + 1; // +1 -> remove path separator, if subdirectory
	  return hlp.substring(startIdx);
  }
  /**
   * creates a MapFunction which converts a File-Object into a RunnerEntry object according to a
   * given Configuration
   * 
   * @param config
   * @return
   * @throws IOException
   */
  public static MapFunction fileToRunnerEntry(EnvironmentContext ctx) throws IOException {
    final File inDir = ctx.getInDir();
    return new MapFunction() {
      public Object f(Object current) {
        File inFile = (File)current;
        String relativePath = generateRelativePath(inDir,inFile);
        return new RunnerEntry(inFile, relativePath);
      }
    };
  }

  protected boolean isActionEnabled() {
    return getLockCoordinator().canReadFilesystem();
  }
}
