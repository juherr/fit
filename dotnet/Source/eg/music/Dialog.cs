// Copyright (c) 2002, 2003 Cunningham & Cunningham, Inc.
// Read license.txt in this directory.

using fit;

namespace eg.music {

    public class Dialog : Fixture {
        protected internal string messageVar;
        protected internal Fixture caller;

        protected internal Dialog (string message, Fixture caller) {
            this.messageVar = message;
            this.caller = caller;
        }

        public string message() {
            return messageVar;
        }

        public void ok () {
            if (messageVar.Equals("load jamed"))   {MusicPlayer.stop();}
            ActionFixture.actor = caller;
        }

    }
}