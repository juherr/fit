//Copyright (c) 2002 Cunningham & Cunningham, Inc.
//Released under the terms of the GNU General Public License version 2 or later.
/*
 * Extensions Copyright (c) 2004 Rick Mugridge, University of Auckland, NZ
 * Released under the terms of the GNU General Public License version 2 or later.
 *
 */
package fit;

/**
 * Simply to make it available as a separate Class
 */
public class Counts {
    public int right = 0;
    public int wrong = 0;
    public int ignores = 0;
    public int exceptions = 0;

    public String toString() {
        return
            right + " right, " +
            wrong + " wrong, " +
            ignores + " ignored, " +
            exceptions + " exceptions";
    }

    public void tally(Counts source) {
        right += source.right;
        wrong += source.wrong;
        ignores += source.ignores;
        exceptions += source.exceptions;
    }
}
