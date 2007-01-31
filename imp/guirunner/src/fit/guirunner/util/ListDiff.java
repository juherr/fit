package fit.guirunner.util;

import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

public class ListDiff {
  Set left;

  Set right;

  Set leftOnly;

  Set rightOnly;

  Set both;

  boolean computed;

  public ListDiff(Collection left, Collection right) {
    leftOnly = new HashSet();
    rightOnly = new HashSet();
    both = new HashSet();
    this.left = new HashSet();
    this.right = new HashSet();
    this.left.addAll(left);
    this.right.addAll(right);
    computed = false;
  }

  private void doCompute() {
    if (!computed) {
      Set hlp = new HashSet();
      hlp.addAll(left);
      hlp.addAll(right);
      for (Iterator i = hlp.iterator(); i.hasNext();) {
        Object anEntry = i.next();
        boolean inLeft = left.contains(anEntry);
        boolean inRight = right.contains(anEntry);
        if (inLeft && inRight) {
          both.add(anEntry);
        } else {
          if (inLeft) {
            leftOnly.add(anEntry);
          } else {
            rightOnly.add(anEntry);
          }
        }
      }
      left = null;
      right = null;
      computed = true;
    }
  }

  public boolean hasDiffs() {
    doCompute();
    return leftOnly.size() > 0 || rightOnly.size() > 0;
  }

  public Set getBoth() {
    doCompute();
    return both;
  }

  public Set getLeftOnly() {
    doCompute();
    return leftOnly;
  }

  public Set getRightOnly() {
    doCompute();
    return rightOnly;
  }
}
