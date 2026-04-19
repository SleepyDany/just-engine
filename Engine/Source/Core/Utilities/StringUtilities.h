#pragma once

#define JE_PRIVATE_TYPE_FORMATTER_IMPL(Type, FormatterImpl)                 \
	template <>                                                             \
	struct std::formatter<Type>                                             \
	{                                                                       \
		constexpr auto parse(std::format_parse_context& _ctx)               \
		{                                                                   \
			auto it = _ctx.begin();                                         \
			while (it != _ctx.end() && *it != '}')                          \
			{                                                               \
				++it;                                                       \
			}                                                               \
			return it;                                                      \
		}                                                                   \
                                                                            \
		auto format(const Type& _obj, std::format_context& _ctx) const      \
		{                                                                   \
			std::ostringstream out;                                         \
			out << (FormatterImpl);                                         \
			return std::ranges::copy(std::move(out).str(), _ctx.out()).out; \
		}                                                                   \
	};

#define JE_DEFINE_DEFAULT_TYPE_FORMATTER(Type) JE_PRIVATE_TYPE_FORMATTER_IMPL(Type, _obj.ToString())

#define JE_DEFINE_CUSTOM_TYPE_FORMATTER(Type, CustomFormatterImpl) JE_PRIVATE_TYPE_FORMATTER_IMPL(Type, CustomFormatterImpl(_obj))
