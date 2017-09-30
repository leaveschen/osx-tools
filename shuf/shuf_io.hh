//
// Created by c on 30/09/2017 10:27 AM
//

#ifndef OSX_TOOLS_SHUF_IO_HH
#define OSX_TOOLS_SHUF_IO_HH

/* include section */

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>

/* class & function section */

namespace osxtools {

class IBuffer {
private:
	char* _buffer;
	struct stat _st;
	std::vector<std::pair<size_t, size_t>> _meta;

	template<class T>
	void _delete(T* p) { if (p != nullptr) { delete p; p = nullptr; } }

public:
	/* constructor & destructor */
	IBuffer() : _buffer(nullptr), _st(), _meta() {}
	IBuffer(IBuffer const&) = delete;
	IBuffer& operator=(IBuffer const&) = delete;
	~IBuffer() { _delete(_buffer); }

	/* read from stdin */
	int read_from_stdin() {
		if (_buffer != nullptr) { return 1; }
		std::FILE* fp = std::tmpfile();
		if (!fp) { return 1; }
		for (std::string line; std::getline(std::cin, line);) {
			if (line.empty() or *(line.data()+line.length()-1) != '\n')
				line.push_back('\n');
			std::fputs(line.data(), fp);
		}
		std::rewind(fp);

		fstat(fileno(fp), &_st);
		_buffer = (char*)realloc(_buffer, sizeof(char)*_st.st_size);
		read(fileno(fp), _buffer, _st.st_size);
		std::fclose(fp);
		return 0;
	}

	/* read from file */
	int read_from_file(std::string const& fname) {
		if (_buffer != nullptr) { return 1; }
		std::FILE* fp = std::fopen(fname.c_str(), "r");
		fstat(fileno(fp), &_st);
		_buffer = (char*)realloc(_buffer, sizeof(char)*_st.st_size);
		read(fileno(fp), _buffer, _st.st_size);
		std::fclose(fp);
		return 0;
	}

	/* get meta info */
	void get_meta_info() {
		char* curr = _buffer;
		size_t pos_begin = 0;
		for (size_t pos = 0; pos < _st.st_size;) {
			/* note: first use unix read to read bytes from file stream,
			 *  then call c style memchr to search '\n'(10), efficiency
			 */
			pos_begin = pos;
			curr = (char*)memchr(curr, 10, 1024);
			pos = (size_t)(curr - _buffer);
			if (pos >= _st.st_size) break;
			_meta.push_back({pos_begin, pos-pos_begin+1});
			++curr; ++pos;
		}
	}

	/* shuffle the meta info
	 * Fisher–Yates shuffle
	 */
	void shuffle_meta_info() {
		unsigned int seed = (unsigned int)time(NULL);
		srand(seed);
		size_t n = _meta.size();
		for (size_t i = n-1; i > 0; i--) {
			size_t j = rand() % (i+1);
			if (j != i) std::swap(_meta[i], _meta[j]);
		}
	}

	void shuffle_buffer() {
		auto sz = sizeof(char);
		char* buffer_tmp = (char*)malloc(sz*_st.st_size);
		char* p = buffer_tmp;
		for (auto& x : _meta) {
			memcpy(p, _buffer+x.first, x.second);
			p += x.second;
		}
		std::swap(_buffer, buffer_tmp);
		free(buffer_tmp);
	}

	/* write to stdout */
	void write_stdout() {
		write(STDOUT_FILENO, _buffer, _st.st_size);
	}

};

} // namespace osxtools

#endif//


