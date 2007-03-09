package fit.guirunner;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Locale;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.text.html.HTMLEditorKit;

import fit.GuiRunner;
import fit.guirunner.logic.VariableExpansion;

public class AboutAction extends AbstractAsyncAction {

  Resources resources;

  AboutAction(Resources res) {
    resources = res;
  }

  public void doActionPerformed(ActionEvent e) {
    AboutDialog dlg = new AboutDialog(resources.getApplicationFrame());
    dlg.pack();
    dlg.setLocationRelativeTo(resources.getApplicationFrame());
    dlg.show();
  }

  // test
  public static void main(String[] args) {
    final Resources res = new Resources(null, GuiRunner.RESOURCE, null);
    final AboutAction a = new AboutAction(res);
    SwingUtilities.invokeLater(new Runnable() {
      public void run() {
        a.doActionPerformed(null);
        System.exit(0);
      }
    });
  }
}

class AboutDialog extends JDialog implements RunnerVersion {
  public static final String DEFAULT_RESOURCES = "fit/guirunner/resource/";
  public static final String DEFAULT_ABOUT = "about.html";
  // Encoding of the input stream delivered by the class loader
  public static final String RESOURCE_ENCODING = "iso-8859-1";

  public AboutDialog(JFrame parent) {
    super(parent, true);
    setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
    Container pane = getContentPane();
    pane.setLayout(new BorderLayout());

    JButton ok = new JButton("ok");
    ok.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent arg0) {
        dispose();
      }
    });
    JPanel hlp = new JPanel();
    hlp.setLayout(new FlowLayout(FlowLayout.CENTER));
    hlp.add(ok);
    pane.add(hlp, BorderLayout.SOUTH);

    JEditorPane info = new JEditorPane();
    info.setEditorKit(new HTMLEditorKit());
    info.setEditable(false);
    info.addHyperlinkListener(new HyperlinkListener() {
      public void hyperlinkUpdate(HyperlinkEvent e) {
        if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
          try {
            // TODO: support other OS-es than win32
            Runtime.getRuntime().exec("cmd /C start \"dummy...\" " + e.getURL().toExternalForm());
          } catch (IOException e1) {
            // TODO Message
            e1.printStackTrace();
          }
        }
      }

    });
    info.setText(getAboutText());
    pane.add(info, BorderLayout.NORTH);
  }

  private String getAboutText() {
    String defaultAboutString = "<html><body><b>fit.GuiRunner</b><p><a href=\"http://fit.c2.com\">fit.c2.com</a><br>written by Martin Busik<br>&copy; 2007</p></body></html>";
    String result;
    String language = Locale.getDefault().getLanguage();
    result = readResourceFile(DEFAULT_RESOURCES + language + "_" + DEFAULT_ABOUT);
    if(result == null) {
      result = readResourceFile(DEFAULT_RESOURCES + DEFAULT_ABOUT);
    }
    if(result == null) {
      result = defaultAboutString;
    }
    VariableExpansion exp = new VariableExpansion("version",RUNNER_VERSION,"ID",RUNNER_ID);
    result = exp.replace(result);
    return result;
  }

  /**
   * 
   * @param url
   * @return
   */
  public static String readResourceFile(String name) {
    String result = null;
    try {
      StringBuffer sb = new StringBuffer();
      InputStream is;
      is = Thread.currentThread().getContextClassLoader().getResourceAsStream(name);
      if(is != null) {
        InputStreamReader isr = new InputStreamReader(is, RESOURCE_ENCODING);
        BufferedReader sr = new BufferedReader(isr);
        String line;
        while ((line = sr.readLine()) != null) {
          sb.append(line).append("\n");
        }
        result = sb.toString();
      } else {
        // System.err.println("Resource not found: "+name);
      }
    } catch (IOException e) {
      // TODO better handling
      e.printStackTrace();
    }
    return result;
  }
}