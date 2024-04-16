#pragma once

#include <string_view>
#include <cstdint>

namespace Creepy::Reflection {

    template <typename T>
	constexpr std::string_view TypeName() noexcept;

	template <>
	constexpr std::string_view TypeName<void>() noexcept {
		return "void";
	}

	namespace hidden{
		
		using void_tt = void;

        // TODO: Use Another macro with clang and msvc
		template <typename T>
		constexpr std::string_view wrapperTypeName() noexcept {
			return __PRETTY_FUNCTION__;
		}

		constexpr size_t wrapperTypePrefixSize() noexcept {
			return wrapperTypeName<void_tt>().find(TypeName<void_tt>());
		}

		constexpr size_t wrapperTypeSuffixSize() noexcept {
			return wrapperTypeName<void_tt>().length() - wrapperTypePrefixSize() - TypeName<void_tt>().length();
		}
	}

	template <typename T>
	constexpr std::string_view TypeName() noexcept {
		constexpr auto Name = hidden::wrapperTypeName<T>();
		constexpr auto PrefixSize = hidden::wrapperTypePrefixSize();
		constexpr auto SuffixSize = hidden::wrapperTypeSuffixSize();
		constexpr auto NameLength = Name.length() - PrefixSize - SuffixSize;
		return Name.substr(PrefixSize, NameLength);
	}

	// Hash
	constexpr uint64_t hash(std::string_view str) noexcept {
		uint64_t hash_value = 0xcbf29ce484222325ull;
		constexpr uint64_t prime = 0x100000001b3ull;

		for(auto& c : str){
			hash_value ^= static_cast<uint64_t>(c);
			hash_value *= prime;
		}
		return hash_value;
	}

	// TypeId From Hash Value
	template <typename T>
	constexpr uint64_t TypeHash() noexcept {
		return hash(TypeName<T>());
	}

}