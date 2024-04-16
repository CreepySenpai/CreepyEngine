#pragma once

#include <bitset>
#include <unordered_map>
#include "ReflectionCore.hpp"

namespace Creepy::Reflection {

    enum TypeFlags{
		FUNDAMENTAL = 0, INTEGRAL, FLOATINGPOINT, TRIVIALLY_COPYABLE, SIZE
	};

	struct TypeInfo {
		std::string_view Name{};
		uint32_t Size{};
		uint32_t Align{};
		std::bitset<TypeFlags::SIZE> Flags{};

		template <typename T>
		static constexpr TypeInfo Create() noexcept;
	};

	template <typename T>
	inline constexpr TypeInfo TypeInfo::Create() noexcept {
		TypeInfo info;
		info.Name = Reflection::TypeName<T>();
		info.Size = sizeof(T);
		info.Align = alignof(T);

		info.Flags[TypeFlags::TRIVIALLY_COPYABLE] = std::is_trivially_copyable_v<T>;
		info.Flags[TypeFlags::INTEGRAL] = std::is_integral_v<T>;
		info.Flags[TypeFlags::FLOATINGPOINT] = std::is_floating_point_v<T>;
		info.Flags[TypeFlags::FUNDAMENTAL] = std::is_fundamental_v<T>;

		return info;
	}

	class TypeID {
		public:
			constexpr TypeID(uint64_t id) noexcept : m_id{id}{}
			
			template <typename T>
			constexpr void SetTypeID(uint64_t id) noexcept {
				m_id = id;
			}

			constexpr uint64_t GetID() noexcept {
				return m_id;
			}

			const TypeInfo& GetTypeInfo() noexcept {
				return GetTypeInfomation(*this);
			}

			template <typename T>
			static constexpr TypeID Create() noexcept;

			static const TypeInfo& GetTypeInfomation(TypeID id) noexcept {
				return GetStaticsStorage().TypeInfos[id.GetID()];
			}

			static const auto& GetAllTypeInformation() noexcept {
				return GetStaticsStorage().TypeInfos;
			}

			template <typename T>
			static void RegisterTypeID() noexcept {
				
				constexpr const uint64_t id = TypeID::Create<T>().GetID();

				auto&& s_typeInfos = GetStaticsStorage().TypeInfos;
				s_typeInfos.emplace(id, TypeInfo::Create<T>());
			}

		private:
			uint64_t m_id;

			struct StaticData{
				std::unordered_map<uint64_t, TypeInfo> TypeInfos{};
			};

			static constexpr StaticData& GetStaticsStorage(){
				static StaticData data{};
				return data;
			}
	};

	template <typename T>
	inline constexpr Reflection::TypeID TypeID::Create() noexcept {
		return {Reflection::TypeHash<T>()};
	}


	template <typename T>
	class RegisterType{
		private:
			class RegisterTypeOnce{
				public:
					constexpr RegisterTypeOnce() noexcept {
						TypeID::RegisterTypeID<T>();
					}
					
			};

			inline static constexpr RegisterTypeOnce Register{};
	};

}