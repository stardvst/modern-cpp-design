template <typename T, typename U>
class Conversion
{
	using Small = char;
	class Big
	{
		char dummy[2];
	};

	static Small Test(const U &);
	static Big Test(...);

	static T MakeT();

public:
	enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
	enum { sameType = false };
};

template <typename T>
class Conversion<T, T>
{
public:
	enum { exists = true, sameType = true };
};

template <typename T>
class Conversion<T, void>
{
public:
	enum { exists = false, sameType = false };
};

template <typename T>
class Conversion<void, T>
{
public:
	enum { exists = false, sameType = false };
};
