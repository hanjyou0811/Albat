# ifndef LIBRARY_HPP
# define LIBRARY_HPP

# include <string>
# include <vector>
# include <set>
# include <map>

class LibraryManager {
public:
    static LibraryManager& getInstance() {
        static LibraryManager instance;
        static int initialized = 0;
        if (!initialized) {
            instance.init();
            initialized = 1;
        }

        return instance;
    }

    void init();    
    
    std::string extractInsertLibraries(const std::string &pos);
    std::string getLibraryIdentifier(const std::string &name);
    void requestLibrary(const std::string &name, int already);
    void clearRequests();

    void switch_Library(std::string lib_name, int ok = 1){
        if (ok) {
            Lib_use[lib_name] = 1;
        }else{
            Lib_use[lib_name] = 0;
        }
    }
    int CanUseLibrary(std::string lib_name)
    {
        return Lib_use[lib_name];
    }

    void switch_g_Lib(std::string lib_name, int ok = 1){
        if (ok) {
            g_Lib[lib_name] = 1;
        }else{
            g_Lib[lib_name] = 0;
        }
    }
    int CanUse_g_Lib(std::string lib_name)
    {
        return g_Lib[lib_name];
    }

    void insertLibrary(std::string lib_name, std::string lib_code, std::string lib_pos, std::vector<std::string> lib_deps = {})
    {
        registerLibrary(lib_name, lib_code, lib_pos, lib_deps);
    }

private:
    LibraryManager() = default;
    const std::string HEAD = "head";
    const std::string MAIN = "header_of_main";
    std::map<std::string, int> Lib_use, g_Lib;

    std::vector<std::string> lib_names;
    std::set<std::string> insert_is_Still, insert_is_Already;
    std::map<std::string, std::string> lib_Ident, lib_positions, lib_parents;
    std::map<std::string, std::vector<std::string>> lib_dependencies;
    std::map<std::string, std::vector<std::string>> lib_deletions;
    
    void registerLibrary(
        const std::string& lib_name,
        const std::string& lib_code,
        const std::string& lib_pos,
        const std::vector<std::string>& lib_deps = {}
    );
        
    void initBasicSettings();
    void initMathLibraries();
    void initDataStructures();
    void initUtilityFunctions();
    void initModIntVariants();
    
    void processDependencies();
    bool processLibraryDependencies();
    bool processDeletionRules();
};

#endif