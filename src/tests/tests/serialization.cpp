#include <gtest/gtest.h>
#include <zasm/assembler.hpp>
#include <zasm/program.hpp>

namespace zasm::tests
{
    TEST(SerializationTests, LeaRax0X64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);

        ASSERT_EQ(assembler.lea(rax, qword_ptr(0)), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 8> expected = {
            0x48, 0x8D, 0x04, 0x25, 0x00, 0x00, 0x00, 0x00,
        };
        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, LeaEax0X86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);

        ASSERT_EQ(assembler.lea(eax, dword_ptr(0)), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 6> expected = {
            0x8D, 0x05, 0x00, 0x00, 0x00, 0x00,
        };
        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, LeaRipLabelX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.lea(rax, qword_ptr(rip, label)), Error::None);
        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.dq(0x123C312F21329F31ULL), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 15> expected = {
            0x48, 0x8D, 0x05, 0x00, 0x00, 0x00, 0x00, 0x31, 0x9F, 0x32, 0x21, 0x2F, 0x31, 0x3C, 0x12,
        };
        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, LeaLabelX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.lea(eax, dword_ptr(label)), Error::None);
        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.dq(0x123C312F21329F31ULL), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 14> expected = { 0x8D, 0x05, 0x06, 0x10, 0x40, 0x00, 0x31,
                                                   0x9F, 0x32, 0x21, 0x2F, 0x31, 0x3C, 0x12 };
        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel8FrontX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.jmp(label), Error::None);
        for (int i = 0; i < 127; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 130> expected = {
            0xEB, 0x7F, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xCC,
        };

        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel8FrontX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.jmp(label), Error::None);
        for (int i = 0; i < 127; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 130> expected = {
            0xEB, 0x7F, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xCC,
        };

        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel8BackX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        for (int i = 0; i < 124; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.jmp(label), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 127> expected = {
            0xCC, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xEB, 0x81,
        };
        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel8BackX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        for (int i = 0; i < 124; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.jmp(label), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 127> expected = {
            0xCC, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xEB, 0x81,
        };
        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel32BackX64)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_64);
        Assembler assembler(program);

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        for (int i = 0; i < 125; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.jmp(label), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 131> expected = {
            0xCC, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9, 0x7D, 0xFF, 0xFF, 0xFF,
        };
        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

    TEST(SerializationTests, JmpLabelRel32BackX86)
    {
        using namespace zasm::operands;

        Program program(ZYDIS_MACHINE_MODE_LONG_COMPAT_32);
        Assembler assembler(program);

        auto label = assembler.createLabel();

        ASSERT_EQ(assembler.bind(label), Error::None);
        ASSERT_EQ(assembler.int3(), Error::None);
        for (int i = 0; i < 125; i++)
            ASSERT_EQ(assembler.nop(), Error::None);
        ASSERT_EQ(assembler.jmp(label), Error::None);
        ASSERT_EQ(program.serialize(0x0000000000401000), Error::None);

        const std::array<uint8_t, 131> expected = {
            0xCC, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9, 0x7D, 0xFF, 0xFF, 0xFF,
        };
        ASSERT_EQ(program.getCodeSize(), expected.size());

        const auto* data = program.getCode();
        ASSERT_NE(data, nullptr);
        for (size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data[i], expected[i]);
        }
    }

} // namespace zasm::tests