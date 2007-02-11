package fit.guirunner;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;

import javax.swing.JFileChooser;
import javax.swing.SwingUtilities;

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
  File currentConfigurationFile = null;
  List mruList;

  public ConfigurationStorage(Resources resources) {
    this.resources = resources;
    currentConfigurationFile = null;
    mruList = new ArrayList(MAX_MRU_ENTRIES + 1);
  }

  public void setCurrentConfiguration(String filename) {
    if (filename != null) {
      setCurrentConfiguration(new File(filename));
    }
  }

  public void setCurrentConfiguration(File configFile) {
    Configuration c = null;
    try {
      c = load(configFile);
      EnvironmentContext.setCurrentDirectory(configFile.getParentFile());
      currentConfigurationFile = configFile;
      resources.getUserLayout().storeProperty(UserPreferences.KEY_CURRENT_CONFIGURATION,
          configFile.getAbsolutePath());
      // TODO: Add to mru list
      if(resources.getApplicationFrame() != null) {
        resources.getApplicationFrame().setTitle(configFile.getAbsolutePath());
      }
    } catch (IOException x) {
      // TODO: MessageService
      x.printStackTrace();
    }
    resources.setConfiguration(c);
  }

  public AbstractAsyncAction getOpenConfigurationAction(String resourceKey) {
    AbstractAsyncAction reloadAction = (AbstractAsyncAction)resources.getActionMap().get(
        REFRESH_ENTRIES);
    AbstractAsyncAction openConfigAction = new OpenConfigurationAction(CMD_EXISTING, reloadAction,
        resources.getLockCoordinator());
    openConfigAction.configureFromResources(resources.getResource(), resourceKey);
    return openConfigAction;
  }

  public AbstractAsyncAction getNewConfigurationAction(String resourceKey) {
    AbstractAsyncAction reloadAction = (AbstractAsyncAction)resources.getActionMap().get(
        REFRESH_ENTRIES);
    AbstractAsyncAction newConfigAction = new OpenConfigurationAction(CMD_NEW, 
        reloadAction, resources.getLockCoordinator());
    newConfigAction.configureFromResources(resources.getResource(), resourceKey);
    return newConfigAction;
  }

  public AbstractAsyncAction getEditConfigurationAction(String resourceKey) {
    final AbstractAsyncAction reloadAction = (AbstractAsyncAction)resources.getActionMap().get(
        REFRESH_ENTRIES);
    AbstractAsyncAction a = new AbstractAsyncAction() {
      public void doActionPerformed(ActionEvent e) {
        EditConfigurationDialog dlg = new EditConfigurationDialog(resources, resources.getConfiguration());
        dlg.setTitle(currentConfigurationFile.getAbsolutePath());
        dlg.setLocationRelativeTo(resources.getApplicationFrame());
        dlg.pack();
        dlg.show();
        if (dlg.getConfiguration() != null) {
          try {
            save(dlg.getConfiguration(), currentConfigurationFile);
            resources.setConfiguration(dlg.getConfiguration());
            reloadAction.doActionPerformed(null);
          } catch (IOException e1) {
            // TODO MessageService
            e1.printStackTrace();
          }
        }
      }

    };
    a.configureFromResources(resources.getResource(), resourceKey);
    return a;
  }

  protected File showOpenDialog(int a) {
    final int action = a;
    File choosen = null;
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
    if (currentConfigurationFile != null) {
      File wd = currentConfigurationFile.getAbsoluteFile().getParentFile();
      fd.setCurrentDirectory(wd);
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
      fis = new FileInputStream(configfile);
      hlp.load(fis);
      for (Iterator i = hlp.entrySet().iterator(); i.hasNext();) {
        setProperty(c, (Map.Entry)i.next());
      }
    } finally {
      if (fis != null) {
        fis.close();
      }
    }
    return c;
  }

  public void save(Configuration c, File configfile) throws IOException {
    Properties hlp = new Properties();
    hlp.put(KEY_INDIR, c.getInDir());
    hlp.put(KEY_OUTDIR, c.getOutDir());
    hlp.put(KEY_LIBDIR, c.getLibDir());
    hlp.put(KEY_PATTERN, c.getPattern());
    hlp.put(KEY_CMD_RUNNER, c.getRunnerCommand());
    hlp.put(KEY_CMD_OPEN, c.getOpenCommand());
    hlp.put(KEY_CMD_EDIT, c.getEditCommand());
    FileOutputStream fos = new FileOutputStream(configfile);
    hlp.store(fos, null);
    fos.close();
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
    } else {
      System.err.println("Configuration property " + key + " is not supported. Entry ignored");
    }
  }

  class OpenConfigurationAction extends AbstractAsyncAction {
    Component parent = null;
    AbstractAsyncAction reloadAction = null;
    int newOrExisting;

    OpenConfigurationAction(int newOrExisting, AbstractAsyncAction reloadAction,
        GlobalLockCoordinator lockCoordinator) {
      this.newOrExisting = newOrExisting;
      this.reloadAction = reloadAction;
      setParent(parent);
      setLockCoordinator(lockCoordinator);
    }

    public void setParent(Component parent) {
      this.parent = parent;
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
          reloadAction.doActionPerformed(null);
        } catch (IOException x) {
          // TODO MessageSerivce
          x.printStackTrace();
        }
      }
    }
  }
}
