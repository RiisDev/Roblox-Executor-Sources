#pragma once
#include <sstream>
#include <string>
#include <cstdint>
#include <string_view>

namespace hyperion::conversion
{
	class writer
	{
	public:
		template <typename T>
		void write(T value)
		{
			stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void write_compressed_int(int value)
		{
			do
			{
				int v = value & 0x7F;
				value >>= 7;

				if (value)
					v |= 0x80;

				write<std::uint8_t>(v);
			} while (value);
		}

		void write_string(std::string value)
		{
			write_compressed_int(value.size());

			for(const auto& character : value)
				write<std::uint8_t>(character);
		}

		std::string to_string()
		{
			return stream.str();
		}

	private:
		std::stringstream stream;
	};
}