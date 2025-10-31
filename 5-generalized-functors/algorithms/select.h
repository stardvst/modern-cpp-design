#pragma once

template <bool flag, typename T, typename U>
struct Select
{
	using Result = T;
};

template <typename T, typename U>
struct Select<false, T, U>
{
	using Result = U;
};
