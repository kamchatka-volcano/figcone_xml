#ifndef FIGCONE_XML_PARSER_H
#define FIGCONE_XML_PARSER_H

#include <figcone_tree/tree.h>
#include <figcone_tree/iparser.h>

namespace figcone::xml {
class Parser : public IParser {
public:
    TreeNode parse(std::istream& stream) override;
};
}

#endif //FIGCONE_XML_PARSER_H