#include "cosmogony.h"

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>

#define WSPACE " \t\n"
#define DELIM "<>()[],:/"

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

Tag parse_tag(vector<string> &tokens, int &i) {
    int j = i;
    Tag_Type t_type = Open_Tag;

    // Open a tag.
    if (tokens[j] != "<") throw string("Expected tag opened with <.");
    ++j;

    // Determine if tag is a closer.
    if (tokens[j] == "/") {
        t_type = Close_Tag;
        ++j;
    }

    // Tag body must be a single string not containing any delimiters.
    if (is_delim(tokens[j])) {
       if (tokens[j] == ">") throw string("Tag body may not be empty.");
       throw string("Inadmissible symbol " + tokens[j] + " in tag body.");
    }

    Tag new_tag = {t_type, tokens[j]};
    ++j;

    // Check that the tag closes properly.
    if (tokens[j] != ">") throw string("Tag contains extra symbol " + tokens[j]);

    i = j + 1; // Advance the index if we succeeded.
    return new_tag;
}

Tag parse_attribute(vector<string> &tokens, int &i) {
    int j = i;

    // Open an attribute.
    if (tokens[j] != "[") throw string("Expected attribute opened with [.");
    ++j;
    if (j >= tokens.size()) throw string("Attribute never closed.");

    // First item must be the attribute's name, containing no delimiters.
    if (is_delim(tokens[j])) {
        if (tokens[j] == "]") throw string("Empty attribute body.");
        throw string("Inadmissible symbol " + tokens[j] + " in attribute name.");
    }

    Tag new_attribute = {Attribute, tokens[j]};

    while (true) {
        ++j;
        if (j >= tokens.size()) throw string("Attribute never closed.");

        // Valueless attribute.
        if (tokens[j] == "]") {
            i = j + 1;
            return new_attribute;
        }
        // Terminate name.
        else if (tokens[j] == ":") {
            ++j;
            if (j >= tokens.size()) throw string("Attribute never closed.");
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
        ++j;
        if (j >= tokens.size()) throw string("Attribute never closed.");

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


void parsing_main() {
    ifstream in_file("~/code/cqt/phys/universe.txt");

    if (in_file.is_open())
    {
        vector<string> *tokens = tokenise(in_file);
        vector<Tag> *tags = parse_tags(*tokens);
        cout << tags->size() << endl;

        for (int i = 0; i < tokens->size(); ++i) {
            cout << (*tokens)[i] << endl;
        }

        for (int i = 0; i < tags->size(); ++i) {
            cout << (*tags)[i].type << " : " << (*tags)[i].attr << " : ";

            for (string s : (*tags)[i].values) {
                cout << s << " ";
            }

            cout << endl;
        }

        delete tokens;
        delete tags;
        in_file.close();

    }
    else {
        cout << "File never opened." << endl;
    }
}

Circle* parse_circle(std::ifstream file_stream) {
    std::cout << "Unclosed <circle> tag within input file." << std::endl;
    return NULL;
}
