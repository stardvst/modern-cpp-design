#pragma once

#include "algorithms/type-traits.h"
#include "algorithms/typeat-nonstrict.h"
#include "functor-handler.h"
#include "functor-impl.h"
#include "member-function-handler.h"

#include <memory>

template <typename R, typename TList = NullType>
class Functor
{
public:
	using ResultType = R;
	using ParamList = TList;

	// Functor();
	// Functor(const Functor &other);
	// Functor &operator=(const Functor &other);

	template <typename Fun>
	Functor(const Fun &fun);

	using Param1 = typename TypeAtNonStrict<TList, 0>::Result;
	using Param2 = typename TypeAtNonStrict<TList, 1>::Result;
	using Param3 = typename TypeAtNonStrict<TList, 2>::Result;

	explicit Functor(std::unique_ptr<FunctorImpl<ResultType, TList>> impl) : m_impl(std::move(impl))
	{
	}

	template <typename PtrObj, typename PtrMemFn>
	explicit Functor(const PtrObj &obj, PtrMemFn memFn) : m_impl(std::make_unique<MemFunHandler<Functor, PtrObj, PtrMemFn>>(obj, memFn))
	{
	}

	ResultType operator()()
	{
		return (*m_impl)();
	}

	// typename Loki::TypeTraits<Param1>::ParameterType:
	// U, const U -> U if U is fundamental type, otherwise const U &
	// U &				-> U &
	// const U &	-> const U &
	ResultType operator()(typename Loki::TypeTraits<Param1>::ParameterType p1)
	{
		return (*m_impl)(p1);
	}

	ResultType operator()(typename Loki::TypeTraits<Param1>::ParameterType p1, typename Loki::TypeTraits<Param2>::ParameterType p2)
	{
		return (*m_impl)(p1, p2);
	}

	ResultType operator()(typename Loki::TypeTraits<Param1>::ParameterType p1, typename Loki::TypeTraits<Param2>::ParameterType p2,
	  typename Loki::TypeTraits<Param3>::ParameterType p3)
	{
		return (*m_impl)(p1, p2, p3);
	}

private:
	using ImplType = FunctorImpl<ResultType, TList>;
	std::unique_ptr<ImplType> m_impl{nullptr};
};

template <typename ResultType, typename TList>
template <typename Fun>
Functor<ResultType, TList>::Functor(const Fun &fun) : m_impl(std::make_unique<FunctorHandler<Functor<ResultType, TList>, Fun>>(fun))
{
}
