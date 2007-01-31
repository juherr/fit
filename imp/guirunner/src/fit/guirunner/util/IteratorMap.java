package fit.guirunner.util;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class IteratorMap implements Iterator {

  final Iterator i;

  final MapFunction f;

  public IteratorMap(Iterator i, MapFunction f) {
    this.i = i;
    this.f = f;
  }

  public void remove() {
    throw new RuntimeException("not implemented!");
  }

  public boolean hasNext() {
    return i.hasNext();
  }

  public Object next() {
    if (!i.hasNext()) {
      throw new NoSuchElementException();
    }
    return f.f(i.next());
  }

}
