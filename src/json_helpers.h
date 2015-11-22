/*!
 * @file 		json_helpers.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		20. 10. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef CXX_SRC_JSON_HELPERS_H_
#define CXX_SRC_JSON_HELPERS_H_

#include "json/json.h"

namespace iim {

inline const Json::Value& get_nested_element(const Json::Value& root) {
  return root;
}

template <class... Ts>
const Json::Value& get_nested_element(const Json::Value& root,
                                      Json::ArrayIndex idx, Ts... idxs) {
  if (root.isArray() and root.size() > idx) {
    return get_nested_element(root[idx], std::forward<Ts...>(idxs...));
  }
  throw std::runtime_error("Bad index");
}

template <class... Ts>
const Json::Value& get_nested_element(const Json::Value& root,
                                      const std::string& idx, Ts... idxs) {
  if (root.isMember(idx)) {
    return get_nested_element(root[idx], idxs...);
  }
  throw std::runtime_error("Bad index");
}

template<class... Ts>
std::string get_nested_value_or_default(const Json::Value& root, const std::string& def_value, Ts... idxs)
{
	try {
		auto v = get_nested_element(root, idxs...);
		return v.asString();
	}
	catch(std::runtime_error&)
	{}
	return def_value;
}

template<class... Ts>
std::string get_nested_value_or_default(const Json::Value& root, const char* def_value, Ts... idxs)
{
	return (get_nested_value_or_default(root, std::string{def_value}, idxs...));
}

template<class... Ts>
bool get_nested_value_or_default(const Json::Value& root, const bool& def_value, Ts... idxs)
{
	try {
		auto v = get_nested_element(root, idxs...);
		return v.asBool();
	}
	catch(std::runtime_error&)
	{}
	return def_value;
}

template<class T, class... Ts>
typename std::enable_if<std::is_integral<T>::value, Json::LargestInt>::type
get_nested_value_or_default(const Json::Value& root, const T& def_value, Ts... idxs)
{
	try {
		auto v = get_nested_element(root, idxs...);
		return v.asLargestInt();
	}
	catch(std::runtime_error&)
	{}
	return def_value;
}
template<class T, class... Ts>
typename std::enable_if<std::is_floating_point<T>::value, double>::type
get_nested_value_or_default(const Json::Value& root, const T& def_value, Ts... idxs)
{
	try {
		auto v = get_nested_element(root, idxs...);
		return v.asDouble();
	}
	catch(std::runtime_error&)
	{}
	return def_value;
}

}

#endif /* CXX_SRC_JSON_HELPERS_H_ */
