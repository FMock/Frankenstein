#include "XmlSettings.h"
#include <cctype>
#include <fstream>
#include <sstream>

namespace {

void SkipWhitespaceAndComments(const std::string& s, size_t& pos) {
    while (pos < s.size()) {
        unsigned char c = static_cast<unsigned char>(s[pos]);
        if (std::isspace(c)) {
            ++pos;
        } else if (s.compare(pos, 4, "<!--") == 0) {
            size_t end = s.find("-->", pos + 4);
            if (end == std::string::npos) { pos = s.size(); return; }
            pos = end + 3;
        } else if (s.compare(pos, 5, "<?xml") == 0) {
            size_t end = s.find("?>", pos + 5);
            if (end == std::string::npos) { pos = s.size(); return; }
            pos = end + 2;
        } else {
            break;
        }
    }
}

std::string DecodeEntities(const std::string& in) {
    std::string out;
    out.reserve(in.size());
    for (size_t i = 0; i < in.size(); ) {
        if (in[i] == '&') {
            if (in.compare(i, 4, "&lt;") == 0)        { out += '<';  i += 4; continue; }
            if (in.compare(i, 4, "&gt;") == 0)        { out += '>';  i += 4; continue; }
            if (in.compare(i, 5, "&amp;") == 0)       { out += '&';  i += 5; continue; }
            if (in.compare(i, 6, "&quot;") == 0)      { out += '"';  i += 6; continue; }
            if (in.compare(i, 6, "&apos;") == 0)      { out += '\''; i += 6; continue; }
        }
        out += in[i++];
    }
    return out;
}

std::string EncodeEntities(const std::string& in) {
    std::string out;
    out.reserve(in.size());
    for (char c : in) {
        switch (c) {
            case '<':  out += "&lt;";   break;
            case '>':  out += "&gt;";   break;
            case '&':  out += "&amp;";  break;
            case '"':  out += "&quot;"; break;
            case '\'': out += "&apos;"; break;
            default:   out += c;        break;
        }
    }
    return out;
}

std::string Trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

// Reads one tag starting at s[pos] (which must be '<'). On success, advances pos
// past the closing '>' and fills nameOut. Returns:
//   'O' open tag <Name>, 'C' close tag </Name>, 'S' self-closing <Name/>, 'X' on error.
char ReadTag(const std::string& s, size_t& pos, std::string& nameOut) {
    if (pos >= s.size() || s[pos] != '<') return 'X';
    size_t p = pos + 1;
    bool isClose = false;
    if (p < s.size() && s[p] == '/') { isClose = true; ++p; }
    size_t nameStart = p;
    while (p < s.size() && s[p] != '>' && s[p] != '/' &&
           !std::isspace(static_cast<unsigned char>(s[p]))) {
        ++p;
    }
    if (p == nameStart || p >= s.size()) return 'X';
    nameOut.assign(s, nameStart, p - nameStart);
    // Skip any attributes (unsupported but tolerated).
    while (p < s.size() && s[p] != '>' && s[p] != '/') ++p;
    bool isSelf = false;
    if (p < s.size() && s[p] == '/') { isSelf = true; ++p; }
    if (p >= s.size() || s[p] != '>') return 'X';
    pos = p + 1;
    if (isClose) return 'C';
    return isSelf ? 'S' : 'O';
}

} // namespace

bool XmlSettings::Load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;
    std::ostringstream buf;
    buf << in.rdbuf();
    const std::string s = buf.str();

    m_data.clear();

    size_t pos = 0;
    SkipWhitespaceAndComments(s, pos);

    std::string tag;
    if (ReadTag(s, pos, tag) != 'O' || tag != "Settings") return false;

    while (true) {
        SkipWhitespaceAndComments(s, pos);
        if (pos >= s.size()) return false;

        std::string sectionTag;
        char kind = ReadTag(s, pos, sectionTag);
        if (kind == 'C' && sectionTag == "Settings") break;
        if (kind == 'S') continue;          // empty <Section/> — nothing to add
        if (kind != 'O') return false;

        auto& section = m_data[sectionTag];

        while (true) {
            SkipWhitespaceAndComments(s, pos);
            if (pos >= s.size()) return false;

            std::string keyTag;
            char k = ReadTag(s, pos, keyTag);
            if (k == 'C' && keyTag == sectionTag) break;
            if (k == 'S') { section[keyTag] = ""; continue; }
            if (k != 'O') return false;

            size_t textStart = pos;
            while (pos < s.size() && s[pos] != '<') ++pos;
            std::string text = Trim(s.substr(textStart, pos - textStart));

            std::string closeTag;
            if (ReadTag(s, pos, closeTag) != 'C' || closeTag != keyTag) return false;

            section[keyTag] = DecodeEntities(text);
        }
    }

    return true;
}

bool XmlSettings::Save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Settings>\n";
    for (auto const& sec : m_data) {
        out << "  <" << sec.first << ">\n";
        for (auto const& kv : sec.second) {
            out << "    <" << kv.first << ">"
                << EncodeEntities(kv.second)
                << "</" << kv.first << ">\n";
        }
        out << "  </" << sec.first << ">\n";
    }
    out << "</Settings>\n";
    return out.good();
}

std::string XmlSettings::GetString(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    auto secIt = m_data.find(section);
    if (secIt != m_data.end()) {
        auto keyIt = secIt->second.find(key);
        if (keyIt != secIt->second.end())
            return keyIt->second;
    }
    return defaultValue;
}

int XmlSettings::GetInt(const std::string& section, const std::string& key, int defaultValue) const {
    std::string val = GetString(section, key);
    if (val.empty()) return defaultValue;
    try { return std::stoi(val); } catch (...) { return defaultValue; }
}

float XmlSettings::GetFloat(const std::string& section, const std::string& key, float defaultValue) const {
    std::string val = GetString(section, key);
    if (val.empty()) return defaultValue;
    try { return std::stof(val); } catch (...) { return defaultValue; }
}

void XmlSettings::SetString(const std::string& section, const std::string& key, const std::string& value) {
    m_data[section][key] = value;
}

void XmlSettings::SetInt(const std::string& section, const std::string& key, int value) {
    m_data[section][key] = std::to_string(value);
}

void XmlSettings::SetFloat(const std::string& section, const std::string& key, float value) {
    m_data[section][key] = std::to_string(value);
}
