#ifndef FIGCONE_XML_PARSER_H
#define FIGCONE_XML_PARSER_H

#include <figcone_tree/iparser.h>
#include <figcone_tree/tree.h>

namespace figcone::xml {
class Parser : public IParser {
public:
    TreeNode parse(std::istream& stream) override;
};
} //namespace figcone::xml

#endif //FIGCONE_XML_PARSER_H