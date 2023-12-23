#include "paramparser.h"
#include "stream.h"
#include <rapidxml.hpp>
#include <figcone_tree/errors.h>
#include <figcone_xml/parser.h>
#include <iterator>
#include <optional>
#include <regex>
#include <string>
#include <vector>

namespace figcone::xml::detail {
namespace {
std::string getStreamContent(std::istream& stream)
{
    stream >> std::noskipws;
    return {std::istream_iterator<char>{stream}, std::istream_iterator<char>{}};
}

template<typename T>
std::string name(T node)
{
    return {node->name(), node->name_size()};
}

template<typename T>
std::string value(T& node)
{
    return {node->value(), node->value_size()};
}

template<typename T>
bool hasAttributes(T& node)
{
    return node.first_attribute() != nullptr;
}

struct ListNodeDetectionTraits {
    bool withAttributes = false;
    int childrenCount;
    bool childrenHaveSameName = false;
};

template<typename T>
ListNodeDetectionTraits detectListNode(T& node)
{
    const auto withAttributes = hasAttributes(node);
    const auto [childrenCount, childrenHaveSameName] = [&]() -> std::tuple<int, bool>
    {
        auto count = 0;
        std::optional<std::string> childName;
        for (auto child = node.first_node(); child != nullptr; child = child->next_sibling()) {
            count++;
            if (!childName.has_value()) {
                childName = name(child);
                continue;
            }
            else if (childName.value() != name(child))
                return {count, false};
        }
        return {count, childName.has_value()};
    }();

    return {withAttributes, childrenCount, childrenHaveSameName};
}

void parseXml(const rapidxml::xml_node<char>* xml, figcone::TreeNode& node)
{
    const auto [withAttributes, childrenCount, childrenHaveSameName] = detectListNode(*xml);
    if (withAttributes && childrenCount > 1 && childrenHaveSameName)
        throw ConfigError{"Bad node list '" + name(xml) + "' - node lists can't contain attributes"};

    const auto isListNode = !withAttributes && childrenHaveSameName;
    if (isListNode) {
        if (node.isRoot())
            throw ConfigError{"Root node can't be a node list"};

        for (auto child = xml->first_node(); child != nullptr; child = child->next_sibling()) {
            parseXml(child, node.asList().emplaceBackAny(name(child)));
        }
    }
    else {
        for (auto attr = xml->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
            auto paramList = readParamList(name(attr), value(attr));
            if (paramList)
                node.asItem().addParamList(name(attr), *paramList);
            else
                node.asItem().addParam(name(attr), value(attr));
        }
        for (auto child = xml->first_node(); child != nullptr; child = child->next_sibling()) {
            auto& newNode = node.asItem().addAny(name(child));
            parseXml(child, newNode);
        }
    }
}

figcone::StreamPosition getErrorPosition(std::string& input, const rapidxml::parse_error& error)
{
    auto errorPos = std::distance(input.data(), error.where<char>());
    auto inputStream = std::stringstream{input};
    auto stream = detail::Stream{inputStream};
    for (auto i = 0; i < errorPos; ++i)
        stream.skip(1);
    return stream.position();
}
} //namespace
} //namespace figcone::xml::detail

namespace figcone::xml {
Tree Parser::parse(std::istream& stream)
{
    auto input = detail::getStreamContent(stream);
    auto xml = rapidxml::xml_document{};
    try {
        xml.parse<0>(input.data());
    }
    catch (const rapidxml::parse_error& e) {
        throw figcone::ConfigError{e.what(), detail::getErrorPosition(input, e)};
    }
    auto treeRoot = figcone::makeTreeRoot();
    detail::parseXml(xml.first_node(), *treeRoot);
    auto tree = Tree{std::move(treeRoot)};
    return tree;
}

} //namespace figcone::xml