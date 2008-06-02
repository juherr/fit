package fit.guirunner;

public interface GuiRunnerActions {
  final String REFRESH_ENTRIES = "REFRESH_ENTRIES";
  final String RUN_ALL = "RUN_ALL";
  final String RUN_CURRENT = "RUN_CURRENT";
  final String EDIT_INPUTFILE = "EDIT_INPUTFILE";
  final String OPEN_OUTPUTFILE = "OPEN_OUTPUTFILE";
  final String SHOW_RUNNER_OUTPUT = "SHOW_RUNNER_OUTPUT";
  final String OPEN_CONFIG = "OPEN_CONFIG";
  final String NEW_CONFIG = "NEW_CONFIG";
  final String EDIT_CONFIG = "EDIT_CONFIG";
  final String MRU_CONFIG = "MRU_CONFIG";
  final String EXIT = "EXIT";
  final String SYSTEM_INFO = "SYSTEM_INFO";

  // TBD
  final String RUN_MARKED = "RUN_MARKED";
  final String STOP_TESTS = "STOP_TESTS";
  final String TERMINATE_TESTS = "TERMINATE_TESTS";
  final String NEW_TEST = "NEW_TEST";
  final String DELETE_TEST = "DELETE_TEST";
  final String RENAME_TEST = "RENAME_TEST";
  final String MOVE_TEST = "MOVE_TEST";

  // test only
  final String APPEND_ENTRY = "APPEND_ENTRY";
}
