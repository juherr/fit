package fit.guirunner;

import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Iterator;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;

import javax.swing.JFileChooser;
import javax.swing.SwingUtilities;

import fit.guirunner.actions.AbstractAsyncAction;

/**
 * Loading, Editing and creating of new Configurations. Management of the MRU-List of Configurations
 * 
 * @author Martin Busik
 */
public class ConfigurationStorage implements GuiRunnerActions {

  public static final int MAX_MRU_ENTRIES = 5;
  public static final String KEY_INDIR = "indir";
  public static final String KEY_OUTDIR = "outdir";
  public static final String KEY_LIBDIR = "libdir";
  public static final String KEY_CMD_RUNNER = "command.runner";
  public static final String KEY_CMD_OPEN = "command.open";
  public static final String KEY_CMD_EDIT = "command.edit";
  public static final String KEY_PATTERN = "pattern";
  public static final String KEY_RUNNER_WD = "runner.workingDir";
  
  private static final int CMD_NEW = 1;
  private static final int CMD_EXISTING = 2;

  // resource keys
  public static final String KEY_FILEDIALOG_EXISTING = "text.filedialog.existing";
  public static final String KEY_FILEDIALOG_NEW = "text.filedialog.new";
  public static final String KEY_OPTION_NEW = "text.option.new";
  public static final String KEY_OPTION_EXISTING = "text.option.existiong";
  public static final String KEY_OPTION_CANCEL = "text.option.cancel";
  public static final String KEY_OPTION_TITLE = "text.option.title";
  public static final String KEY_OPTION_MESSAGE = "text.option.message";

  Resources resources;

  public ConfigurationStorage(Resources resources) {
    this.resources = resources;
  }

  public void setCurrentConfiguration(String filename) {
    if (filename != null) {
      setCurrentConfiguration(new File(filename));
    }
  }

  public void setCurrentConfiguration(File configFile) {
    setCurrentConfiguration(resources, configFile);
  }

  public static void setCurrentConfiguration(Resources resources, File configFile) {
    Configuration c = null;
    try {
      c = load(configFile);
      c.setConfigurationName(configFile);
      resources.getUserLayout().storeProperty(UserPreferences.KEY_CURRENT_CONFIGURATION,
          configFile.getAbsolutePath());
    } catch (IOException x) {
      // TODO: MessageService
      x.printStackTrace();
    }
    resources.setConfiguration(c);
  }

  public AbstractAsyncAction getOpenConfigurationAction(String resourceKey) {
    AbstractAsyncAction openConfigAction = new OpenConfigurationAction(CMD_EXISTING, resources
        .getLockCoordinator());
    resources.getResource().configureActionFromResource(openConfigAction, resourceKey);
    return openConfigAction;
  }

  public AbstractAsyncAction getNewConfigurationAction(String resourceKey) {
    AbstractAsyncAction newConfigAction = new OpenConfigurationAction(CMD_NEW, resources
        .getLockCoordinator());
    resources.getResource().configureActionFromResource(newConfigAction, resourceKey);
    return newConfigAction;
  }

  public AbstractAsyncAction getEditConfigurationAction(String resourceKey) {
    AbstractAsyncAction a = new AbstractAsyncAction() {
      public void doActionPerformed(ActionEvent e) {
        Configuration config = resources.getConfiguration();
        File configFile = config.getConfigurationFile();
        EditConfigurationDialog dlg = new EditConfigurationDialog(resources, config);
        dlg.setTitle(configFile.getAbsolutePath());
        dlg.pack();
        dlg.setLocationRelativeTo(resources.getApplicationFrame());
        dlg.setVisible(true);
        if (dlg.getConfiguration() != null) {
          try {
            Configuration newConfig = dlg.getConfiguration();
            save(newConfig, configFile);
            newConfig.setConfigurationName(configFile);
            resources.setConfiguration(newConfig);
          } catch (IOException e1) {
            // TODO MessageService
            e1.printStackTrace();
          }
        }
      }

      protected boolean isActionEnabled() {
        return getLockCoordinator().canEditConfiguration();
      }
    };
    resources.getResource().configureActionFromResource(a, resourceKey);
    return a;
  }

