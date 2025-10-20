#include "append.h"
#include "erase-all.h"
#include "erase-duplicates.h"
#include "erase.h"
#include "indexed-access.h"
#include "replace.h"
#include "search.h"
#include "size.h"
#include <typeinfo>

int main()
{
  using MyTypes = TYPELIST_5(int, char, double, char, int);

  static_assert(Size<MyTypes>::value == 5);
  static_assert(std::is_same_v<TypeAt<MyTypes, 2>::Result, double>);

  using AfterErase = Erase<MyTypes, char>::Result;
  static_assert(Size<AfterErase>::value == 4);

  using AfterEraseAll = EraseAll<MyTypes, char>::Result;
  static_assert(Size<AfterEraseAll>::value == 3);

  using AfterEraseDuplicates = EraseDuplicates<MyTypes>::Result;
  static_assert(Size<AfterEraseDuplicates>::value == 3);

  using AfterAppend = Append<MyTypes, long>::Result;
  static_assert(Size<AfterAppend>::value == 6);

  using AfterReplace = Replace<MyTypes, char, short>::Result;
  static_assert(std::is_same_v<TypeAt<AfterReplace, 1>::Result, short>);
  static_assert(indexOf<MyTypes, char>::value == 1);
}
