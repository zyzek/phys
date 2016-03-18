#include "cosmogony.h"

#include <QColor>

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>

#define WSPACE " \t\n"
#define DELIM "<>()[],:/"

// Break a stream of text into tokens, where any single delimiter is a token,
// and whitespace separates other tokens.
vector<string> *tokenise(istream &file_stream) {
    vector<string> *tokens = new vector<string>();
    string line, curr_token_val = "";
    bool capturing = false;

    /* If capturing a non-delim token:
     *      If whitespace, the token terminates, save it.
     *      If delimiter, current token terminates, save previous string and delimiter.
     * Otherwise, we're in whitespace:
     *      If delimiter, save it as a new token.
     *      If not whitespace, start a new token.
     *
     * If line's done and still capturing, save the remaining string, and cease to capture.
     */
    while ( getline(file_stream, line) ) {
        // Ignore comments, blank lines.
        int f_index = strspn(line.c_str(), WSPACE);
        if (line.empty() || line[f_index] == '#') continue;

        for (char c : line) {
            if (capturing) {
                if (in_str(c, WSPACE) || in_str(c, DELIM)) {
                    capturing = false;
                    tokens->push_back(curr_token_val);
                    curr_token_val = "";

                    if (in_str(c, DELIM)) tokens->push_back(string(&c, 1));
                }
                else {
                    curr_token_val += c;
                }
            }
            else {
                if (in_str(c, DELIM)) {
                    tokens->push_back(string(&c, 1));
                }
                else if (!in_str(c, WSPACE)) {
                    capturing = true;
                    curr_token_val += c;
                }
            }
        }
        if (capturing) {
            tokens->push_back(string(curr_token_val));
            capturing = false;
        }
    }
    return tokens;
}

void checked_incr(int &j, int size) {
    ++j;
    if (j >= size) throw string("Item opened, never closed.");
}

Tag parse_tag(vector<string> &tokens, int &i) {
    int j = i;
    Tag_Type t_type = Open_Tag;

    // Open a tag.
    if (tokens[j] != "<") throw string("Expected tag opened with <.");
    checked_incr(j, tokens.size());

    // Determine if tag is a closer.
    if (tokens[j] == "/") {
        t_type = Close_Tag;
        checked_incr(j, tokens.size());
    }

    // Tag body must be a single string not containing any delimiters.
    if (is_delim(tokens[j])) {
       if (tokens[j] == ">") throw string("Tag body may not be empty.");
       throw string("Inadmissible symbol " + tokens[j] + " in tag body.");
    }

    Tag new_tag = {t_type, tokens[j]};
    checked_incr(j, tokens.size());

    // Check that the tag closes properly.
    if (tokens[j] != ">") throw string("Tag contains extra symbol " + tokens[j]);

    i = j + 1; // Advance the index if we succeeded.
    return new_tag;
}

Tag parse_attribute(vector<string> &tokens, int &i) {
    int j = i;

    // Open an attribute.
    if (tokens[j] != "[") throw string("Expected attribute opened with [.");
    checked_incr(j, tokens.size());

    // First item must be the attribute's name, containing no delimiters.
    if (is_delim(tokens[j])) {
        if (tokens[j] == "]") throw string("Empty attribute body.");
        throw string("Inadmissible symbol " + tokens[j] + " in attribute name.");
    }

    Tag new_attribute = {Attribute, tokens[j]};

    while (true) {
        checked_incr(j, tokens.size());

        // Valueless attribute.
        if (tokens[j] == "]") {
            i = j + 1;
            return new_attribute;
        }
        // Terminate name.
        else if (tokens[j] == ":") {
            checked_incr(j, tokens.size());
            break;
        }
        else if (is_delim(tokens[j])) {
            throw string("Inadmissible symbol " + tokens[j] + " in attribute name.");
        }

        new_attribute.attr += " " + tokens[j];
    }

    if (tokens[j] == "]") throw string("Attribute value list demanded, but never supplied.");
    else if (tokens[j] == "[") throw string("Attributes may not be nested.");
    new_attribute.values.push_back(tokens[j]);

    while (true) {
        checked_incr(j, tokens.size());

        if (tokens[j] == "]") {
            i = j+1;
            return new_attribute;
        }
        else if (tokens[j] == "[") {
            throw string("Attributes may not be nested.");
        }
        new_attribute.values.push_back(tokens[j]);
    }
}

