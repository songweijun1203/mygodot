#pragma once

#include <memory>
#include "mytype.h"
#include "xutils.h"

template<typename T>
using sptr = std::shared_ptr<T>;

template<typename T>
using csptr =const std::shared_ptr<T>;

template<typename T>
using wptr = std::weak_ptr<T>;

template<typename object,typename ...Args>
std::shared_ptr<object> makeShareObject(Args&& ...args){
    return std::make_shared<object>(std::forward<Args>(args)...);
}


class XBaseObject : public std::enable_shared_from_this<XBaseObject> {
protected:
	XBaseObject(){}
	virtual ~XBaseObject(){}
public:
	template<typename Derived>
	sptr<const Derived> asDerived() const {
		return std::dynamic_pointer_cast<const Derived>(this->shared_from_this());
	}

	template<typename Derived>
	sptr<Derived> asDerived() {
		return std::dynamic_pointer_cast<Derived>(this->shared_from_this());
	}
};
