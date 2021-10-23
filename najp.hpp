#pragma once
#include <fstream>
#include <vector>
#include <type_traits>
#include <iostream>
#include <map>

#if defined(__unix) || defined(__unix__) || defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    #include <unistd.h>
#elif defined(_WIN32)
    #include <windows.h>
    #define TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
#endif

#include "termcolor/termcolor.hpp"

#define NAJP_OK 0
#define NAJP_TITLE_ALREADY_IN_USE -1
#define NAJP_SUBCLASS_NOT_CURRENT -2

#define NAJP_VERSION 1.2

class najp
{
    private:
        struct data
        {
            bool comma, isubclass, isubclasstart;
            int parentsubclasses;
            std::vector<std::string> titles;
            std::vector<std::string> subclasstitles;

            data() {
                comma = false;
                parentsubclasses = 0;
            }
        };

        std::ofstream json;
        data d;
    public:
        typedef std::map<std::string, bool> array;

        void open(const std::string file)
        {
            json.open(file);
            json << "{\n";

            #if __has_include(<unistd.h>)
                sleep(1);
            #else
                sleep(1000);
            #endif

            std::cout << "  " << termcolor::underline << "File \"" << file << "\" opened ..\n" << termcolor::reset;
        }

        int addarray(const std::string title, const array values)
        {
            int i = 0;
            for (const auto& [value, istring] : values)
            {
                if (i == d.titles.size()) { break; }
                if (value == d.titles[i])
                {
                    #if __has_include(<unistd.h>)
                        sleep(1);
                    #else
                        sleep(1000);
                    #endif
                    std::cout << termcolor::red << "✗ Array \"" << title << "\" finished with one error: NAJP_TITLE_ALREADY_IN_USE\n" << termcolor::reset;
                    return NAJP_TITLE_ALREADY_IN_USE;
                }
            }
            i = 0;
            d.titles.push_back(title);

            if (!d.comma)
            {
                d.comma = true;
            } else
            {
                json << ",\n";
            }

            if (d.isubclass)
            {
                d.isubclasstart = false;
                for (int j = 0; j != d.parentsubclasses; j++)
                {
                    json << "\t";
                }
            }

            json << "\t\"" << title << "\" : [\n";
            for (const auto& [value, istring] : values)
            {
                if (i != 0)
                {
                    json << ",\n";
                }
                if (d.isubclass)
                {
                    for (int j = 0; j != d.parentsubclasses; j++)
                    {
                        json << "\t";
                    }
                }
                if (istring)
                {
                    json << "\t\t\"" << value << "\"";
                } else
                {
                    json << "\t\t" << value;
                }
                i++;
            }
            json << "\n";

            if (d.isubclass)
            {
                for (int j = 0; j != d.parentsubclasses; j++) {
                    json << "\t";
                }
            }
            json << "\t]";

            #if __has_include(<unistd.h>)
                sleep(1);
            #else
                sleep(1000);
            #endif
            std::cout << termcolor::green << "✓ Array \"" << title << "\" finished with no errors\n" << termcolor::reset;
            return NAJP_OK;
        }

        template<typename T>
        int addelement(const std::string title, const T value)
        {
            for (int i = 0; i != d.titles.size(); i++)
            {
                if (title == d.titles[i])
                {
                    #if __has_include(<unistd.h>)
                        sleep(1);
                    #else
                        sleep(1000);
                    #endif
                    std::cout << termcolor::red << "✗ Element \"" << title << "\" finished with one error: NAJP_TITLE_ALREADY_IN_USE\n" << termcolor::reset;
                    return NAJP_TITLE_ALREADY_IN_USE;
                }
            }
            d.titles.push_back(title);
            if (d.comma)
            {
                json << ",\n";
            } else
            {
                d.comma = true;
            }

            if (d.isubclass)
            {
                d.isubclasstart = false;
                for (int i = 0; i != d.parentsubclasses; i++)
                {
                    json << "\t";
                }
            }

            json << "\t\"" << title << "\" : ";
            if constexpr (std::is_convertible<T, std::string>::value || std::is_convertible<T, const char*>::value)
            {
                json << "\"" << value << "\"";
            } else
            {
                json << value;
            }

            #if __has_include(<unistd.h>)
                sleep(1);
            #else
                sleep(1000);
            #endif
            std::cout << termcolor::green << "✓ Element \"" << title << "\" finished with no errors\n" << termcolor::reset;
            return NAJP_OK;
        }

