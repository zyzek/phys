#ifndef COSMOGONY_H
#define COSMOGONY_H

#include <fstream>
#include <vector>

#include "phys.h"
#include "circle.h"
#include "polygon.h"

using namespace std;

enum Tag_Type {
    Open_Tag,
    Close_Tag,
    Attribute
};

struct Tag {
    Tag_Type type;
    string attr;
    vector<string> values;
};

vector<string> *tokenise(istream&);
vector<Tag> *parse_tags(vector<string>&);
Tag parse_tag(vector<string>&, int&);
Tag parse_attribute(vector<string>&, int&);
vector<Phys*> parse(string);

bool is_delim(string);
bool in_str(char, const char* str);

Circle* parse_circle(vector<Tag>);
Polygon* parse_polygon(vector<Tag>);

void parsing_main();


#endif // COSMOGONY_H
