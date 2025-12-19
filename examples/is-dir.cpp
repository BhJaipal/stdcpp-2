#include <fs.hpp>
#include <file.hpp>

int main(int c, char **v) {
	if (c > 1) {
		return !std::fs::does_directory_exist(v[1]);
	} else {
		std::io::stderr().fwrite("Expected dir_name as 2nd argument\n");
		return 1;
	}
}
