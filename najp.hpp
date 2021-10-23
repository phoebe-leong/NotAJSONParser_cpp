#pragma once
#include <fstream>
#include <vector>
#include <type_traits>
#include <map>

#define NAJP_OK 0
#define NAJP_TITLE_ALREADY_IN_USE -1
#define NAJP_SUBCLASS_NOT_CURRENT -2

struct najp_data {
    bool comma, isubclass, isubclasstart;
    int parentsubclasses;
    std::vector<std::string> titles;
    std::vector<std::string> subclasstitles;

    najp_data() {
        comma = false;
        parentsubclasses = 0;
    }
};

class najp {
    private:
        std::ofstream json;
        najp_data d;
    public:
        typedef std::map<std::string, bool> najp_array;

        void open(const std::string file)
        {
            json.open(file);
            json << "{\n";
        }

        int addarray(const std::string title, const najp_array values)
        {
            int i = 0;
            for (const auto& [value, istring] : values)
            {
                if (i == d.titles.size()) { break; }
                if (value == d.titles[i]) { return NAJP_TITLE_ALREADY_IN_USE; }
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
                for (int j = 0; j != d.parentsubclasses; j++)
                {
                    json << "\t";
                }
            }
            json << "\t]";
            return NAJP_OK;
        }

        template<typename T>
        int addelement(const std::string title, const T value)
        {
            for (int i = 0; i != d.titles.size(); i++)
            {
                if (title == d.titles[i])
                {
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
            return NAJP_OK;
        }

        int addelement(const std::string title)
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

            return NAJP_OK;
            }

        int addsubclass(const std::string title)
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

        int closesubclass()
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
            json << "\n}";
            json.close();
        }
};
