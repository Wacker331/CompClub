#include "CompClub.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        std::cout << "<filename> - file with actions" << std::endl;
    }
    else
    {
        std::ifstream InputFile(argv[1]);
        if (InputFile.is_open())
        {
            Club MainClub(InputFile);
            std::string Line;
            while (std::getline(InputFile, Line))
                MainClub.ParseActionLine(Line);
            MainClub.Close();
        }
        else
        {
            std::cout << "Can't open " << argv[1] << std::endl;
        }
    }
}