#include "CfgVar.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

/*
 * JSON Parser
JSON stands for JavaScript Object Notation.
 The property tree dataset is not typed, and does not support arrays as such. Thus, the following JSON / property tree mapping is used:
1. JSON objects are mapped to nodes. Each property is a child node.
2. JSON arrays are mapped to nodes. Each element is a child node with an empty name. If a node has both named and unnamed child nodes, it cannot be mapped to a JSON representation.
3. JSON values are mapped to nodes containing the value. However, all type information is lost; numbers, as well as the literals "null", "true" and "false" are simply mapped to their string form.
4. Property tree nodes containing both child nodes and data cannot be mapped.
5. JSON round-trips, except for the type information loss.

For example this JSON:

{
   "menu":
   {
      "foo": true,
      "bar": "true",
      "value": 102.3E+06,
      "popup":
      [
         {"value": "New", "onclick": "CreateNewDoc()"},
         {"value": "Open", "onclick": "OpenDoc()"},
      ]
   }
}
will be translated into the following property tree:

menu
{
   foo true
   bar true
   value 102.3E+06
   popup
   {
      ""
      {
         value New
         onclick CreateNewDoc()
      }
      ""
      {
         value Open
         onclick OpenDoc()
      }
   }
}
 * */

static string print(const pt::ptree &tree, const int step) {
  string prefix = "@ ";
  for (int idx = 0; idx < step; ++idx)
    prefix += "\t@ ";

  string res = (tree.data().empty() ? "NULL" : tree.data()) + "\n";
  for (auto it = tree.begin(); it != tree.end(); ++it) { // 访问所有子树,可能不存在
    res += prefix + (it->first.empty() ? "NULL" : it->first) + ": "; // 子树的根节点
    res += print(it->second, step+1);
  }
  return res;
}

// 搜集所有key符合pathitem的元素,因此结果为resvec
static void getvalues(const pt::ptree &tree, const vector<string> &pathitem, vector<string> &resvec) {
  if (pathitem.empty()) { // 路径解析完毕,需要获取该节点数据
    resvec.push_back(tree.data());
    return;
  }
  // 现在pathitem非空
  if (tree.empty()) // 没有子树,中断该支线的查找
    return;

  // 现在pathitem和tree均非空
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    if (it->first != pathitem[0]) // 子树不匹配
      continue;
    // 匹配一级目录,递归查找下一级目录
    getvalues(it->second, vector<string>(pathitem.begin() + 1, pathitem.end()), resvec);
  }
}

// 搜集所有key符合pathitem的向量,因此结果为resvecvec
static void getvaluevecs(const pt::ptree &tree, const vector<string> &pathitem, vector<vector<string>> &resvecvec) {
  if (pathitem.empty()) { // 路径解析完毕,需要获取该节点下的向量
    if (tree.empty()) // 没有子树
      return;

    // 可以搜集下一层""对应的元素,即向量的元素,这是和getvalue的不同之处
    vector<string> strvec;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
      if (it->first != "") // 不是向量
        continue;
      // 可以访问向量数据
      strvec.push_back(it->second.data());
    }
    resvecvec.push_back(strvec);
    return;
  }
  // 现在pathitem非空
  if (tree.empty()) // 没有子树,中断该支线的查找
    return;

  // 现在pathitem和tree均非空
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    if (it->first != pathitem[0]) // 子树不匹配
      continue;
    // 匹配一级目录,递归查找下一级目录
    getvaluevecs(it->second, vector<string>(pathitem.begin() + 1, pathitem.end()), resvecvec);
  }
}

CfgVar::CfgVar(string jsonFileName) : jsonFileName_(jsonFileName) {
  LOG(INFO) << "CfgVar ctor for " << jsonFileName_;
  vector<string> filenames = {jsonFileName_, "../"+jsonFileName_, "models/"+jsonFileName_};
  bool valid = false;
  for (auto &&fn : filenames) {
    try {
      pt::json_parser::read_json(fn, tree_);
      valid = true;
      break;
    } catch (std::runtime_error &error) {
      LOG(INFO) << error.what();
    }
  }
  CHECK(valid);
}

string CfgVar::print() {
  return ::print(tree_, 0);
}

#include "Mix.hpp"
vector<string> CfgVar::get(vector<string> path, bool failcheck) {
  LOG(INFO) << "size = " << path.size() << ", (" << convert_fromVector(path) << ")";
  vector<string> resvec;
  ::getvalues(tree_, path, resvec);
  // 要么不检查,要么返回有值,否则退出
  CHECK(!failcheck || !resvec.empty()) << "can not find " << convert_fromVector(path) << " in " << jsonFileName_;
  return resvec;
}

vector<vector<string>> CfgVar::getvec(vector<string> path, bool failcheck) {
  LOG(INFO) << "size = " << path.size() << ", (" << convert_fromVector(path) << ")";
  vector<vector<string>> resvecvec;
  ::getvaluevecs(tree_, path, resvecvec);
  // 要么不检查,要么返回有值,否则退出
  CHECK(!failcheck || !resvecvec.empty()) << "can not find " << convert_fromVector(path) << " in " << jsonFileName_;
  return resvecvec;
}
