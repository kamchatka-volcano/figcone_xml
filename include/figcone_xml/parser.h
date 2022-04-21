#pragma once

#include <figcone_tree/tree.h>
#include <figcone_tree/iparser.h>
#include <figcone_tree/errors.h>
#include "detail/paramparser.h"
#include "detail/stream.h"
#include <rapidxml.hpp>
#include <regex>

#include <vector>
#include <string>

namespace figcone::xml::detail {

inline std::string getStreamContent(std::istream& stream)
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

inline void parseXml(const rapidxml::xml_node<char>* xml, figcone::TreeNode& node)
{
    for (auto attr = xml->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
        auto paramList = readParamList(name(attr), value(attr));
        if (paramList)
            node.asItem().addParamList(name(attr), *paramList);
        else
            node.asItem().addParam(name(attr), value(attr));
    }

    for (auto child = xml->first_node(); child != nullptr; child = child->next_sibling()) {
        if (child->first_node() && name(child->first_node()) == "_") {
            if (child->first_attribute())
                throw figcone::ConfigError("Node list '" + name(child) + "' cannot have attributes");
            auto& newNode = node.asItem().addNodeList(name(child));
            for (auto item = child->first_node(); item != nullptr; item = item->next_sibling())
                parseXml(item, newNode.asList().addNode());
        } else {
            auto& newNode = node.asItem().addNode(name(child));
            parseXml(child, newNode);
        }
    }
}

inline figcone::StreamPosition getErrorPosition(std::string& input, const rapidxml::parse_error& error)
{
    auto errorPos = std::distance(input.data(), error.where<char>());
    auto inputStream = std::stringstream{input};
    auto stream = detail::Stream{inputStream};
    for (auto i = 0; i < errorPos; ++i)
        stream.skip(1);
    return stream.position();
}
}

namespace figcone::xml {
class Parser : public IParser {
public:
    TreeNode parse(std::istream& stream) override
    {
        auto input = detail::getStreamContent(stream);
        auto xml = rapidxml::xml_document{};
        try {
            xml.parse<0>(input.data());
        }
        catch (const rapidxml::parse_error& e){
            throw figcone::ConfigError{e.what(), detail::getErrorPosition(input, e)};
        }
        auto tree = figcone::makeTreeRoot();
        detail::parseXml(xml.first_node(), tree);
        return tree;
    }
};

}