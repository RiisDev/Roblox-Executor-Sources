#pragma once

#include <cstdint>

namespace memory {
	class fake_vmt_t {
	private:
		std::uintptr_t original_obj;
		std::uintptr_t original_vmt;
		std::uintptr_t length;
		
		std::uintptr_t* vmt;
		
		fake_vmt_t(std::uintptr_t obj, std::uintptr_t vt_size);
		~fake_vmt_t();

	public:
		static fake_vmt_t* from(std::uintptr_t obj);
		static fake_vmt_t* create(std::uintptr_t obj, std::uintptr_t length);

		std::uintptr_t swap_vfunction(std::uintptr_t vf, std::uintptr_t idx);

		void free();

		std::uintptr_t get_original_obj();
		std::uintptr_t get_original_vmt();
		std::uintptr_t get_length();
	};
}