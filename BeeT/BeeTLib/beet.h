#ifndef __BEET_H__
#define __BEET_H__

#ifdef BEET_EXPORTS
	#define BEET_API __declspec(dllexport)
#else
	#define BEET_API __declspec(dllimport)
#endif // BEET_EXPORTS

namespace BeeT
{
	class TestClass
	{
	public:
		static BEET_API int AddFour(int number); // This is a comment

		// This is a comment
		static BEET_API int AddFive(int number);
	};
}
#endif // !__BEET_H__
