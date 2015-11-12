/*!
 * @file 		utils.h
 * @author 		Zdenek Travnicek <travnicek@iim.cz>
 * @date 		12. 11. 2015
 * @copyright	Institute of Intermedia, CTU in Prague, 2013
 * 				Distributed under BSD Licence, details in file doc/LICENSE
 *
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <memory>
#include <functional>

namespace iim {

//template<class T>
//using managed_resource = std::unique_ptr<T, std::function<void(T*)>>;

template<class T>
class managed_resource {
public:
	using value_type = T;
	using deleter_type = std::function<void(value_type&)>;
	managed_resource() = default;
	managed_resource(managed_resource&& rhs):
		data_(std::move(rhs.data_)),del_(std::move(rhs.del_)) {
		rhs.del_ = deleter_type{};
	}
	managed_resource(const managed_resource&) = delete;

	managed_resource(value_type v, deleter_type d):
			data_(std::move(v)),del_(std::move(d)) {}

	template<class T2, class D>
	managed_resource(T2&& v, D&& d):
		data_(std::forward<T2>(v)), del_(std::forward<D>(d)) {}

	managed_resource& operator=(const managed_resource&) = delete;
	managed_resource& operator=(managed_resource&& rhs)
	{
		reset();
		data_ = std::move(rhs.data_);
		del_ = std::move(rhs.del_);
		rhs.del_ = deleter_type{};
		return *this;
	}
	~managed_resource() {
		reset();
	}

	void reset()
	{
		if (del_) del_(data_);
	}

	const T& get() const { return data_; }
	T& get() { return data_; }

private:
	T data_;
	deleter_type del_;

};




}



#endif /* UTILS_H_ */
