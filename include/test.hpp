enum class Result {
	FAILED = false,
	PASS = true
};

template <class T, long n= 100>
class Test {
	bool(T::*fns[n])();
	bool (*solo_tests[n])();
	long len = 0;
	long len_solo = 0;
public:

	void add(bool(T:: *fn)()) {
		fns[len++] = fn;
	}
	template <class Z>
	void add(Z fn) {
		void *fn_ = (void*)&fn;
		solo_tests[len_solo++] = (bool(*)())fn_;
	}
	void add(bool(*fn)()) {
		void *fn_ = (void*)&fn;
		solo_tests[len_solo++] = fn_;
	}
	template <class ...U>
	Result emit_all(U ...args) {
		T obj(args...);
		Result out = Result::PASS;
		for (long i = 0; i < len; i++) {
			bool (T::*fn)() = fns[i];
			bool res= (obj.*fn)();
			if (!res) {
				out = Result::FAILED;
			}
		}
		return out;
	}
};

#define EXPECT(condition) \
	if (!(condition)) {\
		printf("\t\e[91m[" #condition "]\e[0m: failed\n");\
		return false;						\
	} else\
		printf("\t\e[92m[" #condition "]\e[0m: passed\n");\


#define IS_NULL(value) \
	if (!(value == null || (size_t)value == (size_t)-1)) {\
		printf("\t\e[91m[" #value ".is_null()]\e[0m: failed\n");\
		return false;						\
	} else\
		printf("\t\e[92m[" #value ".is_null()]\e[0m: passed\n");\


#define IS_NOT_NULL(value) \
	if (value == null || (size_t)value == (size_t)-1) {\
		printf("\t\e[91m[" #value ".is_not_null()]\e[0m: failed\n");\
		return false;						\
	} else\
		printf("\t\e[92m[" #value ".is_not_null()]\e[0m: passed\n");\

