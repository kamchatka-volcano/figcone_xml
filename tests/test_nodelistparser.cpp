#include "assert_exception.h"
#include <figcone_tree/errors.h>
#include <figcone_xml/parser.h>
#include <gtest/gtest.h>

namespace test_nodelistparser {

auto parse(const std::string& str)
{
    auto input = std::stringstream{str};
    auto parser = figcone::xml::Parser{};
    return parser.parse(input);
}

TEST(TestNodeListParser, Basic)
{
    auto result = parse(R"(
    <root testStr = "Hello">
        <testNodes>
           <node testInt = "3" />
           <node testInt = "2" />
        </testNodes>
    </root>
    )");

    auto& tree = result.root().asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_TRUE(tree.hasParam("testStr"));
    ASSERT_TRUE(tree.param("testStr").isItem());
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_TRUE(tree.hasNode("testNodes"));
    ASSERT_TRUE(tree.node("testNodes").isList());
    auto& testNodes = tree.node("testNodes").asList();
    ASSERT_EQ(testNodes.size(), 2);
    {
        auto& nodeData = testNodes.at(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.at(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}

TEST(TestNodeListParser, ListLikeSingleElementHasNestedList)
{
    auto result = parse(R"(
    <root testStr = "Hello">
        <listHolder>
            <testNodes>
               <node testInt = "3" />
               <node testInt = "2" />
            </testNodes>
        </listHolder>
    </root>
    )");

    auto& tree = result.root().asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_TRUE(tree.hasParam("testStr"));
    ASSERT_TRUE(tree.param("testStr").isItem());
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_TRUE(tree.hasNode("listHolder"));
    ASSERT_TRUE(tree.node("listHolder").isItem());
    auto& listHolder = tree.node("listHolder").asItem();
    ASSERT_TRUE(listHolder.hasNode("testNodes"));
    ASSERT_TRUE(listHolder.node("testNodes").isList());
    auto& testNodes = listHolder.node("testNodes").asList();
    ASSERT_EQ(testNodes.size(), 2);
    {
        auto& nodeData = testNodes.at(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.at(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}

TEST(TestNodeListParser, TwoNestedLists)
{
    auto result = parse(R"(
    <root testStr = "Hello">
        <listHolder>
            <testNodes>
               <node testInt = "3" />
               <node testInt = "2" />
            </testNodes>
            <testNodes2>
               <node testInt = "3" />
               <node testInt = "2" />
            </testNodes2>
        </listHolder>
    </root>
    )");

    auto& tree = result.root().asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_TRUE(tree.hasParam("testStr"));
    ASSERT_TRUE(tree.param("testStr").isItem());
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_TRUE(tree.hasNode("listHolder"));
    ASSERT_TRUE(tree.node("listHolder").isItem());
    auto& listHolder = tree.node("listHolder").asItem();
    ASSERT_EQ(listHolder.nodesCount(), 2);
    ASSERT_TRUE(listHolder.hasNode("testNodes"));
    ASSERT_TRUE(listHolder.node("testNodes").isList());
    auto& testNodes = listHolder.node("testNodes").asList();
    ASSERT_EQ(testNodes.size(), 2);
    {
        auto& nodeData = testNodes.at(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.at(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
    ASSERT_TRUE(listHolder.hasNode("testNodes2"));
    ASSERT_TRUE(listHolder.node("testNodes2").isList());
    auto& testNodes2 = listHolder.node("testNodes2").asList();
    ASSERT_EQ(testNodes2.size(), 2);
    {
        auto& nodeData = testNodes2.at(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes2.at(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}



TEST(TestNodeListParser, SingleElemenItemOrListAccess)
{
    auto result = parse(R"(
    <root testStr = "Hello">
        <testNodes>
           <node testInt = "3" />
        </testNodes>
    </root>
    )");

    auto& tree = result.root().asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_TRUE(tree.hasParam("testStr"));
    ASSERT_TRUE(tree.param("testStr").isItem());
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_TRUE(tree.hasNode("testNodes"));
    ASSERT_TRUE(tree.node("testNodes").isList());

    auto& testNodes = tree.node("testNodes").asList();
    ASSERT_EQ(testNodes.size(), 1);
    auto& nodeData = testNodes.at(0).asItem();
    ASSERT_EQ(nodeData.paramsCount(), 1);
    ASSERT_TRUE(nodeData.hasParam("testInt"));
    ASSERT_TRUE(nodeData.param("testInt").isItem());
    EXPECT_EQ(nodeData.param("testInt").value(), "3");

    auto& testItem = tree.node("testNodes").asItem();
    ASSERT_EQ(testItem.hasNode("node"), true);
    auto& itemNodeData = testItem.node("node").asItem();
    ASSERT_EQ(itemNodeData.paramsCount(), 1);
    ASSERT_TRUE(itemNodeData.hasParam("testInt"));
    ASSERT_TRUE(itemNodeData.param("testInt").isItem());
    EXPECT_EQ(itemNodeData.param("testInt").value(), "3");
}

TEST(TestNodeListParser, Empty)
{
    auto result = parse(R"(
    <root testStr = "Hello">
        <testNodes>
        </testNodes>
    </root>
    )");

    auto& tree = result.root().asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_TRUE(tree.hasParam("testStr"));
    ASSERT_TRUE(tree.param("testStr").isItem());
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_TRUE(tree.hasNode("testNodes"));
    ASSERT_TRUE(tree.node("testNodes").isList());
    auto& testNodes = tree.node("testNodes").asList();
    ASSERT_EQ(testNodes.size(), 0);
}

TEST(TestNodeListParser, Nested)
{
    auto result = parse(R"(
   <root testDouble = "0.5">
        <testCfg testStr = "Hello">
            <testNodes>
                <node testInt ="3"/>
                <node testInt ="2"/>
            </testNodes>
        </testCfg>
    </root>
    )");

    auto& tree = result.root().asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_TRUE(tree.hasParam("testDouble"));
    ASSERT_TRUE(tree.param("testDouble").isItem());
    EXPECT_EQ(tree.param("testDouble").value(), "0.5");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_TRUE(tree.hasNode("testCfg"));
    auto& testCfg = tree.node("testCfg").asItem();
    ASSERT_EQ(testCfg.paramsCount(), 1);
    ASSERT_TRUE(testCfg.hasParam("testStr"));
    ASSERT_TRUE(testCfg.param("testStr").isItem());
    EXPECT_EQ(testCfg.param("testStr").value(), "Hello");
    ASSERT_EQ(testCfg.nodesCount(), 1);
    ASSERT_TRUE(testCfg.hasNode("testNodes"));
    auto& testNodes = testCfg.node("testNodes").asList();
    ASSERT_EQ(testNodes.size(), 2);
    {
        auto& nodeData = testNodes.at(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.at(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testInt"));
        ASSERT_TRUE(nodeData.param("testInt").isItem());
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}

TEST(TestNodeListParser, NestedCfgList)
{
    auto result = parse(R"(
    <root testStr = "Hello">
        <testList>
            <test testStr = "Hello">
                <testNodes>
                    <node testInt = "3"/>
                    <node testInt = "33"/>
                </testNodes>
            </test>
            <test testStr = "World">
                <testNodes>
                    <node testInt = "5"/>
                </testNodes>
            </test>
        </testList>
    </root>
    )");

    auto& tree = result.root().asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_TRUE(tree.hasParam("testStr"));
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_TRUE(tree.hasNode("testList"));
    ASSERT_TRUE(tree.node("testList").isList());
    auto& testList = tree.node("testList").asList();
    ASSERT_EQ(testList.size(), 2);
    {
        auto& nodeData = testList.at(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testStr"), 1);
        EXPECT_EQ(nodeData.param("testStr").value(), "Hello");
        ASSERT_EQ(nodeData.nodesCount(), 1);
        ASSERT_EQ(nodeData.hasNode("testNodes"), 1);
        auto& testNodes = nodeData.node("testNodes").asList();
        ASSERT_EQ(testNodes.size(), 2);
        {
            auto& listNodeData = testNodes.at(0).asItem();
            ASSERT_EQ(listNodeData.paramsCount(), 1);
            ASSERT_EQ(listNodeData.hasParam("testInt"), 1);
            ASSERT_TRUE(listNodeData.param("testInt").isItem());
            EXPECT_EQ(listNodeData.param("testInt").value(), "3");
        }
        {
            auto& listNodeData = testNodes.at(1).asItem();
            ASSERT_EQ(listNodeData.paramsCount(), 1);
            ASSERT_EQ(listNodeData.hasParam("testInt"), 1);
            ASSERT_TRUE(listNodeData.param("testInt").isItem());
            EXPECT_EQ(listNodeData.param("testInt").value(), "33");
        }
    }
    {
        auto& nodeData = testList.at(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_TRUE(nodeData.hasParam("testStr"));
        ASSERT_TRUE(nodeData.param("testStr").isItem());
        EXPECT_EQ(nodeData.param("testStr").value(), "World");
        ASSERT_EQ(nodeData.nodesCount(), 1);
        ASSERT_TRUE(nodeData.hasNode("testNodes"));
        auto& testNodes = nodeData.node("testNodes").asList();
        ASSERT_EQ(testNodes.size(), 1);
        {
            auto& listNodeData = testNodes.at(0).asItem();
            ASSERT_EQ(listNodeData.paramsCount(), 1);
            ASSERT_TRUE(listNodeData.hasParam("testInt"));
            EXPECT_EQ(listNodeData.param("testInt").value(), "5");
        }
    }
}

TEST(TestNodeParser, NodeListWithAttributesError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parse(R"(
    <root testStr = "Hello">
        <testNodes forbiddenParam="">
           <node testInt = "3" />
           <node testInt = "2" />
        </testNodes>
    </root>)");
            },
            [](const figcone::ConfigError& e)
            {
                ASSERT_EQ(std::string{e.what()}, "Bad node list 'testNodes' - node lists can't contain attributes");
            });
}

TEST(TestNodeListParser, AccessListAsItemError)
{
    auto result = parse(R"(
    <root testStr = "Hello">
        <testNodes>
           <node testInt = "3" />
           <node testInt = "2" />
        </testNodes>
    </root>
    )");

    auto& tree = result.root().asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_TRUE(tree.hasParam("testStr"));
    ASSERT_TRUE(tree.param("testStr").isItem());
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_TRUE(tree.hasNode("testNodes"));
    assert_exception<figcone::ConfigError>(
            [&]
            {
                tree.node("testNodes").asItem();
            },
            [](const figcone::ConfigError& e)
            {
                ASSERT_EQ(std::string{e.what()}, "Bad any node access - trying to get multiple items as a single item");
            });
}

} //namespace test_nodelistparser
