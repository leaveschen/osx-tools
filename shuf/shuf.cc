//
// Created by c on 30/09/2017 12:10 PM
//

/* include section */

#include <iostream>
#include "shuf_io.hh"

/* class & function section */

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "[usage]: shuf [file]" << std::endl;
		return 0;
	}
	osxtools::IBuffer ib;
	ib.read_from_file(argv[1]);
	ib.get_meta_info();
	ib.shuffle_meta_info();
	ib.shuffle_buffer();
	ib.write_stdout();
	return 0;
}


