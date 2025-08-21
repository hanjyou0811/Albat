#include "../library.h"
#include "../libraryflg.h"
#include "../../Utils/stringutils.h"

std::string LibraryManager::extractInsertLibraries(const std::string &str) {
    bool flag;
    std::string result;
    std::string temp;
    std::vector<std::string> currentDependencies;
  
    while (true) {
      flag = false;
      for (const auto &dep : lib_dependencies) {
        const std::string &lib = dep.first;
        currentDependencies = dep.second;
  
        if (insert_is_Still.find(lib) == insert_is_Still.end())
          continue;
  
        if (lib_parents.find(lib) != lib_parents.end() &&
            insert_is_Still.find(lib_parents[lib]) == insert_is_Still.end())
          continue;
  
        for (const auto &dependency : currentDependencies) {
          if (insert_is_Still.find(dependency) == insert_is_Still.end()) {
            flag = true;
            insert_is_Still.insert(dependency);
          }
        }
      }
  
      for (const auto &del : lib_deletions) {
        const std::string &lib = del.first;
        currentDependencies = del.second;
  
        if (insert_is_Still.find(lib) == insert_is_Still.end())
          continue;
  
        if (lib_parents.find(lib) != lib_parents.end() &&
            insert_is_Still.find(lib_parents[lib]) == insert_is_Still.end())
          continue;
  
        for (const auto &deletion : currentDependencies) {
          if (insert_is_Already.find(deletion) == insert_is_Already.end()) {
            flag = true;
            insert_is_Already.insert(deletion);
          }
        }
      }
  
      if (!flag)
        break;
    }
  
    for (const auto &name : lib_names) {
      if (lib_parents.find(name) != lib_parents.end() &&
          insert_is_Still.find(lib_parents[name]) == insert_is_Still.end())
        continue;
      if (insert_is_Still.find(name) != insert_is_Still.end() &&
          insert_is_Already.find(name) == insert_is_Already.end() &&
          lib_positions[name] == str) {
        result += lib_Ident[name];
      }
    }
    StringUtils::trim(result);
    return result;
}

void LibraryManager::requestLibrary(const std::string &name, int already = 0) {
    if(already) {
        insert_is_Already.insert(name);
    } else {
        insert_is_Still.insert(name);
    }
}

void LibraryManager::registerLibrary(
    const std::string& lib_name,
    const std::string& lib_code,
    const std::string& lib_pos,
    const std::vector<std::string>& lib_deps
) {
  if(lib_name.empty() || lib_pos.empty()) {
    return; // Invalid library name or position
  }
  lib_names.push_back(lib_name);
  lib_Ident[lib_name] = lib_code;
  lib_positions[lib_name] = lib_pos;
  insert_is_Still.insert(lib_name);
  if (!lib_deps.empty()) {
      lib_dependencies[lib_name] = lib_deps;
  }
}