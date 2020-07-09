/*
    Header-only argument parser

    ArgParser AP;
    AP.AddOptions(
        {{ "-e", "--example", "This is a mandatory example flag", true },}
    );


    //parse failed
    if(!AP.Parse(argx, argv))
	{
		return 0;
	}

     --examplebool
    connect = AP.Get<bool>("examplebool");

    --exampleInt 5
    if(AP.IsSet("exampleInt"))
		example = AP.Get<int>("fillpercentage");
	else
		example = 50;

    --exampleVector 1 2 3 4
    if(AP.IsSet("exampleVector"))
		dimensions = AP.GetVector<int>("dimensions");
	else
		dimensions = {60, 50};
*/
#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>

#include <algorithm>    // std::find_if
#include <sstream>      //istringstream


class ArgParser
{
    struct Arg
    {
        std::string _shortName;
        std::string _fullName;
        std::string _description;
        bool _required;
    };


    public:

        //=================== C O N S T R U C T O R S ==========================================

        ArgParser() : usage(), description(), exe_name() {}
        ArgParser(const std::string& u, const std::string& d) : 
        usage(u), description(d), exe_name() {}

        ArgParser(const std::string& u, 
            const std::string& d, const std::string& n) : 
            usage(u), description(d), exe_name(n) {}
        

        //=========================================================

        void AddOption(const Arg& argument)
        {
            options.push_back(argument);
            if(!argument._fullName.empty())
            {
                short_to_full[_ltrim_copy(argument._shortName, [](char c) { return c != (char)'-'; })] =
                 _ltrim_copy(argument._fullName, [](char c) { return c != (char)'-'; });
            }
        }

        void AddOptions(const std::vector<Arg>& arguments)
        {
            for ( const auto& arg : arguments )
            {
                AddOption(arg);
            }
        }

        //=============================================================

        bool Parse(int argc, char* argv[])
        {
            exe_name = argv[0];

            if (argc < 2)
                return true;

            std::string flag;
            std::vector<std::string> flagArgs;

            //lambda function to help with processing and parsing
            auto AddArgument = [&flag, &flagArgs, this]()
            {
                if(flag.empty())
                {
                    //dump all previous vals
                    flagArgs.clear();
                    return;
                }

                _ltrim(flag, [](char c) { return c != (char)'-'; });
                std::string key;
                if(short_to_full.find(flag) != short_to_full.end())
                {
                    key = short_to_full[flag];
                }
                else
                {
                    key = flag;
                }

                std::cout<<key<<std::endl;
                values[key] = flagArgs;
                
                flagArgs.clear();
            };

            for(int i = 1; i < argc; i++)
            {
                if (std::strlen(argv[i]) == 0 ) 
                    continue;

                if (strcmp(argv[i], "--help") == 0
                    || strcmp(argv[i], "-help") == 0 ) 
                {
                    PrintHelp();
                    return false;
                }

                //rest of the parsing
                if(argv[i][0] == '-')
                {
                    AddArgument();
                    flag = &(argv[i][0]);
                }
                else
                    flagArgs.push_back(argv[i]);


                if( i == argc - 1)
                    AddArgument();
            }

            //iterate over all options to check for required
            for (const auto& o : options)
            {
                if(o._required)
                {
                    std::string name = _ltrim_copy(o._fullName, 
                    [](char c) { return c != (char)'-'; });

                    auto f = values.find(name);
                    if( f == values.end())
                    {
                        std::cout<<"Missing required arguments."<<std::endl;
                        PrintHelp();
                        return false;
                    }
                }
            }

            return true;
        }

        void PrintHelp()
        {
            PrintUsage();
            std::cout << "Options: " <<std::endl;

            for (const auto& o : options) 
            {
                std::cout << "  " << o._shortName << ", " 
                << o._fullName << " " << o._description << std::endl;
            }
        }

        void PrintUsage()
        {
            std::cout << "Usage: " << exe_name << " ";

            for (const auto& o : options) 
            {
                if (o._required) 
                {
                    std::string fullname = o._fullName;
                    fullname.erase(0, 2);
                    std::cout << o._shortName << " [" << fullname << "] ";
                }
            }
            std::cout << std::endl << std::endl;
        }

        bool IsSet(const std::string &name)
        {
            std::string key = name;
            if( short_to_full.find(name) != short_to_full.end()) 
            {
                key = short_to_full[key];
            }

            if(values.find(key) != values.end())
            {
                return true;
            }
            return false;
        }

        template <typename T>
        T Get(const std::string &name)
        {
            T out;

            std::string key = name;
            if( short_to_full.find(name) != short_to_full.end()) 
                key = short_to_full[key];

            if(values.find(key) != values.end())
            {
                //really handy!
                //error check this though
                return get<T>(values[key][0]);
            }

            return out;
        }

        template <typename T>
        std::vector<T> GetVector(const std::string &name)
        {
            std::vector<T> out;

            std::string key = name;
            if( short_to_full.find(name) != short_to_full.end()) 
                key = short_to_full[key];

            if(values.find(key) != values.end())
            {
                auto v = values[key];
                for(auto& x : v)
                {
                    out.push_back(get<T>(x));
                }
            }

            return out;
        }

    private:

        template <typename T>
        T get(std::string &val)
        {
            T out;

            std::istringstream in(val);
            in >> out;

            return out;
        }


        static inline bool NotSpace(char c) { return !std::isspace(c); }

        static inline void _ltrim(std::string &s, bool (*f)(char) = NotSpace) 
        {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), f));
        }

        static inline std::string _ltrim_copy(
            std::string s, bool (*f)(char) = NotSpace) 
        {
            _ltrim(s, f);
            return s;
        }

        static inline void _rtrim(std::string &s, bool (*f)(char) = NotSpace) 
        {
            s.erase(std::find_if(s.rbegin(), s.rend(), f).base(), s.end());
        }
        static inline void Trim(std::string &s, bool (*f)(char) = NotSpace) 
        {
            _ltrim(s, f);
            _rtrim(s, f);
        }


        std::vector<Arg> options;

        std::unordered_map<std::string, std::string> short_to_full;
        std::unordered_map<std::string, std::vector< std::string> > values;

        std::string usage;
        std::string description;
        std::string exe_name;
};

//and for bool, shall be this
template <>
inline bool ArgParser::Get<bool>(const std::string &name) 
{
  return IsSet(name);
}