        int addelement(const std::string title)
        {
            for (int i = 0; i != d.parentsubclasses; i++)
            {
                if (title == d.titles[i])
                {
                    #if __has_include(<unistd.h>)
                    sleep(1);
                    #else
                        sleep(1000);
                    #endif
                    std::cout << termcolor::red << "✗ Element \"" << title << "\" finished with one error: NAJP_TITLE_ALREADY_IN_USE\n" << termcolor::reset;
                    return NAJP_TITLE_ALREADY_IN_USE;
                }
            }
            d.titles.push_back(title);
            if (d.comma)
            {
                json << ",\n";
            } else
            {
                d.comma = true;
            }

            if (d.isubclass)
            {
                d.isubclasstart = false;
                for (int i = 0; i != d.parentsubclasses; i++)
                {
                    json << "\t";
                }
            }
            json << "\t\"" << title << "\" : null";

            #if __has_include(<unistd.h>)
                sleep(1);
            #else
                sleep(1000);
            #endif
            std::cout << termcolor::green << "✓ Element \"" << title << "\" finished with no errors\n" << termcolor::reset;
            return NAJP_OK;
        }

        int addsubclass(const std::string title)
        {
            for (int i = 0; i != d.titles.size(); i++)
            {
                if (title == d.titles[i])
                {
                    #if __has_include(<unistd.h>)
                        sleep(1);
                    #else
                        sleep(1000);
                    #endif
                    std::cout << termcolor::red << "✗ Subclass \"" << title << "\" finished with one error: NAJP_TITLE_ALREADY_IN_USE\n" << termcolor::reset;
                    return NAJP_TITLE_ALREADY_IN_USE;
                }
            }
            for (int i = 0; i != d.subclasstitles.size(); i++)
            {
                if (title == d.subclasstitles[i])
                {
                    #if __has_include(<unistd.h>)
                        sleep(1);
                    #else
                        sleep(1000);
                    #endif
                    std::cout << termcolor::red << "✗ Subclass \"" << title << "\" finished with one error: NAJP_TITLE_ALREADY_IN_USE\n" << termcolor::reset;
                    return NAJP_TITLE_ALREADY_IN_USE;
                }
            }
            d.subclasstitles.push_back(title);

            if (d.comma)
            {
                json << ",\n";
            }

            if (d.isubclass)
            {
                for (int i = 0; i != d.parentsubclasses; i++) {
                    json << "\t";
                }
            }

            json << "\t\"" << title << "\" : {\n";

            d.parentsubclasses++;
            d.isubclass = true;
            d.isubclasstart = true;
            d.comma = false;
            return NAJP_OK;
        }

        int closesubclass()
        {
            if (!d.isubclass)
            {
                #if __has_include(<unistd.h>)
                    sleep(1);
                #else
                    sleep(1000);
                #endif
                std::cout << termcolor::red << "✗ Subclass closure finished with one error: NAJP_SUBCLASS_NOT_CURRENT\n" << termcolor::reset;
                return NAJP_SUBCLASS_NOT_CURRENT;
            } else if (!d.isubclasstart)
            {
                json << "\n";
            }

            for (int i = 0; i != d.parentsubclasses; i++)
            {
                json << "\t";
            }
            json << "}";
            if (d.parentsubclasses == 0)
            {
                d.isubclass = false;
            }
            d.isubclasstart = false;
            d.parentsubclasses--;

            return NAJP_OK;
        }

        void close()
        {
            json << "\n}";
            json.close();
            #if __has_include(<unistd.h>)
                sleep(1);
            #else
                sleep(1000);
            #endif
            std::cout << "  " << termcolor::underline << "File closed ..\n";
            #if __has_include(<unistd.h>)
                sleep(1);
            #else
                sleep(1000);
            #endif
        }

        najp(const std::string file = "") { if (file != "") open(file); }
        ~najp() { close(); }
};
