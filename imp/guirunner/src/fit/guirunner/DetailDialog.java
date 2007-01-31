package fit.guirunner;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
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
    getContentPane().add(ok, BorderLayout.SOUTH);
  }

  public Dimension getPreferredSize() {
    return new Dimension(500, 300);
  }

  public void actionPerformed(ActionEvent arg0) {
    dispose();
  }
}
