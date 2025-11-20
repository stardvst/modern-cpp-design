#pragma once

struct AllowConversion
{
	enum { allow = true };
};

struct DisallowConversion
{
	DisallowConversion() = default;

	DisallowConversion(const AllowConversion &)
	{
	}

	enum { allow = false };
};
