#ifndef COMMAND_MESSAGE_HPP
#define COMMAND_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class command_message
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 128 };
	
	command_message()
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

	const char* body() const
	{
		return data_ + header_length;
	}

    int length() const
    {
   		return header_length + body_length_;
    }

	char* body()
	{
		return data_ + header_length;
	}

	int body_length() const
	{
		return body_length_;
	}

	void body_length(int new_length)
	{
		body_length_ = new_length;
		if(body_length_ > max_body_length)
			body_length_ = max_body_length;
	}

	bool decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length_ = std::atoi(header);
		if(body_length_ > max_body_length)
		{
			body_length_ = 0;
			return 0;
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

#endif // COMMAND_MESSAGE_HPP