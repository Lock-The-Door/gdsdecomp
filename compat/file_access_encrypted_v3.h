/*************************************************************************/
/*  file_access_encrypted_compat.h                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include <core/io/file_access_encrypted.h>

#ifndef FILE_ACCESS_ENCRYPTED_V3_H
#define FILE_ACCESS_ENCRYPTED_V3_H

#include "core/io/file_access.h"

#define ENCRYPTED_HEADER_MAGIC 0x43454447

class FileAccessEncryptedv3 : public FileAccess {
public:
	enum Mode {
		MODE_READ,
		MODE_WRITE_AES256,
		MODE_MAX
	};

private:
	Mode mode;
	Vector<uint8_t> key;
	bool writing = false;
	Ref<FileAccess> file;
	uint64_t base = 0;
	uint64_t length = 0;
	Vector<uint8_t> data;
	mutable uint64_t pos = 0;
	mutable bool eofed = false;
	bool use_magic = true;

	void _close();

public:
	Error open_and_parse(Ref<FileAccess> p_base, const Vector<uint8_t> &p_key, Mode p_mode, bool p_with_magic = true);
	Error open_and_parse_password(Ref<FileAccess> p_base, const String &p_key, Mode p_mode);

	virtual Error open_internal(const String &p_path, int p_mode_flags); ///< open a file
	virtual bool is_open() const; ///< true when file is open

	virtual String get_path() const; /// returns the path for the current open file
	virtual String get_path_absolute() const; /// returns the absolute path for the current open file

	virtual void seek(uint64_t p_position); ///< seek to a given position
	virtual void seek_end(int64_t p_position = 0); ///< seek from the end of file
	virtual uint64_t get_position() const; ///< get position in the file
	virtual uint64_t get_length() const; ///< get size of the file

	virtual bool eof_reached() const; ///< reading passed EOF

	virtual uint8_t get_8() const; ///< get a byte
	virtual uint64_t get_buffer(uint8_t *p_dst, uint64_t p_length) const;

	virtual Error get_error() const; ///< get last error

	virtual void flush();
	virtual void store_8(uint8_t p_dest); ///< store a byte
	virtual void store_buffer(const uint8_t *p_src, uint64_t p_length); ///< store an array of bytes

	virtual bool file_exists(const String &p_name); ///< return true if a file exists

	virtual uint64_t _get_modified_time(const String &p_file);
	virtual uint32_t _get_unix_permissions(const String &p_file);
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions);

	virtual void close() override;

	FileAccessEncryptedv3() {}
	~FileAccessEncryptedv3();
};

#endif //FILE_ACCESS_ENCRYPTED_V3_H