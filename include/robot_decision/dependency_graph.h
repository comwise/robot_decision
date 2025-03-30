#ifndef __COMWISE__ROBOT_DECISION__DEPENDENCY_GRAPH__H__
#define __COMWISE__ROBOT_DECISION__DEPENDENCY_GRAPH__H__

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

// 基于有向无环图实现
// A->B C D
// B->C D E
// C->D
// D->X
// E->X
// A->B->C->D->E
// 循环依赖会出问题，需优化
class DependencyGraph {
public:
    void addDependency(const std::string& node, const std::string& depend) {
        graph_[node].push_back(depend);
    }

    std::vector<std::string> topologicalSort() {
        std::vector<std::string> result;
        std::unordered_map<std::string, bool> visited;
        for (auto& entry : graph_) {
            const std::string& node = entry.first;
            if (!visited[node]) {
                dfs(node, visited, result);
            }
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

private:
   // depth first search深度优先搜索算法
    void dfs(const std::string& node, 
            std::unordered_map<std::string, bool>& visited,
            std::vector<std::string>& result) {
        visited[node] = true;
        for (auto& dep : graph_[node]) {
            if (!visited[dep]) {
                dfs(dep, visited, result);
            }
        }
        result.push_back(node);
    }

    std::unordered_map<std::string, std::vector<std::string>> graph_;
};

#endif // __COMWISE__ROBOT_DECISION__DEPENDENCY_GRAPH__H__