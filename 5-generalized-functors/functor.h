#pragma once

#include "functor-handler.h"
#include "functor-impl.h"
#include "typeat-nonstrict.h"

#include <memory>

template <typename R, typename TList>
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

	ResultType operator()()
	{
		return (*m_impl)();
	}

	ResultType operator()(Param1 p1)
	{
		return (*m_impl)(p1);
	}

	ResultType operator()(Param1 p1, Param2 p2)
	{
		return (*m_impl)(p1, p2);
	}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3)
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