  protected File showOpenDialog(int a) {
    final int action = a;
    File choosen = null;
    File configDir = null;
    if (resources.getConfiguration() != null) {
      configDir = resources.getConfiguration().getConfigurationDir();
    }

    RunnerResourceBundle resource = resources.getResource();
    JFileChooser fd = new JFileChooser() {
      public void approveSelection() {
        File selectedFile = getSelectedFile();
        if ((selectedFile != null)) {
          if ((action == CMD_NEW && !selectedFile.exists())
              || (action == CMD_EXISTING && selectedFile.isFile())) {
            super.approveSelection();
          }
        }
      }
    };
    fd.setDialogTitle(resource.getResourceString((action == CMD_NEW) ? KEY_FILEDIALOG_NEW
        : KEY_FILEDIALOG_EXISTING));
    fd.setFileSelectionMode(JFileChooser.FILES_ONLY);
    if (configDir != null) {
      fd.setCurrentDirectory(configDir);
    }
    int result = fd.showOpenDialog(SwingUtilities.getRoot(resources.getApplicationFrame()));
    if (result == JFileChooser.APPROVE_OPTION) {
      choosen = fd.getSelectedFile().getAbsoluteFile();
    }
    return choosen;
  }

  public static Configuration load(File configfile) throws IOException {
    Configuration c = new Configuration();
    Properties hlp = new Properties();
    FileInputStream fis = null;
    try {
      if (configfile.canRead()) {
        fis = new FileInputStream(configfile);
        hlp.load(fis);
        for (Iterator i = hlp.entrySet().iterator(); i.hasNext();) {
          setProperty(c, (Map.Entry)i.next());
        }
      }
    } finally {
      if (fis != null) {
        fis.close();
      }
    }
    return c;
  }

  public static void save(Configuration c, File configfile) throws IOException {
    Properties hlp = new Properties();
    hlp.put(KEY_INDIR, c.getInDir());
    hlp.put(KEY_OUTDIR, c.getOutDir());
    hlp.put(KEY_LIBDIR, c.getLibDir());
    hlp.put(KEY_PATTERN, c.getPattern());
    hlp.put(KEY_CMD_RUNNER, c.getRunnerCommand());
    hlp.put(KEY_CMD_OPEN, c.getOpenCommand());
    hlp.put(KEY_CMD_EDIT, c.getEditCommand());
    hlp.put(KEY_RUNNER_WD, c.getRunnerWorkingDir());
    FileOutputStream fos = new FileOutputStream(configfile);
    try {
      hlp.store(fos, null);
    } finally {
      fos.close();
    }
  }

  private static void setProperty(Configuration c, Entry entry) {
    String key = (String)entry.getKey();
    String value = (String)entry.getValue();
    if (KEY_INDIR.equals(key)) {
      c.inDir = value;
    } else if (KEY_OUTDIR.equals(key)) {
      c.outDir = value;
    } else if (KEY_LIBDIR.equals(key)) {
      c.libDir = value;
    } else if (KEY_PATTERN.equals(key)) {
      c.pattern = value;
    } else if (KEY_CMD_RUNNER.equals(key)) {
      c.runnerCommand = value;
    } else if (KEY_CMD_OPEN.equals(key)) {
      c.openCommand = value;
    } else if (KEY_CMD_EDIT.equals(key)) {
      c.editCommand = value;
    } else if (KEY_RUNNER_WD.equals(key)) {
      c.runnerWorkingDir = value;
    } else {
      System.err.println("Configuration property " + key + " is not supported. Entry ignored");
    }
  }

  class OpenConfigurationAction extends AbstractAsyncAction {
    int newOrExisting;

    OpenConfigurationAction(int newOrExisting, GlobalLockCoordinator lockCoordinator) {
      this.newOrExisting = newOrExisting;
      setLockCoordinator(lockCoordinator);
    }

    protected boolean isActionEnabled() {
      return !(getLockCoordinator().isReadingFilesystem() || getLockCoordinator()
          .isRunnerIsRunning());
    }

    public void doActionPerformed(ActionEvent e) {
      Configuration c = null;
      File selected = showOpenDialog(newOrExisting);
      if (selected != null) {
        try {
          if (newOrExisting == CMD_NEW) {
            c = new Configuration();
            save(c, selected);
          }
          setCurrentConfiguration(selected);
        } catch (IOException x) {
          // TODO MessageSerivce
          x.printStackTrace();
        }
      }
    }
  }
}
