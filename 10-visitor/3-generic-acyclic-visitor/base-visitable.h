#pragma once

#include "base-visitor.h"
#include <type_traits>

template <typename R = void>
class BaseVisitable
{
public:
	using ReturnType = R;
	virtual ~BaseVisitable() = default;
	virtual ReturnType Accept(BaseVisitor &visitor) = 0;

	static_assert(std::is_void_v<ReturnType> || std::is_default_constructible_v<ReturnType>,
				  "ReturnType must be void or default-constructible");

protected:
	template <typename T>
	static ReturnType AcceptImpl(T &element, BaseVisitor &visitor)
	{
		if (auto specificVisitor = dynamic_cast<Visitor<T, R> *>(&visitor))
		{
			return specificVisitor->visit(element);
		}
		if constexpr (std::is_void_v<ReturnType>)
		{
			return;
		}
		return ReturnType{};
	}

	// Small inline helper to implement Accept in concrete visitable types
	template <typename T>
	static ReturnType AcceptThis(T &element, BaseVisitor &visitor)
	{
		return AcceptImpl(element, visitor);
	}
};
