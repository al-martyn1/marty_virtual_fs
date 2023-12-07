#if defined(_MSC_VER)

    #pragma warning(push)

    // MSVC 2022+
    #if _MSC_VER>=1930
        // _MSC_VER and other predefined macros - https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170
        #pragma warning(disable:5267) // warning C5267: definition of implicit copy constructor for 'i_base' is deprecated because it has a user-provided destructor
    #endif

#endif

