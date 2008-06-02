package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import fit.guirunner.Configuration;
import fit.guirunner.EnvironmentContext;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.RunnerTableModel;
import fit.guirunner.logic.FileFind;
import fit.guirunner.util.ListDiff;

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
      List latest = files2entries(ctx, fileFind.execute(ctx.getInDir()));
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

  private List files2entries(EnvironmentContext ctx, List files) {
    List result = new LinkedList();
    for (Iterator i = files.iterator(); i.hasNext();) {
      File inFile = (File)i.next();
      String relativePath = generateRelativePath(ctx.getInDir(), inFile);
      result.add(new RunnerEntry(inFile, relativePath));
    }
    return result;
  }

  public static String generateRelativePath(File inDir, File inFile) {
    String hlp = inFile.getParentFile().getAbsolutePath();
    int dirnameLen = inDir.getAbsolutePath().length();
    int startIdx = (dirnameLen == hlp.length()) ? dirnameLen : dirnameLen + 1; // +1 -> remove path
                                                                                // separator, if
                                                                                // subdirectory
    return hlp.substring(startIdx);
  }

  protected boolean isActionEnabled() {
    return getLockCoordinator().canReadFilesystem();
  }
}
