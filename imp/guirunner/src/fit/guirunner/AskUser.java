package fit.guirunner;

import java.awt.Component;
import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import fit.GuiRunner;

/*
 * Asks if the user want to select a configuration or to create a new one
 */
public class AskUser {
  public static final int CMD_CANCEL = 0;

  public static final int CMD_NEW = 1;

  public static final int CMD_EXISTING = 2;

  // resource keys
  public static final String KEY_FILEDIALOG_EXISTING = "text.filedialog.existing";

  public static final String KEY_FILEDIALOG_NEW = "text.filedialog.new";

  public static final String KEY_OPTION_NEW = "text.option.new";

  public static final String KEY_OPTION_EXISTING = "text.option.existiong";

  public static final String KEY_OPTION_CANCEL = "text.option.cancel";

  public static final String KEY_OPTION_TITLE = "text.option.title";

  public static final String KEY_OPTION_MESSAGE = "text.option.message";

  RunnerResourceBundle resource;

  int action;

  String selectedFile;

  public AskUser(RunnerResourceBundle resource) {
    this.resource = resource;
  }

  public void show(Component parent) {
    Object[] options = { resource.getResourceString(KEY_OPTION_NEW),
        resource.getResourceString(KEY_OPTION_EXISTING),
        resource.getResourceString(KEY_OPTION_CANCEL) };
    int result = JOptionPane.showOptionDialog(SwingUtilities.getRoot(parent), resource
        .getResourceString(KEY_OPTION_MESSAGE), resource.getResourceString(KEY_OPTION_TITLE),
        JOptionPane.DEFAULT_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[2]);
    switch (result) {
      case 0:
        action = CMD_NEW;
        break;
      case 1:
        action = CMD_EXISTING;
        break;
      default:
        action = CMD_CANCEL;
    }
    if (action != CMD_CANCEL) {
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
      result = fd.showOpenDialog(SwingUtilities.getRoot(parent));
      if (result == JFileChooser.APPROVE_OPTION) {
        selectedFile = fd.getSelectedFile().getAbsolutePath();
      } else {
        action = CMD_CANCEL;
      }
    }
  }

  // Test
  public static void main(String[] args) {
    new AskUser(new RunnerResourceBundle(GuiRunner.RESOURCE)).show(null);
  }

  public int getAction() {
    return action;
  }

  public String getSelectedFile() {
    return selectedFile;
  }
}
