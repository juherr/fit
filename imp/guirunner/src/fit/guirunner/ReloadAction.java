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

  /**
   * Generates an output Filename. Such filename has the same relative path to the outDir as the
   * inFile to inDir
   * 
   * @param inDir - canonical form of the input directory
   * @param outDir - canonical form of the output directory
   * @param inFile - canonical form of the input filename
   * @return
   */
  public static File generateOutputFilename(File inDir, File outDir, File inFile) {
    StringBuffer sb = new StringBuffer(outDir.getAbsolutePath().length()
        + inFile.getAbsolutePath().length());
    sb.append(outDir.getAbsolutePath());
    sb.append(System.getProperties().getProperty("file.separator"));
    sb.append(inFile.getAbsolutePath().substring(inDir.getAbsolutePath().length()));
    return new File(sb.toString());
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
    final File outDir = ctx.getOutDir();
    return new MapFunction() {
      public Object f(Object current) {
        File inFile = (File)current;
        File outFile = generateOutputFilename(inDir, outDir, inFile);
        return new RunnerEntry(inFile, outFile);
      }
    };
  }

  protected boolean isActionEnabled() {
    return getLockCoordinator().canReadFilesystem();
  }
}
