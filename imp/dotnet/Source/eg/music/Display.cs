// Copyright (c) 2002 Cunningham & Cunningham, Inc.
// Released under the terms of the GNU General Public License version 2 or later.

using System;

namespace eg.music {
    public class Display : fit.RowFixture {

        public override Type getTargetClass() {
            return typeof(Music);
        }

        public override Object[] query() {
            return MusicLibrary.displayContents();
        }
    }
}
