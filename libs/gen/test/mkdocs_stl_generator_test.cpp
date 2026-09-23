// === mkdocs_stl_generator_test.cpp ===================================================================================
//                                               Sen Infrastructure
//                   Released under the Apache License v2.0 (SPDX-License-Identifier Apache-2.0).
//                                    See the LICENSE.txt file for more information.
//                   © Airbus SAS, Airbus Helicopters, and Airbus Defence and Space SAU/GmbH/SAS.
// =====================================================================================================================

#include "mkdocs_fixture_test.h"

// 3rd party
#include <gtest/gtest.h>

// std
#include <string>

constexpr auto empty = R"()";

constexpr auto errorFormat = R"(;)";

constexpr auto package = R"(package my.test;)";

constexpr auto emptyClass = R"(package my_test_package;

class MyTestClass
{}
)";

constexpr auto basicProperties = R"(package my_test_package;

class MyTestClass
{
  var prop1 : string       [static];
  var prop2 : i32          [writable];
}
)";

constexpr auto structProperty = R"(package my_test_package;

struct MyTestStruct
{
  x : f32,
  y : f64
}

class MyTestClass
{
  var struct1 : MyTestStruct          [writable];
}
)";

constexpr auto enumProperty = R"(package my_test_package;

enum MyTestEnum: u8
{
  first,
  second,
  third
}

class MyTestClass
{
  var enum1 : MyTestEnum          [writable];
}
)";

constexpr auto variantProperty = R"(package my_test_package;

variant MyTestVariant
{
  i32,
  bool,
  string
}

class MyTestClass
{
  var variant1 : MyTestVariant          [confirmed, writable];
}
)";

constexpr auto sequenceProperty = R"(package my_test_package;

sequence<i64, 100> MyTestBoundedSequence;
sequence<i32> MyTestUnboundedSequence;

class MyTestClass
{
  var sequence1 : MyTestBoundedSequence          [writable];
  var sequence2 : MyTestUnboundedSequence            [writable, confirmed];
}
)";

constexpr auto aliasProperty = R"(package my_test_package;

alias MyTestAlias i64;

class MyTestClass
{
  var alias1 : MyTestAlias          [writable];
}
)";

constexpr auto optionalProperty = R"(package my_test_package;

optional<f32> MyTestOptional;

class MyTestClass
{
  var optional1 : MyTestOptional          [writable];
}
)";

constexpr auto quantityProperty = R"(package my_test_package;

quantity<f32, m> MyTestQuantity;

class MyTestClass
{
  var quantity1 : MyTestQuantity          [writable];
}
)";

constexpr auto methods = R"(package my_test_package;

class MyTestClass
{
  var result : i32          [writable];

  fn addNumbers(a: i32, b: i32) -> i32;
  fn subtractNumbers(x: i32, y: i32) -> i32;
}
)";

constexpr auto events = R"(package my_test_package;

class MyTestClass
{
  var result : i32          [writable];

  // Emitted when there's an attempt to divide by zero.
  event divisionByZero();
}
)";

constexpr auto inheritance = R"(package my_test_package;

class MyBaseTestClass
{
  var baseProp : string [confirmed];
}

class MyDerivedTestClass : extends MyBaseTestClass
{
  var derivedProp: f64 [writable];
}
)";

constexpr auto comment = R"(package my_test_package;

// This is my test class
class MyTestClass
{
  // This is my prop
  var prop1 : string       [static];

  // This is my method
  fn addNumbers(a: i32, b: i32) -> i32;

  // This is my event
  event divisionByZero();
}

// This is my struct
struct MyTestStruct
{
  x : f32, // This is x
  y : f64 // This is y
}

// This is my enum
enum MyTestEnum: u8
{
  first, // This is my first value
  second, // This is my second value
  third // This is my third value
}

// This is my variant
variant MyTestVariant
{
  i32, //integer
  bool, //boolean
  string //string
}

sequence<i64, 100> MyTestBoundedSequence; // This is my bounded sequence

sequence<i32> MyTestUnboundedSequence; // This is my unbounded sequence

alias MyTestAlias i64; // This is my alias

optional<f32> MyTestOptional; // This is my optional

quantity<f32, m> MyTestQuantity; // This is my quantity
)";

constexpr auto escapedPipeComment = R"(package my_test_package;

// escaped |
class MyTestClass
{
  // escaped |
  var prop1 : string       [static];
}

struct MyTestStruct
{
  x : f32, // escaped |
  y : f64 // escaped |
}

enum MyTestEnum: u8
{
  first, // escaped |
  second, // escaped |
  third // escaped |
}

