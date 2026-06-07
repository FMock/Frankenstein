#pragma once
#include <string>
#include <map>
#include "FrankensteinAPI.h"

/* XmlSettings
 * Minimal reader/writer for a flat two-level XML settings file of the form:
 *   <Settings>
 *     <Section>
 *       <Key>value</Key>
 *       ...
 *     </Section>
 *     ...
 *   </Settings>
 * No attributes, no nested elements inside a key, no namespaces.
 */
class FRANKENSTEIN_API XmlSettings {
public:
    bool Load(const std::string& filename);
    bool Save(const std::string& filename) const;

    std::string GetString(const std::string& section, const std::string& key, const std::string& defaultValue = "") const;
    int GetInt(const std::string& section, const std::string& key, int defaultValue = 0) const;
    float GetFloat(const std::string& section, const std::string& key, float defaultValue = 0.0f) const;

    void SetString(const std::string& section, const std::string& key, const std::string& value);
    void SetInt(const std::string& section, const std::string& key, int value);
    void SetFloat(const std::string& section, const std::string& key, float value);

private:
    std::map<std::string, std::map<std::string, std::string>> m_data;
};
