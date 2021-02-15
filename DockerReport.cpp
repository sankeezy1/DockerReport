/*
    DockerReport.cpp

    Produces a report with various counts of the parts
    of a DockerCompose file.

    Input is an YAML file in the Docker Compose format.

    Limitations:
    * Much of YAML is not supported
*/
#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>
int main() {
    bool invalue = false;
    bool inversion = false;
    int key_count = 0;
    int ubuntu_count = 0;
    int fedora_count = 0;
    int centos_count = 0;
    int opensuse_count = 0;
    std::string version;
    std::ostringstream sstream;
    sstream << std::cin.rdbuf();
    std::string buffer = sstream.str();
    std::string::const_iterator pc = buffer.cbegin();
    while (true) {
        if (pc == buffer.cend()) {
            break;
        } else if (!invalue && isalpha(*pc)) {
            // parse key
            std::string::const_iterator endpc = buffer.cend();
            std::string::const_iterator endname = std::find_if_not(pc, endpc, [] (char c) { return isalnum(c) || c == '_' || c == '.'; });
            if (endname == endpc)
                return 1;
            std::string name(pc, endname);
            if (name == "version") {
                inversion = true;
            } else {
                ++key_count;
                std::string name_prefix = name.substr(0, name.find('_'));
                if (name_prefix == "ubuntu") {
                    ++ubuntu_count;
                } else if (name_prefix == "fedora") {
                    ++fedora_count;
                } else if (name_prefix == "centos") {
                    ++centos_count;
                } else if (name_prefix == "opensuse") {
                    ++opensuse_count;
                }
            }
            pc = endname;
            pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
            if (*pc != ':')
                return 1;
            std::advance(pc, 1);
            pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
            invalue = true;
        } else if (invalue && (isalpha(*pc) || *pc == '"')) {
            // parse value
            std::string::const_iterator endpc = buffer.cend();
            pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
            if (*pc == '"')
                std::advance(pc, 1);
            std::string::const_iterator valueend = std::find_if(pc, endpc, [] (char c) { return c == '"' || c == ' ' || c == '\n'; });
            std::string value(pc, valueend);
            if (inversion) {
                version = value;
                inversion = false;
            }
            pc = valueend;
            if (*pc == '"')
                std::advance(pc, 1);
            pc = std::find_if_not(pc, endpc, [] (char c) { return isspace(c); });
            invalue = false;
        } else {
            std::advance(pc, 1);
        }
    }
    std::cout << "# Docker Report: version " << version << '\n';
    std::cout << "| Platform | Count |\n";
    std::cout << "|:-----|-----:|\n";
    std::cout << "| all | " << key_count << " |\n";
    std::cout << "| ubuntu | " << ubuntu_count << " |\n";
    std::cout << "| fedora | " << fedora_count << " |\n";
    std::cout << "| centos | " << centos_count << " |\n";
    std::cout << "| opensuse | " << opensuse_count << " |\n";
    return 0;
}