vector<Tag> *parse_tags(vector<string> &tokens) {
    vector<Tag> *tags = new vector<Tag>();

    int i = 0;

    while (i < tokens.size()) {
        try {
            Tag t = parse_tag(tokens, i);
            tags->push_back(t);
        }
        catch (string msg) {
            try {
                Tag a = parse_attribute(tokens, i);
                tags->push_back(a);
            }
            catch (string msg2) {
                cerr << msg << endl;
                cerr << msg2 << endl;
                delete tags;
                return new vector<Tag>();
            }
        }
    }
    return tags;
}

bool in_str(char d, const char* str) {
    while (*str) {
        if (d == *str) return true;
        ++str;
    }
    return false;
}

bool is_delim(std::string s) {
    return s.length() == 1 && in_str(s[0], DELIM);
}

double parse_double(vector<string> &strs, int &i) {
    try {
        if (i >= strs.size()) throw string("Tried to parse floating point number; none found.");
        double d = stod(strs[i], NULL);
        ++i;
        return d;
    }
    catch (...) {
        throw string("Expected floating point number, found " + strs[i] + " instead.");
    }
}

vector<double> parse_tuple(vector<string> &strs, int &i) {
    vector<double> tuple;
    int j = i;

    if (strs[j] != "(") {
       throw string("Expected ( at start of tuple.");
    }
    if ((j + 1) < strs.size() && strs[j + 1] == ")") {
        throw string("Tuples may not be empty.");
    }

    while (j < strs.size()) {
        checked_incr(j, strs.size());

        try {
            tuple.push_back(parse_double(strs, j));
        }
        catch (string s){
            throw "In tuple: " + s;
        }

        if (strs[j] == ")") {
            i = j + 1;
            return tuple;
        }
        else if (strs[j] != ",") {
           throw string("Expected ',' in tuple.");
        }
    }

    throw string("Tuple never closed");
}

vector<vector<double>> parse_tuples(vector<string> &strs, int &i) {
    int j = i;
    vector<vector<double>> tuples;

    try {
        tuples.push_back(parse_tuple(strs, j));
    }
    catch (string s) {
        throw s;
    }

    while (j < strs.size()) {
       if (strs[j] != ",") {
           i = j;
           return tuples;
       }

       try {
           checked_incr(j, strs.size());
           tuples.push_back(parse_tuple(strs, j));
       }
       catch (string s) {
           i = j;
           return tuples;
       }

    }

    return tuples;

}


