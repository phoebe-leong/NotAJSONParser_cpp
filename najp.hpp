#pragma once
#include <fstream>
#include <vector>
#include <type_traits>
#include <map>

#define NAJP_OK 0
#define NAJP_TITLE_ALREADY_IN_USE -1
#define NAJP_SUBCLASS_NOT_CURRENT -2

#define NAJP_VERSION "2.0"

class najp
{
    private:
        void sys(const std::string& string)
        {
            system(string.c_str());
        }

        struct data
        {
            bool comma, isubclass, isubclasstart;
            int parentsubclasses;
            std::vector<std::string> titles;
            std::vector<std::string> arraytitles;
            std::vector<std::string> subclasstitles;

            void reset()
            {
                comma = false;
                isubclass = false;
                isubclasstart = false;

                parentsubclasses = 0;

                titles.clear();
                arraytitles.clear();
                subclasstitles.clear();
            }
        };

        bool empty, closed;
        std::string filename;

        std::ofstream json;
        data d;
    public:
        class map
        {
            private:
                std::vector<std::string> strings;
                std::vector<bool> bools;
            public:
                inline size_t size() const { return strings.size(); }
                std::pair<std::string, bool> operator[](const int& index) const
                {
                    std::pair<std::string, bool> pair;
                    if (index < size())
                    {
                        pair.first = strings[index];
                        pair.second = bools[index];
                    }

                    return pair;
                }

                inline void push_back(const std::pair<std::string, bool>& pair)
                {
                    strings.push_back(pair.first);
                    bools.push_back(pair.second);
                }
        };

        void open(const std::string file)
        {
            closed = false;

            {
                std::ifstream input_stream(file);

                if (input_stream.peek() != std::ifstream::traits_type::eof()) empty = false;
                else empty = true;

                input_stream.close();
            }

            if (!empty)
            {
                {
                    // If the file does not exist, opening it first in std::ofstream creates it
                    std::ofstream out(".2" + file);
                    out.close();
                }

                json.open(".2" + file);
                filename = file;
            }
            else
            {
                json.open(file);
                json << "{\n";
            }
        }

        int add_array(const std::string title, const map values)
        {
            // Checking that the title of the array isn't already being used
            for (int i = 0; i < d.arraytitles.size(); i++)
            {
                if (title == d.arraytitles[i]) return NAJP_TITLE_ALREADY_IN_USE;
            }
            d.arraytitles.push_back(title);

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
            for (int i = 0; i < values.size(); i++)
            {
                const std::pair<std::string, bool> pair = values[i];

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
                if (pair.second)
                {
                    json << "\t\t\"" << pair.first << "\"";
                } else
                {
                    json << "\t\t" << pair.first;
                }
            }
            json << "\n";

            if (d.isubclass)
            {
                for (int j = 0; j != d.parentsubclasses; j++)
                {
                    json << "\t";
                }
            }
            json << "\t]";
            return NAJP_OK;
        }

        template<typename T>
        int add_element(const std::string title, const T value)
        {
            for (int i = 0; i != d.titles.size(); i++)
            {
                if (title == d.titles[i])
                {
                    return NAJP_TITLE_ALREADY_IN_USE;
                }
            }
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
                for (int i = 0; i != d.parentsubclasses; i++)
                {
                    json << "\t";
                }
            }

            json << "\t\"" << title << "\" : ";

            // 'constexpr if' is not used because that is a C++17 only thing and I don't want there to be reliance on any particular
            // standard
            {
                constexpr bool str = std::is_convertible<T, std::string>::value;
                constexpr bool ch = std::is_convertible<T, const char*>::value;
                if (str || ch)
                {
                    json << "\"" << value << "\"";
                } else
                {
                    json << value;
                }
            }
            return NAJP_OK;
        }

        int add_element(const std::string title)
        {
            for (int i = 0; i != d.parentsubclasses; i++)
            {
                if (title == d.titles[i])
                {
                    return NAJP_TITLE_ALREADY_IN_USE;
                }
            }
            d.titles.push_back(title);
            if (d.comma)
            {
                d.comma = true;
            } else
            {
                json << ",\n";
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

            return NAJP_OK;
            }

        int add_subclass(const std::string title)
        {
            for (int i = 0; i != d.titles.size(); i++)
            {
                if (title == d.titles[i])
                {
                    return NAJP_TITLE_ALREADY_IN_USE;
                }
            }
            for (int i = 0; i != d.subclasstitles.size(); i++)
            {
                if (title == d.subclasstitles[i])
                {
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
                for (int i = 0; i != d.parentsubclasses; i++)
                {
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

        int close_subclass()
        {
            if (!d.isubclass)
            {
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
            if (!closed)
            {
                json << "\n}";
                json.close();

                if (!empty)
                {
                    std::ofstream out(".3" + filename);
                    std::ifstream in1(filename);
                    std::ifstream in2(".2" + filename);

                    // Adds the first file to the buffer file
                    while (!in1.eof())
                    {
                        std::string line;
                        getline(in1, line);

                        if (line != "}")
                        {
                            if (line != "{") out << "\n";
                            out << line;
                        }
                    }
                    in1.close();

                    // Joins the two files by adding a ',' to the end of the buffer file then continues as normal printing the second file to it
                    {
                        std::string line;
                        getline(in2, line);

                        out << ",\n" << line << "\n";
                    }
                    while (!in2.eof())
                    {
                        std::string line;
                        getline(in2, line);

                        out << line;
                        if (line != "}") out << "\n";
                    }
                    in2.close();
                    out.close();

                    // Overwrites the previous contents with the new ones - prints the buffer file onto the user's initial one
                    out.open(filename);
                    in1.open(".3" + filename);

                    while (!in1.eof())
                    {
                        std::string line;
                        getline(in1, line);

                        out << line;
                        if (line != "}") out << "\n";
                    }
                    out.close();
                    in1.close();

                    // Delete the second file and the buffer file
                    #if defined(__linux__) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
                        sys("rm .2" + filename);
                        sys("rm .3" + filename);
                    #elif defined(_WIN32) || defined(_WIN16) || defined(_WIN64)
                        sys("delete /f .2" + filename);
                        sys("delete /f .3" + filename);
                    #endif
                }
                d.reset();
                closed = true;
            }
        }

        najp() {}
        najp(const std::string& file) { open(file); }

        ~najp() { close(); }
};
