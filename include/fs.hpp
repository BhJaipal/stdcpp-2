#pragma once

namespace std::fs {
	/** Gives current working dir by ENV[["PWD"]] */
	char *getcwd();
	bool does_file_exist(char *file);
	bool does_directory_exist(char *file);
}
