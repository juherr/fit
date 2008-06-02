package fit.guirunner;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.KeyStroke;

public class DetailDialog extends JDialog {

  public DetailDialog(Frame parent, String title, String text) {
    super(parent, true);
    setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

    Action closeAction = new AbstractAction() {
      public void actionPerformed(ActionEvent e) {
        dispose();
      }
    };

    getContentPane().setLayout(new BorderLayout());
    JTextArea textArea = new JTextArea(text, 10, 80);
    textArea.setEditable(false);
    textArea.setBackground(Color.WHITE); // default bg color for read only is grey
    textArea.setAutoscrolls(true);
    textArea.setLineWrap(true);
    getContentPane().add(new JScrollPane(textArea), BorderLayout.CENTER);

    JButton ok = new JButton("ok");
    ok.addActionListener(closeAction);

    JPanel hlp = new JPanel();
    hlp.setLayout(new FlowLayout(FlowLayout.CENTER));
    hlp.add(ok);

    getContentPane().add(hlp, BorderLayout.SOUTH);

    // ESC + ENTER shall close the dialog
    getRootPane().setDefaultButton(ok);
    KeyStroke escKey = KeyStroke.getKeyStroke("ESCAPE");
    InputMap im = getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
    im.put(escKey, "ESCAPE");
    getRootPane().getActionMap().put("ESCAPE", closeAction);
  }
}
