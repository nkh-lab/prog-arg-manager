#include "ProgArgManager.h"

namespace nlab {

ProgArgManager::ProgArgManager(int argc, char** argv, std::string version, std::ostream& output)
    : m_argc{argc}
    , m_argv{argv}
    , m_version{version}
    , m_output{output}
    , m_all_options{"Allowed options"}
    , m_optional_options{"Optional"}
    , m_mandatory_options{"Mandatory (last option name can be omitted)"}
{
}

void ProgArgManager::addGenericOptions()
{
    bpo::options_description generic_opt("Generic");

    generic_opt.add_options()("help,h", "display this help text and exit");

    if (!m_version.empty())
        generic_opt.add_options()("version,v", "display version information and exit");

    m_all_options.add(generic_opt);
}

// Required by template parameter pack
void ProgArgManager::addOptions()
{
}

// Required by template parameter pack
void ProgArgManager::retriveValues(bool& check_mandatory)
{
    (void)check_mandatory;
}

void ProgArgManager::printHelp()
{
    m_output << m_all_options << "\n";
}

void ProgArgManager::printVersion()
{
    m_output << m_version << "\n";
}

std::string ProgArgManager::getName(const std::string& name_and_symbol)
{
    auto coma_pos = name_and_symbol.find(',');

    if (std::string::npos == coma_pos)
        return name_and_symbol;
    else
        return name_and_symbol.substr(0, coma_pos);
}

} // namespace nlab
