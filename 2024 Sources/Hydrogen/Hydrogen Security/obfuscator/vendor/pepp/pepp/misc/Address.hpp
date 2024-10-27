#pragma once


#include "Concept.hpp"

namespace pepp {
	template<typename value_t = std::uintptr_t>
	class Address
	{
		value_t	m_address;
	public:
		template<typename T>
		constexpr Address(T value = 0)  noexcept requires pepp::msc::MemoryAddress<T>
			: m_address((value_t)(value))
		{
		}

		constexpr Address(const Address& rhs) = default;
		~Address() = default;

		Address& operator=(const Address& rhs) noexcept
		{
			m_address = rhs.m_address;
			return *this;
		}

		constexpr explicit operator std::uintptr_t() const noexcept
		{
			return m_address;
		}

		constexpr value_t uintptr() const noexcept
		{
			return m_address;
		}

		constexpr size_t size() const noexcept
		{
			return sizeof value_t;
		}

		template<typename C>
		C* ptr() noexcept requires pepp::msc::MemoryAddress<C*>
		{
			return reinterpret_cast<C*>(m_address);
		}

		template<typename C>
		C* ptr() const noexcept requires pepp::msc::MemoryAddress<C*>
		{
			return reinterpret_cast<C*>(m_address);
		}

		template<typename C>
		C as() noexcept
		{
			return (C)m_address;
		}

		template<typename C>
		C as() const noexcept
		{
			return (C)m_address;
		}

		template<typename C>
		C& deref() noexcept
		{
			return *reinterpret_cast<C*>(m_address);
		}

		template<typename C>
		C& deref(size_t idx) noexcept
		{
			return *reinterpret_cast<C*>(m_address + idx);
		}

		constexpr bool operator==(const Address& rhs) const noexcept
		{
			return m_address == rhs.m_address;
		}

		constexpr bool operator!=(const Address& rhs) const noexcept
		{
			return m_address != rhs.m_address;
		}

		constexpr bool operator>=(const Address& rhs) const noexcept
		{
			return m_address >= rhs.m_address;
		}

		constexpr bool operator<=(const Address& rhs) const noexcept
		{
			return m_address <= rhs.m_address;
		}

		constexpr bool operator>(const Address& rhs) const noexcept
		{
			return m_address > rhs.m_address;
		}

		constexpr bool operator<(const Address& rhs) const noexcept
		{
			return m_address < rhs.m_address;
		}

		constexpr Address operator+(const Address& rhs) const noexcept
		{
			return m_address + rhs.m_address;
		}

		constexpr Address operator-(const Address& rhs) const noexcept
		{
			return m_address - rhs.m_address;
		}

		constexpr Address operator*(const Address& rhs) const noexcept
		{
			return m_address * rhs.m_address;
		}

		constexpr Address operator/(const Address& rhs) const noexcept
		{
			return m_address / rhs.m_address;
		}

		constexpr Address& operator+=(const Address& rhs) noexcept
		{
			m_address += rhs.m_address;
			return *this;
		}

		constexpr Address& operator-=(const Address& rhs) noexcept
		{
			m_address -= rhs.m_address;
			return *this;
		}

		constexpr Address& operator*=(const Address& rhs) noexcept
		{
			m_address *= rhs.m_address;
			return *this;
		}

		constexpr Address operator>>(const Address& rhs) const noexcept
		{
			return m_address >> rhs.m_address;
		}

		constexpr Address operator<<(const Address& rhs) const noexcept
		{
			return m_address << rhs.m_address;
		}

		constexpr Address operator^(const Address& rhs) const noexcept
		{
			return m_address ^ rhs.m_address;
		}

		constexpr Address operator&(const Address& rhs) const noexcept
		{
			return m_address & rhs.m_address;
		}

		constexpr Address operator|(const Address& rhs) const noexcept
		{
			return m_address | rhs.m_address;
		}

		/*
		/!
		*/

		constexpr Address& operator>>=(const Address& rhs) noexcept
		{
			m_address >>= rhs.m_address;
			return *this;
		}

		constexpr Address& operator<<=(const Address& rhs) noexcept
		{
			m_address <<= rhs.m_address;
			return *this;
		}

		constexpr Address& operator^=(const Address& rhs) noexcept
		{
			m_address ^= rhs.m_address;
			return *this;
		}

		constexpr Address& operator&=(const Address& rhs) noexcept
		{
			m_address &= rhs.m_address;
			return *this;
		}

		constexpr Address& operator|=(const Address& rhs) noexcept
		{
			m_address |= rhs.m_address;
			return *this;
		}
	};
}