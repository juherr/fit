package fit.guirunner;

import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import fit.GuiRunner;

public class EditConfigurationDialog extends JDialog {

  public static final String KEY_INDIR = "label.indir";
  public static final String KEY_OUTDIR = "label.outdir";
  public static final String KEY_LIBDIR = "label.libdir";
  public static final String KEY_PATTERN = "label.pattern";
  public static final String KEY_RUNNER_COMMAND = "label.runner.command";
  public static final String KEY_OPEN_COMMAND = "label.open.command";
  public static final String KEY_EDIT_COMMAND = "label.edit.command";
  public static final String KEY_CANCEL = "cancel";
  public static final String KEY_APPLY = "apply";
  public static final String KEY_TITLE = "title.EditConfiguration";

  RunnerResourceBundle resource;
  /**
   * resulting new configuration, if the user pressed the approve button. null otherwise
   */
  Configuration configuration;

  JTextField inDir;
  JTextField outDir;
  JTextField libDir;
  JTextField pattern;
  JTextField runnerCommand;
  JTextField openCommand;
  JTextField editCommand;
  JButton cancel;
  JButton apply;
  String dialogTitle;

  public EditConfigurationDialog(Resources resources, Configuration config) {
    super(resources.getApplicationFrame(), "", true);
    this.resource = resources.getResource();
    this.configuration = null;
    dialogTitle = resource.getResourceString(KEY_TITLE);
    setTitle(null);
    init();
    config2fields(config);
    init_layout();
  }

  private Configuration fields2config() {
    return new Configuration(inDir.getText(), outDir.getText(), libDir.getText(),
        pattern.getText(), runnerCommand.getText(), openCommand.getText(), editCommand.getText());
  }

  private void config2fields(Configuration configuration) {
    inDir.setText(configuration.getInDir());
    outDir.setText(configuration.getOutDir());
    libDir.setText(configuration.getLibDir());
    pattern.setText(configuration.getPattern());
    runnerCommand.setText(configuration.getRunnerCommand());
    openCommand.setText(configuration.getOpenCommand());
    editCommand.setText(configuration.getEditCommand());
  }

  protected void init() {
    inDir = new JTextField(40);
    outDir = new JTextField(40);
    libDir = new JTextField(40);

    pattern = new JTextField(10);
    runnerCommand = new JTextField(50);
    openCommand = new JTextField(50);
    editCommand = new JTextField(50);

    cancel = new JButton(resource.getResourceString(KEY_CANCEL));
    apply = new JButton(resource.getResourceString(KEY_APPLY));

    setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    getRootPane().setDefaultButton(apply);

    Action cancelAction = new AbstractAction() {
      public void actionPerformed(ActionEvent arg0) {
        configuration = null;
        dispose();
      }
    };
    cancel.addActionListener(cancelAction);
    apply.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent arg0) {
        configuration = fields2config();
        dispose();
      }
    });

    KeyStroke escKey = KeyStroke.getKeyStroke("ESCAPE");
    InputMap im = getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
    im.put(escKey, "ESCAPE");
    getRootPane().getActionMap().put("ESCAPE", cancelAction);
  }

  protected JLabel label(String key) {
    return new JLabel(resource.getResourceString(key));
  }

  protected void init_layout() {
    Container cp = getContentPane();
    cp.setLayout(new GridBagLayout());
    GridBagConstraints label = new GridBagConstraints();
    GridBagConstraints fields = new GridBagConstraints();
    GridBagConstraints label1;
    GridBagConstraints fields1;
    GridBagConstraints labelAbove;
    GridBagConstraints buttons = new GridBagConstraints();

    label.insets = new Insets(2, 8, 2, 4);
    label.gridwidth = GridBagConstraints.RELATIVE;
    label.weightx = 1;
    label.anchor = GridBagConstraints.EAST;
    label.gridx = 0;
    label1 = (GridBagConstraints)label.clone();
    label1.insets = new Insets(8, 8, 2, 4);
    labelAbove = (GridBagConstraints)label.clone();
    labelAbove.gridwidth = GridBagConstraints.REMAINDER;
    labelAbove.anchor = GridBagConstraints.WEST;

    fields.insets = new Insets(2, 2, 2, 4);
    fields.gridwidth = GridBagConstraints.REMAINDER;
    fields.weightx = 1;
    fields.anchor = GridBagConstraints.WEST;
    fields.gridx = GridBagConstraints.RELATIVE;
    fields1 = (GridBagConstraints)fields.clone();
    fields1.insets = new Insets(8, 2, 2, 4);

    buttons.insets = new Insets(8, 4, 4, 4);
    buttons.anchor = GridBagConstraints.LAST_LINE_END;
    buttons.gridwidth = GridBagConstraints.REMAINDER;

    cp.add(label(KEY_INDIR), label1);
    cp.add(inDir, fields1);
    cp.add(label(KEY_OUTDIR), label);
    cp.add(outDir, fields);
    cp.add(label(KEY_LIBDIR), label);
    cp.add(libDir, fields);

    cp.add(label(KEY_PATTERN), label);
    cp.add(pattern, fields);
    cp.add(label(KEY_RUNNER_COMMAND), label);
    cp.add(runnerCommand, fields);
    cp.add(label(KEY_OPEN_COMMAND), label);
    cp.add(openCommand, fields);
    cp.add(label(KEY_EDIT_COMMAND), label);
    cp.add(editCommand, fields);

    JPanel hlp = new JPanel();
    hlp.setLayout(new FlowLayout(FlowLayout.TRAILING, 0, 0));
    hlp.add(apply);
    hlp.add(cancel);
    cp.add(hlp, buttons);
  }

  // test
  public static void main(String[] args) {
    Resources resources = new Resources(null, GuiRunner.RESOURCE, null);
    final EditConfigurationDialog dlg = new EditConfigurationDialog(resources, new Configuration());
    SwingUtilities.invokeLater(new Runnable() {

      public void run() {
        dlg.pack();
        dlg.setVisible(true);
        if (dlg.getConfiguration() != null) {
          System.out.println("Approved: " + String.valueOf(dlg.getConfiguration()));
        }
      }
    });

  }

  /** return the new configuration. Returns null when the dialog was canceled */
  protected Configuration getConfiguration() {
    return configuration;
  }

  public void setTitle(String name) {
    String newTitle = dialogTitle;
    if (name != null) {
      newTitle = newTitle + " - " + name;
    }
    super.setTitle(newTitle);
  }

}
