#ifndef TIN_TERMINAL_SRC_TERMINALMESSAGE_HPP
#define TIN_TERMINAL_SRC_TERMINALMESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

class terminal_message
{
public:
	enum { max_body_length = 1024 };
	enum { header_length = 4 };

	terminal_message()
		: body_length_(0)
	{
	}

	const char* data() const
	{
		return data_;
	}

	char* data()
	{
		return data_;
	}

	int length() const
	{
		return header_length + body_length_;
	}

	const char* body() const
	{
		return data_ + header_length;
	}

	char* body()
	{
		return data_ + header_length;
	}

	int body_length() const
	{
		return body_length_;
	}

	void body_length(int length)
	{
		body_length_ = std::min(length, (int) max_body_length);
	}

	bool decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length_ = std::atoi(header);
		if(body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		char header[header_length + 1] = "";
		std::sprintf(header, "%4d", body_length_);
		std::memcpy(data_, header, header_length);
	}

private:
	char data_[header_length + max_body_length];
	int body_length_;
};

#endif