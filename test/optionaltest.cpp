
#include <cassert>

#include <iostream>
#include <string>
#include <vector>

#include<aq/optional.hpp>

using std::cout;

class noncopyable
{
    noncopyable(noncopyable const&); // not copyable but assignable
    noncopyable& operator=(noncopyable const&); // not copyable but assignable

    protected:
    noncopyable() {}
};

class CopyableClass
{
    int x;
    char* p;

    public:
    CopyableClass()
        : p(new char('c'))
    {}

    CopyableClass(CopyableClass const& other)
        : x(other.x), p(new char(*other.p))
    {}

    CopyableClass& operator=(CopyableClass other)
    {
        swap(*this, other);
        return *this;
    }

    friend void swap(CopyableClass& first, CopyableClass& second)
    {
        using std::swap;

        swap(first.x, second.x);
        swap(first.p, second.p);
    }

    ~CopyableClass()
    {
        delete p;
    }
};

class NonCopyableClass : private noncopyable
{
    public:
    int x;
    NonCopyableClass()  {}
};

// this class has a converting constructor for integer, so should be ok to assign directly to optional<int>
class ConvertableToInt
{
    int i_;
    public:
    ConvertableToInt(int x) : i_(x) {}
    operator int() const
    {
        return i_;
    }
};


template<typename T>
void optional_test_operators(aq::optional<T>& op, bool isinited)
{
    if(isinited)
    {
        assert(op);
        assert(!(!op)); // eugh!?!

    } else
    {
        assert(!op);
        assert(!(op)); // eugh again
    }

}

void maintest()
{
    cout << "c++ version " << __cplusplus << '\n';
    aq::optional<int> i;
    aq::optional<int> i2(i);
    aq::optional<int> i3;
    i3 = i;

    aq::optional<int> j(2);

    aq::optional<int> k(j); // invoke copy ctor on valid optional object

    aq::optional<int> l;
    aq::optional<int> m;
    aq::optional<int> n;

    l = k;  // invoke copy assignment operator for optional
    m = 5;  // invoke copy assignement from templated type

    ConvertableToInt icanbeint(15);
    n = ConvertableToInt(5);
    n = icanbeint;

    aq::optional<std::string> s;

    // a copyable class with some internal memory management to ensure dtors are called correctly

    CopyableClass cpclass;

    // default construct
    aq::optional<CopyableClass> optcopyempty;
    // construct with type param
    aq::optional<CopyableClass> optcopyclass(cpclass);
    // copy construct
    aq::optional<CopyableClass> optcopyclass2(optcopyclass);
    // copy construct with rvalue
    //aq::optional<CopyableClass> optcopyclass3(CopyableClass(); <-- vexing parse

    // create objects for assignment
    aq::optional<CopyableClass> optcopyclass3, optcopyclass4, optcopyclass5;

    // assign from templated type
    optcopyclass3 = cpclass;
    // assign from rvalue
    optcopyclass4 = CopyableClass();
    // assign from other optional
    optcopyclass5 = optcopyclass4;

    // a non copyable class which should not be permitted by optionals requirement that the type be copyable.

    // this should fail to compile and should not need an assignment to the variable to be made to break the compile
    // to achieve this will require some metaprogramming using `enable_if` to eliminate overloads for non copyable types.
    //aq::optional<NonCopyableClass> noncopyclass;

    //NonCopyableClass ncopy;
    //aq::optional<NonCopyableClass> noncopyclass2(ncopy);


    optional_test_operators(i, false);
    optional_test_operators(i2, false);
    optional_test_operators(i3, false);

    optional_test_operators(j, true);
    optional_test_operators(k, true);
    optional_test_operators(l, true);
    optional_test_operators(m, true);

    cout << "j val=" << *j << '\n';
    cout << "k val=" << *k << '\n';
    cout << "l val=" << *l << '\n';
    cout << "m val=" << *m << '\n';
    cout << "n val=" << *n << '\n';

    s = "pizza and chips";
    cout << "s val=" << *s << '\n';

    s->assign("ham & pineapple");
    cout << "(again) s val=" << *s << '\n';


    typedef std::vector<aq::optional<std::string> >  vec_opt_string;
    vec_opt_string headers(10);
    size_t ind = 0;
    for(; ind < 10; ind += 2)  headers[ind] = "cheese";

    vec_opt_string::iterator it;

    for( it = headers.begin(); it != headers.end(); ++it)
    {
        if(*(it))
            cout << *(*it) << '\n';
        else
            cout << "skip uninitialised\n";
    }

    // use << overload for optional
    for( it = headers.begin(); it != headers.end(); ++it)
    {
        if(*it)
            cout << *it << '\n';
        else
            cout << "skip uninitialised\n";
    }

}

int main()
{
     maintest();

}