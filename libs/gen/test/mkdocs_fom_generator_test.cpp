// === mkdocs_fom_generator_test.cpp ===================================================================================
//                                               Sen Infrastructure
//                   Released under the Apache License v2.0 (SPDX-License-Identifier Apache-2.0).
//                                    See the LICENSE.txt file for more information.
//                   © Airbus SAS, Airbus Helicopters, and Airbus Defence and Space SAU/GmbH/SAS.
// =====================================================================================================================

#include "mkdocs_fixture_test.h"

// 3rd party
#include <gtest/gtest.h>

// std
#include <filesystem>
#include <string>
#include <vector>

/// @test
/// Check sen gen mkdocs generates a correct documentation file from a minimal FOM file
TEST_F(AMkdocsGenerator, FomEmpty)
{
  const std::vector<std::filesystem::path> paths {dataDirectoryPath() / "mkdocs" / "empty"};
  ASSERT_TRUE(std::filesystem::exists(paths.front())) << paths.front();

  generateFom(paths);
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
}

/// @test
/// Check sen gen mkdocs does not generate a documentation file from an invalid FOM file
TEST_F(AMkdocsGenerator, FomErrorFormat)
{
  const std::vector<std::filesystem::path> paths {dataDirectoryPath() / "mkdocs" / "error_format"};
  ASSERT_TRUE(std::filesystem::exists(paths.front())) << paths.front();

  ASSERT_ANY_THROW(generateFom(paths));
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from FOM file with empty class definition
TEST_F(AMkdocsGenerator, FomEmptyClass)
{
  const std::vector<std::filesystem::path> paths {dataDirectoryPath() / "mkdocs" / "empty_class"};
  ASSERT_TRUE(std::filesystem::exists(paths.front())) << paths.front();

  generateFom(paths);

  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(
    section("### Type Map").find("[hla.ObjectRoot](#hlaobjectroot \"HLA Base Class that contains the RTI object ID\")"),
    std::string::npos)
    << content();
  EXPECT_NE(section("### Type Map").find("[empty_class.MyTestClass](#empty_classmytestclass)"), std::string::npos)
    << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("**rtiId**"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("[string](#string) HLA RTI object ID"), std::string::npos) << content();
  EXPECT_NE(section("### empty_class.MyTestClass").find("Parent: [hla.ObjectRoot](#hlaobjectroot)"), std::string::npos)
    << content();
  EXPECT_NE(section("### hla.MaybeU8").find("Optional Type: [u8](#u8)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI16").find("Optional Type: [i16](#i16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU16").find("Optional Type: [u16](#u16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI32").find("Optional Type: [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU32").find("Optional Type: [u32](#u32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI64").find("Optional Type: [i64](#i64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU64").find("Optional Type: [u64](#u64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeF32").find("Optional Type: [f32](#f32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeF64").find("Optional Type: [f64](#f64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeBool").find("Optional Type: [bool](#bool)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeString").find("Optional Type: [string](#string)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeTimeStamp").find("Optional Type: [sen.TimeStamp](#sentimestamp)"), std::string::npos)
    << content();
  EXPECT_NE(section("### hla.MaybeDuration").find("Optional Type: [sen.Duration](#senduration)"), std::string::npos)
    << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from FOM file with basic properties in the class
TEST_F(AMkdocsGenerator, FomBasicProperties)
{
  const std::vector<std::filesystem::path> paths {dataDirectoryPath() / "mkdocs" / "basic_properties"};
  ASSERT_TRUE(std::filesystem::exists(paths.front())) << paths.front();

  generateFom(paths);

  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(
    section("### Type Map").find("[hla.ObjectRoot](#hlaobjectroot \"HLA Base Class that contains the RTI object ID\")"),
    std::string::npos)
    << content();
  EXPECT_NE(section("### Type Map").find("[basic_properties.MyTestClass](#basic_propertiesmytestclass)"),
            std::string::npos)
    << content();
  EXPECT_NE(section("### Type Map").find("[MyTestInt](#basic_propertiesmytestint)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("**rtiId**"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("[string](#string) HLA RTI object ID"), std::string::npos) << content();
  EXPECT_NE(section("### basic_properties.MyTestClass").find("Parent: [hla.ObjectRoot](#hlaobjectroot)"),
            std::string::npos)
    << content();
  EXPECT_NE(section("### basic_properties.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### basic_properties.MyTestClass").find("**prop1**"), std::string::npos) << content();
  EXPECT_NE(
    section("### basic_properties.MyTestClass").find("[basic_properties.MyTestInt](#basic_propertiesmytestint)"),
    std::string::npos)
    << content();
  EXPECT_NE(section("### basic_properties.MyTestInt").find("Aliased Type: [i32](#i32)"), std::string::npos)
    << content();
  EXPECT_NE(section("### hla.MaybeU8").find("Optional Type: [u8](#u8)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI16").find("Optional Type: [i16](#i16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU16").find("Optional Type: [u16](#u16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI32").find("Optional Type: [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU32").find("Optional Type: [u32](#u32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI64").find("Optional Type: [i64](#i64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU64").find("Optional Type: [u64](#u64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeF32").find("Optional Type: [f32](#f32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeF64").find("Optional Type: [f64](#f64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeBool").find("Optional Type: [bool](#bool)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeString").find("Optional Type: [string](#string)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeTimeStamp").find("Optional Type: [sen.TimeStamp](#sentimestamp)"), std::string::npos)
    << content();
  EXPECT_NE(section("### hla.MaybeDuration").find("Optional Type: [sen.Duration](#senduration)"), std::string::npos)
    << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from FOM file with a struct property in the class
TEST_F(AMkdocsGenerator, FomStructProperty)
{
  const std::vector<std::filesystem::path> paths {dataDirectoryPath() / "mkdocs" / "struct_property"};
  ASSERT_TRUE(std::filesystem::exists(paths.front())) << paths.front();

  generateFom(paths);

  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(
    section("### Type Map").find("[hla.ObjectRoot](#hlaobjectroot \"HLA Base Class that contains the RTI object ID\")"),
    std::string::npos)
    << content();
  EXPECT_NE(section("### Type Map").find("[struct_property.MyTestClass](#struct_propertymytestclass)"),
            std::string::npos)
    << content();
  EXPECT_NE(section("### Type Map").find("[MyTestStruct](#struct_propertymyteststruct)"), std::string::npos)
    << content();
  EXPECT_NE(section("### Type Map").find("[MyTestInt32](#struct_propertymytestint32)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestInt16](#struct_propertymytestint16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("**rtiId**"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("[string](#string) HLA RTI object ID"), std::string::npos) << content();
  EXPECT_NE(section("### struct_property.MyTestClass").find("Parent: [hla.ObjectRoot](#hlaobjectroot)"),
            std::string::npos)
    << content();
  EXPECT_NE(section("### struct_property.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### struct_property.MyTestClass").find("**prop1**"), std::string::npos) << content();
  EXPECT_NE(
    section("### struct_property.MyTestClass").find("[struct_property.MyTestStruct](#struct_propertymyteststruct)"),
    std::string::npos)
    << content();
  EXPECT_NE(section("### struct_property.MyTestStruct").find("\n\n| Field     | Description |"), std::string::npos)
    << content();
  EXPECT_NE(section("### struct_property.MyTestStruct")
              .find("**field1** | [struct_property.MyTestInt32](#struct_propertymytestint32)"),
            std::string::npos)
    << content();
  EXPECT_NE(section("### struct_property.MyTestStruct")
              .find("**field2** | [struct_property.MyTestInt16](#struct_propertymytestint16)"),
            std::string::npos)
    << content();
  EXPECT_NE(section("### struct_property.MyTestInt32").find("Aliased Type: [i32](#i32)"), std::string::npos)
    << content();
  EXPECT_NE(section("### struct_property.MyTestInt16").find("Aliased Type: [i16](#i16)"), std::string::npos)
    << content();
  EXPECT_NE(section("### hla.MaybeU8").find("Optional Type: [u8](#u8)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI16").find("Optional Type: [i16](#i16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU16").find("Optional Type: [u16](#u16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI32").find("Optional Type: [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU32").find("Optional Type: [u32](#u32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI64").find("Optional Type: [i64](#i64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU64").find("Optional Type: [u64](#u64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeF32").find("Optional Type: [f32](#f32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeF64").find("Optional Type: [f64](#f64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeBool").find("Optional Type: [bool](#bool)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeString").find("Optional Type: [string](#string)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeTimeStamp").find("Optional Type: [sen.TimeStamp](#sentimestamp)"), std::string::npos)
    << content();
  EXPECT_NE(section("### hla.MaybeDuration").find("Optional Type: [sen.Duration](#senduration)"), std::string::npos)
    << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from FOM file with an enum property in the class
TEST_F(AMkdocsGenerator, FomEnumProperty)
{
  const std::vector<std::filesystem::path> paths {dataDirectoryPath() / "mkdocs" / "enum_property"};
  ASSERT_TRUE(std::filesystem::exists(paths.front())) << paths.front();

  generateFom(paths);

  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(
    section("### Type Map").find("[hla.ObjectRoot](#hlaobjectroot \"HLA Base Class that contains the RTI object ID\")"),
    std::string::npos)
    << content();
  EXPECT_NE(section("### Type Map").find("[enum_property.MyTestClass](#enum_propertymytestclass)"), std::string::npos)
    << content();
  EXPECT_NE(section("### Type Map").find("[MyTestEnum](#enum_propertymytestenum)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("**rtiId**"), std::string::npos) << content();
  EXPECT_NE(section("### hla.ObjectRoot").find("[string](#string) HLA RTI object ID"), std::string::npos) << content();
  EXPECT_NE(section("### enum_property.MyTestClass").find("Parent: [hla.ObjectRoot](#hlaobjectroot)"),
            std::string::npos)
    << content();
  EXPECT_NE(section("### enum_property.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### enum_property.MyTestClass").find("**prop1**"), std::string::npos) << content();
  EXPECT_NE(section("### enum_property.MyTestClass").find("[enum_property.MyTestEnum](#enum_propertymytestenum)"),
            std::string::npos)
    << content();
  EXPECT_NE(section("### enum_property.MyTestEnum").find("Representation: [i32](#i32)"), std::string::npos)
    << content();
  EXPECT_NE(section("### enum_property.MyTestEnum").find("\n\n| Enumerator  | Value |"), std::string::npos)
    << content();
  EXPECT_NE(section("### enum_property.MyTestEnum").find("| first | 1 |"), std::string::npos) << content();
  EXPECT_NE(section("### enum_property.MyTestEnum").find("| second | 2 |"), std::string::npos) << content();
  EXPECT_NE(section("### enum_property.MyTestEnum").find("| third | 3 |"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU8").find("Optional Type: [u8](#u8)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI16").find("Optional Type: [i16](#i16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU16").find("Optional Type: [u16](#u16)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI32").find("Optional Type: [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU32").find("Optional Type: [u32](#u32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeI64").find("Optional Type: [i64](#i64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeU64").find("Optional Type: [u64](#u64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeF32").find("Optional Type: [f32](#f32)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeF64").find("Optional Type: [f64](#f64)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeBool").find("Optional Type: [bool](#bool)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeString").find("Optional Type: [string](#string)"), std::string::npos) << content();
  EXPECT_NE(section("### hla.MaybeTimeStamp").find("Optional Type: [sen.TimeStamp](#sentimestamp)"), std::string::npos)
    << content();
  EXPECT_NE(section("### hla.MaybeDuration").find("Optional Type: [sen.Duration](#senduration)"), std::string::npos)
    << content();
}
