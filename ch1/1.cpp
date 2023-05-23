//---------------------------------------------------------
// 1: template type deduction
//---------------------------------------------------------


template<typename T>
void f(ParamType param);
f(expr);

// expr -> deduce T
//     \-> deduce ParamType (T+const+ref...)



// In general, the type deduced for T is dependent not just
// on the type of expr, but also on the form of ParamType.
//
// 1. ParamType == pointer or reference type but not
// universal reference
//
// 2. ParamType == universal reference
//
// 3. ParamType == neither a pointer nor a reference


// --------------------------------------------------------


//
// case 1: ParamType: ref/ptr (except universal ref (T&& param))
//


// 1. if expr == ref, ignore ref part.
// 2. pattern-match(expr, ParamType) -> T
// NOTE: constness of the expr becomes part of the type deduced for T


template<typename T>
void f(X param);
int x = 27;                  // int
const int cx = x;            // const int
const int& rx = x;           // ref to const int
const int *px = &x;          // ptr to const int


// X: T&
f(x);       // -> T: int            ParamType: int&
f(cx);      // -> T: const int      ParamType: const int&
f(rx);      // -> T: const int      ParamType: const int&


// X: const T&
f(x);       // -> T: int            ParamType: const int&
f(cx);      // -> T: int            ParamType: const int&
f(rx);      // -> T: int            ParamType: const int&


// X: T*
f(&x);      // -> T: int            ParamType: int*
f(px);      // -> T: const int      ParamType: const int*


// X: const T*
f(&x);      // -> T: int            ParamType: const int*
f(px);      // -> T: int            ParamType: const int*


// --------------------------------------------------------


//
// case 2: ParamType == universal ref
//


// 1. expr == lvalue -> T == ParamType == lvalue-ref
// 2. expr == rvalue -> case 1


template<typename T>
void f(X param);
int x = 27;                  // int
const int cx = x;            // const int
const int& rx = x;           // ref to const int


// X: T&&
f(x);               // T: int&          ParamType: int&
f(cx);              // T: const int&    ParamType: const int&
f(rx);              // T: const int&    ParamType: const int&
f(27);              // T: int           ParamType: int&&


// --------------------------------------------------------


//
// case 3: ParamType == neither a ptr nor a ref (pass-by-value)
//

// param will be a new object (copy).


// 1. if expr == ref -> ignore ref part
// 2. if expr == const/volatile -> ignore const
// SPECIAL CASE:
//      - expr == const ptr to const object -> T == ParamType == ptr to const object
//      - expr == array
//      - expr == function ptr


template<typename T>
void f(X param);
int x = 27;                                     // int
const int cx = x;                               // const int
const int& rx = x;                              // ref to const int
const char* const ptr = "Fun with pointers";    // const ptr to const char


// X: T
f(x);               // T: int           ParamType: int
f(cx);              // T: int           ParamType: int
f(rx);              // T: int           ParamType: int
f(ptr);             // T: const char    ParamType: cosnt char*


// --------------------------------------------------------


//
// array arguments
//


// NOTE: following two are equivalent
void myFunc(int param[]);
void myFunc(int* param);


template<typename T>
void f(T param);
const char name[] = "J. P. Briggs"; // name = const char[13]
const char * ptrToName = name;      // array "decays" to ptr

f(name);                   // name: array, T: const char*


// Although functions can’t declare parameters that are
// truly arrays, they can declare parameters that are
// references to arrays!

template<typename T>
void f(T& param);

f(name);  // T: const char[13]      ParamType: const char (&)[13]




