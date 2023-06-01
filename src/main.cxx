/* headers*/

#include "head.hxx"

#include "main.hxx"

/* content */

_NAMESPACE_ENTER

int main(int argc, const char** argv)
{
    std::clog << "hello world" << std::endl;
    std::clog << std::endl;
    std::clog << "my name is" << " " << _NAME_STR << std::endl;
    std::clog << "my vnum is" << " " << _VNUM_STR << std::endl;
    std::clog << std::endl;
    std::clog << "nice to meet you" << std::endl;
    
    return _ERROR_NONE;
}

_NAMESPACE_LEAVE

#ifdef _TYPE_RUN

int main(int argc, const char** argv)
{
    _NAMESPACE::main(argc, argv);
}

#endif
