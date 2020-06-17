#pragma once
#include "setup.h"
#include "Resource.h"


namespace mv
{
	class Blob final : public Resource
	{
	private:
		size_type _size;
		char* _data;

	public:
		Blob(const std::string& path);
		Blob(std::string&& path);

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

	private:
		void _load() override;
		void _unload() override;
	};
}