variant MyTestVariant
{
  i32, // escaped |
  bool, // escaped |
  string // escaped |
}
)";

/// @test
/// Check sen gen mkdocs generates a correct documentation file from an empty STL file
TEST_F(AMkdocsGenerator, StlEmpty)
{
  ASSERT_TRUE(generateStl(empty));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
}

/// @test
/// Check sen gen mkdocs does not generate a documentation file from an invalid STL file
TEST_F(AMkdocsGenerator, StlErrorFormat) { ASSERT_ANY_THROW(generateStl(errorFormat)); }

/// @test
/// Check sen gen mkdocs generates a correct documentation file with a package definition
TEST_F(AMkdocsGenerator, StlPackage)
{
  ASSERT_TRUE(generateStl(package));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(find("\"my.test\""), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from an STL file with an empty class
TEST_F(AMkdocsGenerator, StlEmptyClass)
{
  ASSERT_TRUE(generateStl(emptyClass));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file with basic properties in the class
TEST_F(AMkdocsGenerator, StlBasicProperties)
{
  ASSERT_TRUE(generateStl(basicProperties));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**prop1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[string](#string)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**prop2**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[i32](#i32)"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with a struct property in the class
TEST_F(AMkdocsGenerator, StlStructProperty)
{
  ASSERT_TRUE(generateStl(structProperty));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestStruct](#my_test_packagemyteststruct)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**struct1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[my_test_package.MyTestStruct](#my_test_packagemyteststruct)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("\n\n| Field     | Description |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("**x** | [f32](#f32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("**y** | [f64](#f64)"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with an enum property in the class
TEST_F(AMkdocsGenerator, StlEnumProperty)
{
  ASSERT_TRUE(generateStl(enumProperty));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestEnum](#my_test_packagemytestenum)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**enum1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[my_test_package.MyTestEnum](#my_test_packagemytestenum)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("\nRepresentation: [u8](#u8)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("first | 0 "), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("second | 1 "), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("third | 2 "), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with a variant property in the class
TEST_F(AMkdocsGenerator, StlVariantProperty)
{
  ASSERT_TRUE(generateStl(variantProperty));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestVariant](#my_test_packagemytestvariant)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**variant1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[my_test_package.MyTestVariant](#my_test_packagemytestvariant)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("\n\n| Type |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[bool](#bool)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[string](#string)"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with a sequence property in the class
TEST_F(AMkdocsGenerator, StlSequenceProperty)
{
  ASSERT_TRUE(generateStl(sequenceProperty));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestBoundedSequence](#my_test_packagemytestboundedsequence)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestUnboundedSequence](#my_test_packagemytestunboundedsequence)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**sequence1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[my_test_package.MyTestBoundedSequence](#my_test_packagemytestboundedsequence)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**sequence2**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[my_test_package.MyTestUnboundedSequence](#my_test_packagemytestunboundedsequence)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("\n\n|  |  |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("Element Type | [i64](#i64)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("Bounded | Yes"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("Max size | 100"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestUnboundedSequence").find("\n\n|  |  |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestUnboundedSequence").find("Element Type | [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestUnboundedSequence").find("Bounded | No"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with an alias property in the class
TEST_F(AMkdocsGenerator, StlAliasProperty)
{
  ASSERT_TRUE(generateStl(aliasProperty));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**alias1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[my_test_package.MyTestAlias](#my_test_packagemytestalias)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestAlias").find("Aliased Type: [i64](#i64)"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with an optional property in the class
TEST_F(AMkdocsGenerator, StlOptionalProperty)
{
  ASSERT_TRUE(generateStl(optionalProperty));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**optional1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[my_test_package.MyTestOptional](#my_test_packagemytestoptional)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestOptional").find("Optional Type: [f32](#f32)"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with a quantity property in the class
TEST_F(AMkdocsGenerator, StlQuantityProperty)
{
  ASSERT_TRUE(generateStl(quantityProperty));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestQuantity](#my_test_packagemytestquantity)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**quantity1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[my_test_package.MyTestQuantity](#my_test_packagemytestquantity)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestQuantity").find("Representation: f32"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestQuantity").find("Unit:  m"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with methods in the class
TEST_F(AMkdocsGenerator, StlMethods)
{
  ASSERT_TRUE(generateStl(methods));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**result**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Methods**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**addNumbers**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("Returns: [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("\n\n    | Argument  | Type | Description |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("a | [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("b | [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**subtractNumbers**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("Returns: [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("\n\n    | Argument  | Type | Description |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("x | [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("y | [i32](#i32)"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with events in the class
TEST_F(AMkdocsGenerator, StlEvents)
{
  ASSERT_TRUE(generateStl(events));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**result**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Events**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**divisionByZero**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find(":   Emitted when there's an attempt to divide by zero."), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with inheritance
TEST_F(AMkdocsGenerator, StlInheritance)
{
  ASSERT_TRUE(generateStl(inheritance));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyBaseTestClass](#my_test_packagemybasetestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyDerivedTestClass](#my_test_packagemyderivedtestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyBaseTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyBaseTestClass").find("**baseProp**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyBaseTestClass").find("[string](#string)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyDerivedTestClass").find("Parent: [my_test_package.MyBaseTestClass](#my_test_packagemybasetestclass)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyDerivedTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyDerivedTestClass").find("**derivedProp**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyDerivedTestClass").find("[f64](#f64)"), std::string::npos) << content();
}


/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with comments
TEST_F(AMkdocsGenerator, StlComment)
{
  ASSERT_TRUE(generateStl(comment));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass \"This is my test class\")"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestStruct](#my_test_packagemyteststruct \"This is my struct\")"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestVariant](#my_test_packagemytestvariant \"This is my variant\")"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestBoundedSequence](#my_test_packagemytestboundedsequence \"This is my bounded sequence\")"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestUnboundedSequence](#my_test_packagemytestunboundedsequence \"This is my unbounded sequence\")"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestQuantity](#my_test_packagemytestquantity \"This is my quantity\")"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestEnum](#my_test_packagemytestenum \"This is my enum\")"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestAlias](#my_test_packagemytestalias \"This is my alias\")"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**prop1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[string](#string) This is my prop"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Methods**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**addNumbers**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find(":   This is my method"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("Returns: [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("\n\n    | Argument  | Type | Description |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("a | [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("b | [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Events**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**divisionByZero**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find(":   This is my event"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("\n\nThis is my struct"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("\n\n| Field     | Description |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("**x** | [f32](#f32) This is x"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("**y** | [f64](#f64) This is y"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("\n\nThis is my variant"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("\n\n| Type |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[i32](#i32) integer"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[bool](#bool) boolean"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[string](#string) string"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestQuantity").find("\n\nThis is my quantity"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestQuantity").find("Representation: f32"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestQuantity").find("Unit:  m"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("\n\nThis is my enum"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("\nRepresentation: [u8](#u8)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("first | 0 This is my first value"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("second | 1 This is my second value"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("third | 2 This is my third value"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("\n\n|  |  |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("Element Type | [i64](#i64)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("Bounded | Yes"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("Max size | 100"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestBoundedSequence").find("\n\nThis is my bounded sequence"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestUnboundedSequence").find("\n\n|  |  |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestUnboundedSequence").find("Element Type | [i32](#i32)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestUnboundedSequence").find("Bounded | No"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestUnboundedSequence").find("\n\nThis is my unbounded sequence"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestAlias").find("\n\nThis is my alias"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestAlias").find("Aliased Type: [i64](#i64)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestOptional").find("\n\nThis is my optional"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestOptional").find("Optional Type: [f32](#f32)"), std::string::npos) << content();
}

/// @test
/// Check sen gen mkdocs generates a correct documentation file from STL file with escaped pipe comments
TEST_F(AMkdocsGenerator, StlEscapedPipeComment)
{
  ASSERT_TRUE(generateStl(escapedPipeComment));
  ASSERT_FALSE(content().empty()) << "mkdocs file is empty";
  ASSERT_TRUE(areAllTemplatesFilled()) << "a substitution site was left unfilled\n" << content();
  EXPECT_NE(section("### Type Map").find("[my_test_package.MyTestClass](#my_test_packagemytestclass \"escaped \\|\")"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestStruct](#my_test_packagemyteststruct)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestVariant](#my_test_packagemytestvariant)"), std::string::npos) << content();
  EXPECT_NE(section("### Type Map").find("[MyTestEnum](#my_test_packagemytestenum)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**Properties**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("**prop1**"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestClass").find("[string](#string) escaped \\|"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("\n\n| Field     | Description |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("**x** | [f32](#f32) escaped \\|"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestStruct").find("**y** | [f64](#f64) escaped \\|"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("\n\n| Type |"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[i32](#i32) escaped \\|"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[bool](#bool) escaped \\|"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestVariant").find("[string](#string) escaped \\|"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("\nRepresentation: [u8](#u8)"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("first | 0 escaped \\|"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("second | 1 escaped \\|"), std::string::npos) << content();
  EXPECT_NE(section("### my_test_package.MyTestEnum").find("third | 2 escaped \\|"), std::string::npos) << content();
}
