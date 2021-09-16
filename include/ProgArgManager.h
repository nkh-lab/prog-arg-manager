#pragma once

#include <boost/program_options.hpp>

#include <iostream>
#include <string>

namespace bpo = boost::program_options;

namespace nlab {

template <typename T>
struct Option
{
    using type = T;

    const std::string name_and_symbol; // comma separated, e.g. "file,f"
    std::string description = {};
    T value = {};
    bool optional = false;
};

template <typename T>
class ValueRetriever
{
public:
    bool getValue(const bpo::variables_map& vm, const std::string& name, bool optional, T& value)
    {
        bool ret = optional ? true : false;

        if (vm.count(name))
        {
            value = vm[name].template as<T>();
            ret = true;
        }

        return ret;
    }
};

template <>
class ValueRetriever<bool>
{
public:
    bool getValue(const bpo::variables_map& vm, const std::string& name, bool optional, bool& value)
    {
        (void)optional;
        if (vm.count(name))
            value = true;
        else
            value = false;

        return true;
    }
};

class ProgArgManager
{
public:
    ProgArgManager(int argc, char** argv, std::string version = {}, std::ostream& output = std::cout);

    template <class T, class... Ts>
    bool parse(T& first, Ts&... rest)
    {
        bool ret = false;

        addGenericOptions();

        addOptions(first, rest...);

        if (!m_optional_options.options().empty()) m_all_options.add(m_optional_options);
        if (!m_mandatory_options.options().empty()) m_all_options.add(m_mandatory_options);

        bpo::store(
            bpo::command_line_parser(m_argc, m_argv)
                .options(m_all_options)
                .positional(m_positional_options_description)
                .run(),
            m_variables_map);

        bpo::notify(m_variables_map);

        if (m_variables_map.count("help"))
        {
            printHelp();
        }
        else if (m_variables_map.count("version"))
        {
            printVersion();
        }
        else
        {
            bool check_mandatory = true;
            retriveValues(check_mandatory, first, rest...);

            if (check_mandatory)
                ret = true;
            else
                printHelp();
        }

        return ret;
    }

private:
    void addGenericOptions();

    template <class T, class... Ts>
    void addOptions(T& first, Ts&... rest)
    {
        addOption(first);

        addOptions(rest...);
    }

    template <class T>
    void addOptions(T& last)
    {
        addOption(last);

        std::string name = getName(last.name_and_symbol);
        m_positional_options_description.add(name.c_str(), -1);
    }

    void addOptions();

    template <class T>
    void addOption(T& opt)
    {
        if (std::is_same<typename T::type, bool>::value)
        {
            m_optional_options.add_options()(opt.name_and_symbol.c_str(), opt.description.c_str());
        }
        else
        {
            if (opt.optional)
            {
                m_optional_options.add_options()(
                    opt.name_and_symbol.c_str(),
                    bpo::value<typename T::type>(),
                    opt.description.c_str());
            }
            else
            {
                m_mandatory_options.add_options()(
                    opt.name_and_symbol.c_str(),
                    bpo::value<typename T::type>(),
                    opt.description.c_str());
            }
        }
    }

    template <class T, class... Ts>
    void retriveValues(bool& check_mandatory, T& first, Ts&... rest)
    {
        std::string name = getName(first.name_and_symbol);

        if (!ValueRetriever<typename T::type>().getValue(
                m_variables_map, name, first.optional, first.value) &&
            check_mandatory)
        {
            check_mandatory = false;
        }

        retriveValues(check_mandatory, rest...);
    }

    void retriveValues(bool& check_mandatory);

    void printHelp();
    void printVersion();

    std::string getName(const std::string& name_and_symbol);

private:
    int m_argc;
    char** m_argv;
    std::string m_version;
    std::ostream& m_output;
    bpo::options_description m_all_options;
    bpo::options_description m_optional_options;
    bpo::options_description m_mandatory_options;

    bpo::positional_options_description m_positional_options_description;
    bpo::variables_map m_variables_map;
};
} // namespace nlab
