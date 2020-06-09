#pragma once
#include "setup.h"
#include "Resource.h"


namespace mv
{
	class Blob : public Resource
	{
	private:
		size_type _size;
		char* _data;

	public:
		Blob(const std::string& path);
		Blob(const Blob&) = delete;
		Blob(Blob&& other) noexcept;

		~Blob();

		Blob& operator=(const Blob&) = delete;
		Blob& operator=(Blob&& other) noexcept;

		/**
			\brief get size in bytes
		*/
		size_type size() const;
		/**
			\brief get byte from buffer
		*/
		char operator[](size_type i) const;
		/**
			\brief get pointer to buffer
		*/
		const char* data() const;
	};
}
