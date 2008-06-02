package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

import fit.guirunner.EnvironmentContext;
import fit.guirunner.GlobalLockCoordinator;
import fit.guirunner.NewTestfileDialog;
import fit.guirunner.Resources;
import fit.guirunner.RunnerEntry;
import fit.guirunner.RunnerResourceBundle;
import fit.guirunner.RunnerTableModel;
import fit.guirunner.logic.SimpleWildcardFilter;
import fit.guirunner.logic.VariableExpansion;

public class NewTestAction extends AbstractAsyncAction {

  public static final String TEMPLATE_NAME = "fit/guirunner/resource/NewTestTemplate.html";
  public static final String TEMPLATE_ENCODING = "cp1252";
  public static final String KEY_TITLE = "title.NewTestfile";

  Resources resource;

  RunnerTableModel model;

  public NewTestAction(GlobalLockCoordinator lockCoordinator, Resources resource,
      RunnerTableModel model) {
    setLockCoordinator(lockCoordinator);
    this.resource = resource;
    this.model = model;
  }

  public void doActionPerformed(ActionEvent e) {
    FilenameFilter validator = new SimpleWildcardFilter(resource.getConfiguration().getPattern());
    NewTestfileDialog dlg;
    try {
      EnvironmentContext environmentContext = new EnvironmentContext(resource.getConfiguration());
      String dlgTitle = resource.getResource().getResourceString(KEY_TITLE);
      dlg = new NewTestfileDialog(resource, environmentContext, validator, dlgTitle);
      dlg.pack();
      dlg.setLocationRelativeTo(resource.getApplicationFrame());
      dlg.setVisible(true);
      File newFile = dlg.getSelectedNewFile();
      if (newFile != null) {
        saveTestTemplate(newFile);
        RunnerEntry re = new RunnerEntry(newFile, ReloadAction.generateRelativePath(
            environmentContext.getInDir(), newFile));
        model.addEntry(re);

        VariableExpansion strrep = new VariableExpansion("infile", newFile.getAbsolutePath());
        String cmd = strrep.replace(environmentContext.getEditCmd());
        Runtime.getRuntime().exec(cmd);
      }
    } catch (IOException e1) {
      // TODO: CommonLogging
      e1.printStackTrace();
    }
  }

  protected boolean isActionEnabled() {
    return !(getLockCoordinator().isRunnerIsRunning() || getLockCoordinator().isReadingFilesystem());
  }

  private void saveTestTemplate(File newFile) throws UnsupportedEncodingException,
      FileNotFoundException {
    String template = RunnerResourceBundle.readResourceFile(TEMPLATE_NAME, TEMPLATE_ENCODING);
    VariableExpansion ve = new VariableExpansion("title", newFile.getName());
    template = ve.replace(template);
    PrintWriter output = new PrintWriter(new OutputStreamWriter(new FileOutputStream(newFile),
        TEMPLATE_ENCODING));
    output.append(template);
    output.close();
  }
}
