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
Tag parse_tag(vector<string>&, size_t&);
Tag parse_attribute(vector<string>&, size_t&);

double parse_double(vector<string> &strs, size_t&);
vector<double> parse_tuple(vector<string>&, size_t&);
vector<vector<double>> parse_tuples(vector<string>&, size_t&);

vector<Phys*> parse(string);

bool is_delim(string);
bool in_str(char, const char* str);

Circle* parse_circle(vector<Tag>&, size_t&);
Polygon* parse_polygon(vector<Tag>&);

#endif // COSMOGONY_H
