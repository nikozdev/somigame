/* headers */

#include "main.hxx"
#include "stbi.hxx"

#include <iostream>

/* content */

_NAMESPACE_ENTER

int main(int argc, const char** argv)
{
    stbi_set_flip_vertically_on_load(true);
    for (auto iter = 1; iter < argc; iter++)
    {
        auto fpath = argv[iter];
        int sizew = _ZERO, sizeh = _ZERO;
        int pixel_size = 3;
        unsigned char*pixel_data = stbi_load(fpath, &sizew, &sizeh, &pixel_size, 3);
        size_t msize = sizew * sizeh * pixel_size;
        if (msize > _ZERO)
        {
            std::cout << "--[[loaded]]--" << std::endl;
            std::cout << "[fpath]=" << fpath << ";" << std::endl;
            std::cout << "[sizes]={" <<
                "w=" << sizew << ";" <<
                "h=" << sizeh << ";" <<
                "};" <<
                std::endl;
            std::cout << "[msize]=" << msize << ";" << std::endl;
            std::cout << "[pixel_size]=" << pixel_size << ";" << std::endl;
            std::cout << "[pixel_data]={" << std::endl;
            for (auto itery = 0; itery < sizeh; itery++)
            {
                for (auto iterx = 0; iterx < sizew; iterx++)
                {
                    size_t step = ((itery * sizew + iterx) * pixel_size);
                    char char_out = static_cast<char>((pixel_data[step] % 95) + 33);
                    char_out = std::abs(char_out);
                    std::cout << char_out << ' ';
                }
                std::cout << std::endl;
            }
            std::cout << "};" << std::endl;
            stbi_image_free(pixel_data);
        }
        else
        {
            std::cout << "--[[failed]]--" << std::endl;
        }
    }
    return _ERROR_NONE;
}

_NAMESPACE_LEAVE

/* actions */

#ifdef _TYPE_RUN
int main(int argc, const char** argv)
{
    for (auto iter = 0; iter < argc; iter++)
    {
        std::cout << argv[iter] << std::endl;
    }
    return _NAMESPACE::main(argc, argv);
}
#endif
