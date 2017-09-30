//
// Created by c on 30/09/2017 12:10 PM
//

/* include section */

#include <iostream>
#include "shuf_io.hh"

/* class & function section */

int main(int argc, char* argv[]) {
	int read_flag;
	osxtools::IBuffer ib;
	if (argc == 1) {
		read_flag = ib.read_from_stdin();
	} else if (argc == 2) {
		read_flag = ib.read_from_file(argv[1]);
	} else {
		std::cerr << "[usage]: shuf [file]" << std::endl;
		return 0;
	}

	ib.get_meta_info();
	ib.shuffle_meta_info();
	ib.shuffle_buffer();
	ib.write_stdout();
	return 0;
}