Circle* parse_circle (vector<Tag> &tags, int &i) {
    static int count = 0;
    string name = "CIRCLE" + to_string(count);

    Vec position(0,0), velocity(0,0);
    float elasticity = 0.98;
    float scale = 1.0;
    float radius = 1.0, mass = 1.0, density = 1.0;
    bool rad_set = false, mass_set = false, dens_set = false;
    float angle = 0.0, angular_velocity = 0.0;

    string fill_color = "#ff00ff", stroke_color = "#000000";
    bool fill_set = false, stroke_set = false;

    bool is_static = false, is_physical = false, is_invisible = false;

    int j = i;

    // Opening tag
    if (tags[j].type != Open_Tag && tags[j].attr == "circle") {
        throw string("No opening tag for circle.");
    }

    checked_incr(j, tags.size());

    // Parse all attributes.
    while (j < tags.size()) {
        if (tags[j].type != Attribute) break;
        int k = 0;

        if (tags[j].attr == "name") {
            name = "";
            for (k = 0; k < tags[j].values.size(); ++k) {
                name += tags[j].values[k] + ((k != tags[j].values.size() - 1) ? " " : "");
            }
        }
        else if (tags[j].attr == "position") {
            vector<double> pos = parse_tuple(tags[j].values, k);
            if (pos.size() != 2) throw string("Position vector must have arity 2.");
            position = Vec(pos[0], pos[1]);
        }
        else if (tags[j].attr == "velocity") {
            vector<double> vel = parse_tuple(tags[j].values, k);
            if (vel.size() != 2) throw string("Velocity vector must have arity 2.");
            velocity = Vec(vel[0], vel[1]);
        }
        else if (tags[j].attr == "density") {
            density = parse_double(tags[j].values, k);
            dens_set = true;

            if (rad_set) {
                mass = PI*radius*radius*density;
                mass_set = true;
            }
            else if (mass_set) {
                radius = sqrt(mass/(density*PI));
                rad_set = true;
            }
        }
        else if (tags[j].attr == "mass") {
            mass = parse_double(tags[j].values, k);
            mass_set = true;

            if (rad_set) {
                density = mass/(PI*radius*radius);
                dens_set = true;
            }
            else if (dens_set) {
                radius = sqrt(mass/(PI*density));
                rad_set = true;
            }
        }
        else if (tags[j].attr == "radius") {
            radius = parse_double(tags[j].values, k);
            rad_set = true;

            if (dens_set) {
                mass = PI*radius*radius*density;
                mass_set = true;
            }
            else if (mass_set) {
                density = mass/(PI*radius*radius);
                dens_set = true;
            }
        }
        else if (tags[j].attr == "diameter") {
            radius = parse_double(tags[j].values, k)/2;
            rad_set = true;

            if (dens_set) {
                mass = PI*radius*radius*density;
                mass_set = true;
            }
            else if (mass_set) {
                density = mass/(PI*radius*radius);
                dens_set = true;
            }
        }
        else if (tags[j].attr == "scale") {
            scale = parse_double(tags[j].values, k);
        }
        else if (tags[j].attr == "angle") {
            angle = parse_double(tags[j].values, k);
        }
        else if (tags[j].attr == "angular velocity") {
            angular_velocity = parse_double(tags[j].values, k);
        }
        else if (tags[j].attr == "elasticity") {
            elasticity = parse_double(tags[j].values, k);
        }
        else if (tags[j].attr == "fill color") {
            if (tags[j].values.empty()) throw string("No fill colour supplied.");
            fill_color = tags[j].values[0];
            if (!stroke_set) {
                stroke_color = fill_color;
            }
            fill_set = true;
        }
        else if (tags[j].attr == "stroke color") {
            if (tags[j].values.empty()) throw string("No stroke colour supplied.");
            stroke_color = tags[j].values[0];
            if (!fill_set) {
                fill_color = stroke_color;
            }
            stroke_set = true;
        }
        else if (tags[j].attr == "static") {
            is_static = true;
        }
        else if (tags[j].attr == "physical") {
            is_physical = true;
        }
        else if (tags[j].attr == "invisible") {
            is_invisible = true;
        }
        else {
            throw string("Invalid tag for circles: " + tags[j].attr);
        }

        checked_incr(j, tags.size());
    }

    // Closing tag
    if (tags[j].type != Close_Tag) {
        throw string("Circle tag not closed for " + name + ".");
    }
    else if (tags[j].attr != "circle") {
        throw string("Circle closed with incorrect tag: " + tags[j].attr);
    }

    Circle *c = new Circle(position, radius*scale, mass);
    c->name = name;
    c->fill_color = QColor(fill_color.c_str());
    c->stroke_color = QColor(stroke_color.c_str());
    c->pos = position;
    c->vel = velocity;
    c->elasticity = elasticity;
    c->angle = angle;
    c->ang_vel = angular_velocity;
    c->is_static = is_static;
    c->is_physical = is_physical;

    i = j + 1;
    return c;
}

Polygon* parse_polygon(vector<Tag> &tags) {
    throw string("Parser not implemented yet.");
    return NULL;
}
