#include "ProgArgManager.h"

#include <gtest/gtest.h>

using namespace nlab;

template <typename T>
constexpr size_t argv_size()
{
    return (sizeof(T) / sizeof(char*));
}

class ExampleOptionsTest : public ::testing::Test
{
protected:
    // void SetUp() override {}
    // void TearDown() override {}

    // Use options from README.md example (from component_test)
    // Optional numeric option with default value
    Option<int> m_opt_num{"num", "to test numeric arguments", 5555, true};

    // Boolean options, always treated like optional, therefore no need to specify their optionality
    Option<bool> m_opt_read{"read,r", "read flag to test boolean"};
    Option<bool> m_opt_write{"write,w", "write flag to test boolean"};
    Option<bool> m_opt_del{"delete,d", "delete flag to test boolean"};

    // Other types are mandatory options, if optionality is not set
    Option<std::string> m_opt_path{"path,p", "path to test string"};
    Option<std::vector<std::string>> m_opt_files{"files,f", "files to test vector<string>"};

    // clang-format off
    const char* m_help_wo_version =
        "Allowed options:\n"
        "\n"
        "Generic:\n"
        "  -h [ --help ]          display this help text and exit\n"
        "\n"
        "Optional:\n"
        "  --num arg              to test numeric arguments\n"
        "  -r [ --read ]          read flag to test boolean\n"
        "  -w [ --write ]         write flag to test boolean\n"
        "  -d [ --delete ]        delete flag to test boolean\n"
        "\n"
        "Mandatory (last option name can be omitted):\n"
        "  -p [ --path ] arg      path to test string\n"
        "  -f [ --files ] arg     files to test vector<string>\n"
        "\n";
    const char* m_help_with_version =
        "Allowed options:\n"
        "\n"
        "Generic:\n"
        "  -h [ --help ]          display this help text and exit\n"
        "  -v [ --version ]       display version information and exit\n"
        "\n"
        "Optional:\n"
        "  --num arg              to test numeric arguments\n"
        "  -r [ --read ]          read flag to test boolean\n"
        "  -w [ --write ]         write flag to test boolean\n"
        "  -d [ --delete ]        delete flag to test boolean\n"
        "\n"
        "Mandatory (last option name can be omitted):\n"
        "  -p [ --path ] arg      path to test string\n"
        "  -f [ --files ] arg     files to test vector<string>\n"
        "\n";
    // clang-format on
};

TEST_F(ExampleOptionsTest, ParceHelpWithVersion)
{
    char* argv[] = {"", "--help"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, "1.0.0", output);

    EXPECT_FALSE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), m_help_with_version);
}

TEST_F(ExampleOptionsTest, ParceHWithVersion)
{
    char* argv[] = {"", "-h"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, "1.0.0", output);

    EXPECT_FALSE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), m_help_with_version);
}

TEST_F(ExampleOptionsTest, ParceHelpWoVersion)
{
    char* argv[] = {"", "--help"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, {}, output);

    EXPECT_FALSE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), m_help_wo_version);
}

TEST_F(ExampleOptionsTest, ParceHWoVersion)
{
    char* argv[] = {"", "-h"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, {}, output);

    EXPECT_FALSE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), m_help_wo_version);
}

TEST_F(ExampleOptionsTest, ParceVersion)
{
    char* argv[] = {"", "--version"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, "1.0.0", output);

    EXPECT_FALSE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), "1.0.0\n");
}

TEST_F(ExampleOptionsTest, ParceV)
{
    char* argv[] = {"", "-v"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, "1.0.0", output);

    EXPECT_FALSE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), "1.0.0\n");
}

TEST_F(ExampleOptionsTest, OnlyAllMandatory)
{
    char* argv[] = {"", "-p", "/path/to/my/dir", "file1.txt", "file2.txt", "file3.txt"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, "1.0.0", output);

    EXPECT_TRUE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), "");

    // Mandatory
    EXPECT_STREQ(m_opt_path.value.c_str(), "/path/to/my/dir");
    EXPECT_EQ(m_opt_files.value.size(), 3);
    EXPECT_STREQ(m_opt_files.value[0].c_str(), "file1.txt");
    EXPECT_STREQ(m_opt_files.value[1].c_str(), "file2.txt");
    EXPECT_STREQ(m_opt_files.value[2].c_str(), "file3.txt");

    // Optional should be default
    EXPECT_EQ(m_opt_num.value, 5555);
    EXPECT_FALSE(m_opt_read.value);
    EXPECT_FALSE(m_opt_write.value);
    EXPECT_FALSE(m_opt_del.value);
}

TEST_F(ExampleOptionsTest, NotAllMandatory)
{
    char* argv[] = {"", "file1.txt", "file2.txt", "file3.txt"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, "1.0.0", output);

    EXPECT_FALSE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), m_help_with_version);
}

TEST_F(ExampleOptionsTest, ReadmeTest)
{
    char* argv[] = {
        "", "--num", "789", "-rw", "-p", "/path/to/my/dir", "file1.txt", "file2.txt", "file3.txt"};
    int argc = argv_size<decltype(argv)>();

    std::stringstream output;

    ProgArgManager pam(argc, argv, "1.0.0", output);

    EXPECT_TRUE(pam.parse(m_opt_num, m_opt_read, m_opt_write, m_opt_del, m_opt_path, m_opt_files));

    EXPECT_STREQ(output.str().c_str(), "");

    // Mandatory
    EXPECT_STREQ(m_opt_path.value.c_str(), "/path/to/my/dir");
    EXPECT_EQ(m_opt_files.value.size(), 3);
    EXPECT_STREQ(m_opt_files.value[0].c_str(), "file1.txt");
    EXPECT_STREQ(m_opt_files.value[1].c_str(), "file2.txt");
    EXPECT_STREQ(m_opt_files.value[2].c_str(), "file3.txt");

    // Optional
    EXPECT_EQ(m_opt_num.value, 789);
    EXPECT_TRUE(m_opt_read.value);
    EXPECT_TRUE(m_opt_write.value);
    EXPECT_FALSE(m_opt_del.value);
}
