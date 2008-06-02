package fit.guirunner.actions;

import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.Properties;

import fit.guirunner.DetailDialog;
import fit.guirunner.Resources;

public class SystemInfoAction extends AbstractAsyncAction {

  Resources resources;

  public SystemInfoAction(Resources res) {
    resources = res;
  }

  public void doActionPerformed(ActionEvent e) {
    DetailDialog dlg = new DetailDialog(resources.getApplicationFrame(), "System information",
        getSystemProperties());
    dlg.pack();
    dlg.setLocationRelativeTo(resources.getApplicationFrame());
    dlg.setVisible(true);
  }

  public static String getSystemProperties() {
    Properties prop = System.getProperties();
    StringBuffer sb = new StringBuffer(prop.size() * 100);
    ArrayList keys = new ArrayList();
    keys.addAll(prop.keySet());
    Collections.sort(keys);
    for (Iterator i = keys.iterator(); i.hasNext();) {
      Object key = i.next();
      sb.append(key).append("=").append(prop.get(key)).append("\n");
    }
    return sb.toString();
  }
}
