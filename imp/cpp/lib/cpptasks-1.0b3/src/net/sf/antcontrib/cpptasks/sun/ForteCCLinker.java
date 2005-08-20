/*
 *
 * Copyright 2001-2004 The Ant-Contrib project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package net.sf.antcontrib.cpptasks.sun;
import java.io.File;
import java.util.Vector;

import net.sf.antcontrib.cpptasks.CUtil;
import net.sf.antcontrib.cpptasks.compiler.LinkType;
import net.sf.antcontrib.cpptasks.compiler.Linker;
import net.sf.antcontrib.cpptasks.gcc.AbstractLdLinker;
import net.sf.antcontrib.cpptasks.CCTask;
import net.sf.antcontrib.cpptasks.types.LibrarySet;
import net.sf.antcontrib.cpptasks.types.LibraryTypeEnum;
/**
 * Adapter for Sun (r) Forte(tm) C++ Linker
 *
 * @author Curt Arnold
 */
public final class ForteCCLinker extends AbstractLdLinker {
    private static final String[] discardFiles = new String[]{".dll", ".so",
    ".sl"};
    private static final String[] objFiles = new String[]{".o", ".a", ".lib"};
    private static final ForteCCLinker arLinker = new ForteCCLinker("CC",
            objFiles, discardFiles, "lib", ".a");
    private static final ForteCCLinker dllLinker = new ForteCCLinker("CC",
            objFiles, discardFiles, "lib", ".so");
    private static final ForteCCLinker instance = new ForteCCLinker("CC",
            objFiles, discardFiles, "", "");
    public static ForteCCLinker getInstance() {
        return instance;
    }
    private File[] libDirs;
    private ForteCCLinker(String command, String[] extensions,
            String[] ignoredExtensions, String outputPrefix, String outputSuffix) {
        super(command, "-V", extensions, ignoredExtensions, outputPrefix,
                outputSuffix, false, null);
    }
    public void addImpliedArgs(boolean debug, LinkType linkType, Vector args) {
        if (debug) {
            args.addElement("-g");
        }
        if (linkType.isStaticRuntime()) {
            //args.addElement("-static");
        }
        if (linkType.isSharedLibrary()) {
            args.addElement("-G");
        }
        if (linkType.isStaticLibrary()) {
            args.addElement("-xar");
        }
    }
    public void addIncremental(boolean incremental, Vector args) {
        /*
         * if (incremental) { args.addElement("-xidlon"); } else {
         * args.addElement("-xidloff"); }
         */
    }

    // HACK:  copied from AbstractLdLinker by DW and modified to not emit -Bstatic by default
    public String[] addLibrarySets(CCTask task, LibrarySet[] libsets,
            Vector preargs, Vector midargs, Vector endargs) {
        Vector libnames = new Vector();
        //super.addLibrarySets(task, libsets, preargs, midargs, endargs);  // was a noop in AbstractLdLinker - DW
        LibraryTypeEnum previousLibraryType = null;
        for (int i = 0; i < libsets.length; i++) {
            LibrarySet set = libsets[i];
            File libdir = set.getDir(null);
            String[] libs = set.getLibs();
            if (libdir != null) {
                    if (set.getType() != null &&
                                    "framework".equals(set.getType().getValue()) &&
                                                isDarwin()) {
                            endargs.addElement("-F" + libdir.getAbsolutePath());
                    } else {
                            endargs.addElement("-L" + libdir.getAbsolutePath());
                    }
            }

            // HACK:  added by DW to default our library type to static and keep -Bstatic from being emitted
            if(previousLibraryType == null && set.getType() != null && "static".equals(set.getType().getValue())) {
                previousLibraryType = set.getType();
            }

            //
            //  if there has been a change of library type
            //
            if (set.getType() != previousLibraryType) {
                    if (set.getType() != null && "static".equals(set.getType().getValue())) {
                            endargs.addElement("-Bstatic");
                            previousLibraryType = set.getType();
                    } else {
                            if (set.getType() == null ||
                                            !"framework".equals(set.getType().getValue()) ||
                                                        !isDarwin()) {
                                    endargs.addElement("-Bdynamic");
                                    previousLibraryType = set.getType();
                            }
                    }
            }
            StringBuffer buf = new StringBuffer("-l");
            if (set.getType() != null &&
                            "framework".equals(set.getType().getValue()) &&
                                        isDarwin()) {
                    buf.setLength(0);
                    buf.append("-framework ");
            }
            int initialLength = buf.length();
            for (int j = 0; j < libs.length; j++) {
                //
                //  reset the buffer to just "-l"
                //
                buf.setLength(initialLength);
                //
                //  add the library name
                buf.append(libs[j]);
                libnames.addElement(libs[j]);
                //
                //  add the argument to the list
                endargs.addElement(buf.toString());
            }
        }
        String rc[] = new String[libnames.size()];
        for (int i = 0; i < libnames.size(); i++) {
            rc[i] = (String) libnames.elementAt(i);
        }
        return rc;
    }

    /**
     * Returns library path.
     *
     */
    public File[] getLibraryPath() {
        if (libDirs == null) {
            File CCloc = CUtil.getExecutableLocation("CC");
            if (CCloc != null) {
                File compilerLib = new File(new File(CCloc, "../lib")
                        .getAbsolutePath());
                if (compilerLib.exists()) {
                    libDirs = new File[2];
                    libDirs[0] = compilerLib;
                }
            }
            if (libDirs == null) {
                libDirs = new File[1];
            }
        }
        libDirs[libDirs.length - 1] = new File("/usr/lib");
        return libDirs;
    }
    public Linker getLinker(LinkType type) {
        if (type.isStaticLibrary()) {
            return arLinker;
        }
        if (type.isSharedLibrary()) {
            return dllLinker;
        }
        return instance;
    }
}
