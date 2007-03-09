package fit.guirunner;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JTextArea;

public class DetailDialog extends JDialog implements ActionListener {

  public DetailDialog(Frame parent, String title, String text) {
    super(parent, true);
    getContentPane().setLayout(new BorderLayout());
    JTextArea textArea = new JTextArea(text);
    textArea.setAutoscrolls(true);
    textArea.setLineWrap(true);
    textArea.setRows(10);
    textArea.setColumns(80);
    getContentPane().add(textArea, BorderLayout.NORTH);

    JButton ok = new JButton("ok");
    ok.addActionListener(this);

    JPanel hlp = new JPanel();
    hlp.setLayout(new FlowLayout(FlowLayout.CENTER));
    hlp.add(ok);
    
    getContentPane().add(hlp, BorderLayout.SOUTH);
  }
  public void actionPerformed(ActionEvent arg0) {
    dispose();
  }
}
