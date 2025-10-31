#pragma once

#include "algorithms/typelist.h"

template <typename R>
class FunctorImplBase
{
public:
	using ResultType = R;

	virtual ~FunctorImplBase() = default;
	virtual FunctorImplBase *Clone() const = 0;
};

#define DEFINE_CLONE_FUNCTOR_IMPL(Class)                                                                                                   \
	FunctorImplBase<ResultType> *Clone() const override                                                                                    \
	{                                                                                                                                      \
		return new Class(*this);                                                                                                           \
	}

template <typename ResultType, typename TList>
class FunctorImpl;

template <typename ResultType>
class FunctorImpl<ResultType, NullType> : public FunctorImplBase<ResultType>
{
public:
	virtual ResultType operator()() = 0;
};

template <typename ResultType, typename P1>
class FunctorImpl<ResultType, TYPELIST_1(P1)> : public FunctorImplBase<ResultType>
{
public:
	virtual ResultType operator()(P1) = 0;
};

template <typename ResultType, typename P1, typename P2>
class FunctorImpl<ResultType, TYPELIST_2(P1, P2)> : public FunctorImplBase<ResultType>
{
public:
	virtual ResultType operator()(P1, P2) = 0;
};

template <typename ResultType, typename P1, typename P2, typename P3>
class FunctorImpl<ResultType, TYPELIST_3(P1, P2, P3)> : public FunctorImplBase<ResultType>
{
public:
	virtual ResultType operator()(P1, P2, P3) = 0;
};
