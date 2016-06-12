#include <iostream>

/*
  
Some tests related to the empty base class optimization (EBCO)
http://www.informit.com/articles/article.aspx?p=31473&seqNum=2

Currently VS2015, update 2 does not inhibit EBCO in case of class derived from empty and also
contains an empty non-static member.
They are aware of it and it will be fixed in the next update  
https://blogs.msdn.microsoft.com/vcblog/2016/03/30/optimizing-the-layout-of-empty-base-classes-in-vs2015-update-2-3/

*/


int
main()
{
    int result(0);

    // Size unequal to 0 (typically 1) otherwise pointer arithmetic would not work
    class Empty 
    { 

    };

    std::cout << "Size of empty class : " << sizeof(Empty) << std::endl;
    if (sizeof(Empty) != 1)
    {
        std::cerr << "Size of empty class is not optimal" << std::endl;
        result = 1;
    }

    // Virtual function pointer table added 
    // Size should be equal to sizeof(void*)
    class EmptyInterface
    {
        virtual ~EmptyInterface() {}
    };

    std::cout << "Size of empty interface class : " << sizeof(EmptyInterface) << std::endl;
    if (sizeof(EmptyInterface) != sizeof(void*))
    {
        std::cerr << "Size of empty interface class is not optimal" << std::endl;
        result = 1;
    }


    // Size should still be 1
    class DerivedEmpty : public Empty   
    {

    };

    std::cout << "Size of derived class from empty class : " << sizeof(DerivedEmpty) << std::endl;
    if (sizeof(DerivedEmpty) != 1)
    {
        std::cerr << "Size of empty class is not optimal" << std::endl;
        result = 1;
    }


    // Virtual function pointer table added 
    // Size should be equal to sizeof(void*)
    class DerivedEmptyInterface : public Empty
    {
        virtual ~DerivedEmptyInterface() {}
    };

    std::cout << "Size of derived empty virtual class : " << sizeof(DerivedEmptyInterface) << std::endl;
    if (sizeof(DerivedEmptyInterface) != sizeof(void*))
    {
        std::cerr << "Size of derived empty interface class is not optimal" << std::endl;
        result = 1;
    }


    // Two subobjects of the same type are required to have different addresses within the object representation of the most derived type.
    class DerivedEmptyEmptyMember : public Empty
    {
    public:
        Empty                       mEmpty;
    };
    std::cout << "Size of derived from empty class containing empty base class as member : " << sizeof(DerivedEmptyEmptyMember) << std::endl;
    if (sizeof(DerivedEmptyEmptyMember) != 2)
    {
        std::cerr << "Size of derived from empty class containing empty base class as member is not correct" << std::endl;
        result = 1;
    }

    DerivedEmptyEmptyMember  deem;
    DerivedEmptyEmptyMember* p_deem(&deem);
    Empty*                   p_deem_empty_base(&deem);
    Empty*                   p_deem_empty_member(&deem.mEmpty);

    std::cout << "Address of derived class from empty : " << p_deem << std::endl;
    std::cout << "Address of empty base               : " << p_deem_empty_base << std::endl;
    std::cout << "Address of empty member             : " << p_deem_empty_member << std::endl;
    if (p_deem_empty_base == p_deem_empty_member)
    {
        std::cerr << "Address of empty as base class and as data member are equal" << std::endl;
        result = 1;
    }

    // Size should be 2
    class EmptyContainer
    {
        Empty                   mEmpty1;
        Empty                   mEmpty2;
    };
    std::cout << "Size of class containing two empty non-static data members : " << sizeof(EmptyContainer) << std::endl;
    if (sizeof(EmptyContainer) != 2)
    {
        std::cerr << "Size of class containing two empty non static members is not optimal" << std::endl;
        result = 1;
    }

    return result;
}