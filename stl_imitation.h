#include <iostream>
#include <stdlib.h>
#include <stdexcept>

#define STLIMI_BEGIN namespace stlimi{
#define STLIMI_END }

STLIMI_BEGIN

template<typename Ty>
class vector {
private:
	Ty* elements;
	size_t siz;
	size_t cap;
public:
	vector() :siz(0), cap(1), elements(new Ty[1]) {
		if (elements == nullptr)throw std::bad_alloc();
	}
	vector(size_t cnt) {
		siz = cnt;
		cap = siz == 0 ? 1 : siz << 1;
		elements = new Ty[cap];
		if (elements == nullptr)throw std::bad_alloc();
		for (size_t i = 0; i < cnt; ++i)elements[i] = Ty();
	}
	vector(const vector<Ty>& copie) { operator= (copie); }
	vector<Ty>& operator=(const vector<Ty>& copie) {
		siz = copie.siz;
		cap = copie.cap;
		elements = new Ty[cap];
		if (elements == nullptr)throw std::bad_alloc();
		for (size_t i = 0; i < siz; ++i)elements[i] = copie.elements[i];
		return *this;
	}
	vector(size_t cnt, const Ty& val) {
		siz = cap = cnt;
		elements = new Ty[cap];
		if (elements == nullptr)throw std::bad_alloc();
		for (size_t i = 0; i < siz; ++i)elements[i] = val;
	}
	~vector() { delete[] elements; }
	size_t size() { return siz; }
	Ty& operator[](size_t idx) {
		if (idx >= siz)throw std::out_of_range("");
		return elements[idx];
	}
	void assign(size_t newsize, const Ty& val) {
		reserve(newsize);
		siz = newsize;
		for (size_t i = 0; i < siz; ++i)elements[i] = val;
	}
	void reserve(size_t newcapacity) {
		if (newcapacity <= cap)return;
		cap = newcapacity;
		Ty* temp = new Ty[newcapacity];
		if (temp == nullptr)throw std::bad_alloc();
		for (size_t i = 0; i < siz; ++i)temp[i] = elements[i];
		delete[] elements;
		elements = temp;
	}
	void push_back(const Ty& val) {
		if (cap == siz)reserve(cap << 1);
		elements[siz] = val;
		++siz;
	}
	void pop_back() {
		if (siz <= 0)throw std::out_of_range("");
		--siz;
	}
	Ty& front() {
		if (siz <= 0)throw std::out_of_range("");
		return elements[0];
	}
	Ty& back() {
		if (siz <= 0)throw std::out_of_range("");
		return elements[siz - 1];
	}
	const Ty* data()const { return elements; }
};

template<typename Ty>
class list {
private:
	struct _node {
		Ty val;
		_node* prev, * next;
		bool is_end;
		_node() :is_end(true), val(Ty()), prev(this), next(this) {}
		_node(const Ty& val, _node* prev, _node* next) :is_end(false), val(val), prev(prev), next(next) {}
	};
	_node* headptr;
	size_t siz;
public:
	class iterator {
	public:
		_node* ptr;
		iterator(_node* ptr) :ptr(ptr) {}
		iterator& operator++() {
			if (ptr->is_end)throw std::out_of_range("");
			ptr = ptr->next;
			return *this;
		}
		iterator& operator--() {
			ptr = ptr->prev;
			if (ptr->is_end)throw std::out_of_range("");
			return *this;
		}
		Ty& operator*() {
			if (ptr->is_end)throw std::out_of_range("");
			return ptr->val;
		}
		bool operator==(const iterator& other)const { return ptr == other.ptr; }
		bool operator!=(const iterator& other)const { return ptr != other.ptr; }
	};
	list() :headptr(new _node()), siz(0) {}
	~list() {
		_node* ptr = headptr->next;
		while (ptr != headptr) {
			_node* temp = ptr;
			ptr = ptr->next;
			delete temp;
		}
		delete headptr;
	}
	list(const list<Ty>& other) { operator=(other); }
	list<Ty>& operator=(const list<Ty>& other) {
		headptr = new _node();
		_node* optr = other.headptr->next, * mptr = headptr;
		while (optr != other.headptr) {
			mptr = new _node(optr->val, mptr, nullptr);
			mptr->prev->next = mptr;
		}
		mptr->next = headptr;
		return *this;
	}
	void push_back(const Ty& val) {
		_node* nptr = new _node(val, headptr->prev, headptr);
		nptr->prev->next = nptr;
		nptr->next->prev = nptr;
		++siz;
	}
	void push_front(const Ty& val) {
		_node* nptr = new _node(val, headptr, headptr->next);
		nptr->prev->next = nptr;
		nptr->next->prev = nptr;
		++siz;
	}
	size_t remove(const Ty& val) {
		_node* ptr = headptr->next;
		size_t cnt = 0;
		while (ptr != headptr) {
			if (ptr->val == val) {
				ptr = _erase(ptr);
				--siz; ++cnt;
			}
		}
		return cnt;
	}
	iterator erase(const iterator& it) {
		return iterator(_erase(it->ptr));
	}
	iterator begin()const { return iterator(headptr->next); }
	iterator end()const { return iterator(headptr); }
private:
	_node* _erase(_node* ptr) {
		ptr->prev->next = ptr->next;
		ptr->next->prev = ptr->prev;
		_node* ret = ptr->next;
		delete ptr;
		return ret;
	}
};

template<typename char_type>
class _string {
private:
	char_type* elements;
	size_t siz;
	size_t cap;
public:
	_string() :siz(0), cap(1), elements(new char_type[1]) {
		if (elements == nullptr)throw std::bad_alloc();
	}
	_string(const char_type* s) {
		siz = 0;
		while (s[siz] != char_type('\0'))++siz;
		cap = siz;
		elements = new char_type[cap];
		if (elements == nullptr)throw std::bad_alloc();
		for (int i = 0; i < siz; ++i)elements[i] = s[i];
	}
	_string(const _string& s) {
		elements = new char_type[1];
		cap = 0;
		operator=(s);
	}
	_string(size_t cnt, char_type val) {
		siz = cnt;
		cap = cnt;
		elements = new char_type[cap];
		if (elements == nullptr)throw std::bad_alloc();
		for (size_t i = 0; i < siz; ++i)elements[i] = val;
	}
	~_string() {delete[] elements; }
	size_t length()const { return siz; }
	char_type& operator[](size_t idx)const {
		if (idx >= siz)throw std::out_of_range("");
		return elements[idx];
	}
	void assign(size_t newsize, char_type val) {
		reserve(newsize);
		siz = newsize;
		for (size_t i = 0; i < siz; ++i)elements[i] = val;
	}
	void reserve(size_t newcapacity) {
		if (newcapacity <= cap)return;
		cap = newcapacity;
		char_type* temp = new char_type[newcapacity + 1];
		if (temp == nullptr)throw std::bad_alloc();
		for (size_t i = 0; i < siz; ++i)temp[i] = elements[i];
		delete[] elements;
		elements = temp;
	}
	void clear() { siz = 0; }
	_string operator+(const _string& s)const {
		_string ret(*this);
		ret.siz += s.siz;
		if (ret.siz >= ret.cap)ret.reserve(siz << 1);
		for (size_t i = 0; i < s.siz; ++i)ret[ret.siz - s.siz + i] = s[i];
		return ret;
	}
	_string operator+(const char_type* s)const {
		_string ret(*this);
		size_t p = 0;
		while (s[p] != '\0')++p, ++ret.siz;
		if (ret.siz >= ret.cap)ret.reserve(siz << 1);
		for (size_t i = 0; i < p; ++i)ret[ret.siz - p + i] = s[i];
		return ret;
	}
	_string operator+(char_type c)const {
		_string ret(*this);
		++ret.siz;
		if (ret.siz >= ret.cap)ret.reserve(siz << 1);
		ret.elements[ret.siz - 1] = c;
		return ret;
	}
	_string& operator=(const _string& s) {
		siz = s.siz;
		reserve(s.cap);
		for (size_t i = 0; i < siz; ++i)elements[i] = s[i];
		return *this;
	}
	_string& operator=(const char_type* s) {
		siz = 0;
		while (s[siz] != '\0')++siz;
		reserve(siz);
		for (size_t i = 0; i < siz; ++i)elements[i] = s[i];
		return *this;
	}
	_string& operator+=(const _string& s) {
		siz += s.siz;
		if (siz >= cap)reserve(siz << 1);
		for (size_t i = 0; i < s.siz; ++i)elements[siz - s.siz + i] = s[i];
		return *this;
	}
	_string& operator+=(const char_type* s) {
		size_t p = 0;
		while (s[p] != '\0')++p, ++siz;
		if (siz >= cap)reserve(siz << 1);
		for (size_t i = 0; i < p; ++i)elements[siz - p + i] = s[i];
		return *this;
	}
	_string& operator+=(char_type c) {
		if (++siz >= cap)reserve(cap << 1);
		elements[siz - 1] = c;
		return *this;
	}
	_string substr(size_t pos, size_t len)const {
		if (pos + len >= siz)throw std::out_of_range("");
		_string ret(len, '\0');
		for (size_t i = pos; i < pos + len; ++i)ret[i - pos] = elements[i];
		return ret;
	}
	bool operator<(const _string& other)const {
		size_t l1 = length(), l2 = other.length();
		for (size_t i = 0; i < l1 && i < l2; ++i) {
			if (elements[i] < other[i])return true;
			if (elements[i] > other[i])return false;
		}
		return l1 < l2;
	}
	bool operator==(const _string& other)const {
		size_t l1 = length(), l2 = other.length();
		if (l1 != l2)return false;
		for (size_t i = 0; i < l1 && i < l2; ++i) if (elements[i] != other[i])return false;
		return true;
	}
	bool operator!=(const _string& other)const {
		return !operator==(other);
	}
	const char_type* c_str()const {
		elements[siz] = '\0';
		return elements;
	}
};

using string = _string<char>;

std::istream& operator>>(std::istream& is, string& str) {
	str.clear();
	while (isspace(is.peek())) {
		if (is.peek() == EOF)return is;
		is.ignore(1);
	}
	while (!isspace(is.peek()))str += getchar();
	return is;
}
std::ostream& operator<<(std::ostream& os, const string& str) {
	os << str.c_str();
	return os;
}

template<typename TyF, typename TyS>
class pair {
public:
	TyF first;
	TyS second;
	pair() :first(TyF()), second(TyS()) {}
	pair(const TyF& first, const TyS& second) :first(first), second(second) {}
	bool operator<(const pair<TyF, TyS>& other)const {
		return first < other.first || (first == other.first && second < other.second);
	}
	bool operator==(const pair<TyF, TyS>& other)const {
		return first == other.first && second == other.second;
	}
	bool operator!=(const pair<TyF, TyS>& other)const {
		return !operator==(other);
	}
};

template<typename Ty>
class queue {
private:
	struct _node {
		_node* next;
		Ty val;
		_node() :next(nullptr), val(Ty()) {}
		_node(_node* next, Ty val) :next(next), val(val) {}
	};
	_node* prefront, * back;
	size_t siz;
public:
	queue() :prefront(new _node()), back(prefront), siz(0) {}
	void push(const Ty& val) {
		back->next = new _node(nullptr, val);
		if (back->next == nullptr)throw std::bad_alloc();
		back = back->next;
		++siz;
	}
	Ty& front() { return prefront->next->val; }
	void pop() {
		if (siz <= 0)throw std::out_of_range("");
		_node* temp = prefront->next;
		prefront->next = temp->next;
		if (temp == back)back = prefront;
		--siz;
		delete temp;
	}
	bool empty() { return prefront == back; }
};

template<typename Ty>
class stack {
private:
	vector<Ty> elem;
public:
	stack() :elem(0) {}
	void push(const Ty& val) { elem.push_back(val); }
	void pop() { elem.pop_back(); }
	Ty& top() { return elem.back(); }
};

template<typename Ptr>
class _tree_iterator_base {
public:
	Ptr ptr;
	_tree_iterator_base(Ptr ptr) :ptr(ptr) {}
	void operator++() {
		if (ptr->is_end)throw std::out_of_range("");
		if (ptr->rs != nullptr) {
			ptr = ptr->rs;
			while (ptr->ls != nullptr)ptr = ptr->ls;
			return;
		}
		while (ptr->fa != nullptr) {
			if (ptr->fa->ls == ptr) {
				ptr = ptr->fa;
				return;
			}
			else ptr = ptr->fa;
		}
	}
	void operator--() {
		if (ptr->ls != nullptr) {
			ptr = ptr->ls;
			while (ptr->rs != nullptr)ptr = ptr->rs;
			return;
		}
		while (ptr->fa != nullptr) {
			if (ptr->fa->rs == ptr) {
				ptr = ptr->fa;
				return;
			}
			else ptr = ptr->fa;
		}
		throw std::out_of_range("");
	}
	bool operator==(const _tree_iterator_base& other)const { return ptr == other.ptr; }
	bool operator!=(const _tree_iterator_base& other)const { return ptr != other.ptr; }
};

template<typename Ty, typename Cmp = std::less<Ty>>
class set {
private:
	struct _node {
		int balance = 0;
		size_t h = 1;
		bool is_end;
		_node* ls = nullptr, * rs = nullptr, * fa = nullptr;
		Ty key;
		_node(const Ty& key) :key(key), is_end(false) {}
		_node() :is_end(true), key(Ty()) {}
		~_node() {
			if (ls != nullptr)delete ls;
			if (ls != rs && rs != nullptr)delete rs;
		}
	};
	_node* pre_root;
	size_t siz = 0;
public:
	class iterator :public _tree_iterator_base<const _node*> {
	private:
		using _base = _tree_iterator_base<const _node*>;
	public:
		using _base::ptr;
		iterator(const _node* ptr) :_base(ptr) {}
		iterator& operator++() {
			_base::operator++();
			return *this;
		}
		iterator& operator--() {
			_base::operator--();
			return *this;
		}
		const Ty& operator*() {
			if (ptr->is_end)throw std::out_of_range("");
			return ptr->key;
		}
	};
	set() :pre_root(new _node()) {}
	~set() { delete pre_root; }
	pair<iterator, bool> insert(const Ty& key) {
		if (pre_root->ls == nullptr) {
			pre_root->ls = pre_root->rs = new _node(key);
			pre_root->ls->fa = pre_root;
			++siz;
			return pair<iterator, bool>(iterator(pre_root->ls), true);
		}
		return _insert(pre_root->ls, pre_root, key);
	}
	size_t erase(const Ty& key) {
		if (pre_root->ls == nullptr)return 0;
		return _erase(pre_root->ls, key);
	}
	iterator begin() { return iterator(_min(pre_root)); }
	iterator end() { return iterator(pre_root); }
	size_t size()const { return siz; }
	iterator lower_bound(const Ty& key) {
		_node* ptr = pre_root->ls;
		if (ptr == nullptr)return iterator(pre_root);
		while (1) {
			if (ptr->key == key)return iterator(ptr);
			else if (Cmp()(ptr->key, key)) {
				if (ptr->rs == nullptr)return ++iterator(ptr);
				else ptr = ptr->rs;
			}
			else {
				if (ptr->ls == nullptr)return iterator(ptr);
				else ptr = ptr->ls;
			}
		}
	}
	iterator upper_bound(const Ty& key) {
		_node* ptr = pre_root->ls;
		if (ptr == nullptr)return iterator(pre_root);
		while (1) {
			if (ptr->key == key)return ++iterator(ptr);
			else if (Cmp()(ptr->key, key)) {
				if (ptr->rs == nullptr)return ++iterator(ptr);
				else ptr = ptr->rs;
			}
			else {
				if (ptr->ls == nullptr)return iterator(ptr);
				else ptr = ptr->ls;
			}
		}
	}
	iterator find(const Ty& key) {
		iterator ret = lower_bound(key);
		if (ret == this->end() || *ret != key)return this->end();
		else return ret;
	}
	void clear() {
		if (pre_root->ls != nullptr)delete pre_root->ls;
		pre_root->ls = pre_root->rs = nullptr;
		siz = 0;
	}
private:
	void _update(_node* rt) {
		size_t lh = _geth(rt->ls), rh = _geth(rt->rs);
		rt->h = std::max(lh, rh) + 1;
		rt->balance = lh - rh;
	}
	size_t _geth(_node* rt) {
		return rt == nullptr ? 0 : rt->h;
	}
	_node* _min(_node* rt) {
		while (rt->ls != nullptr)rt = rt->ls;
		return rt;
	}
	_node* _max(_node* rt) {
		while (rt->rs != nullptr)rt = rt->rs;
		return rt;
	}
	pair<iterator, bool> _insert(_node*& rt, _node* fn, const Ty& key) {
		if (rt == nullptr) {
			rt = new _node(key);
			++siz;
			rt->fa = fn;
			auto temp = rt;
			_insert_maintain(rt);
			return pair<iterator, bool>(iterator(temp), true);
		}
		if (rt->key == key) {
			return pair<iterator, bool>(iterator(rt), false);
		}
		else if (Cmp()(rt->key, key))return _insert(rt->rs, rt, key);
		else return _insert(rt->ls, rt, key);
	}
	void _insert_maintain(_node* rt) {
		while (abs(rt->balance) < 2) {
			rt = rt->fa;
			if (rt->is_end)return;
			_update(rt);
		}
		_maintain(rt);
	}
	size_t _erase(_node* rt, const Ty& key) {
		if (rt == nullptr)return 0;
		if (rt->key == key) {
			if (rt->ls != nullptr) {
				_node* ptr = _max(rt->ls);
				_swap(rt, ptr);
				return _erase(rt, key);
			}
			else if (rt->rs != nullptr) {
				_node* ptr = _min(rt->rs);
				_swap(rt, ptr);
				return _erase(rt, key);
			}
			else {
				if (rt->fa->ls == rt)rt->fa->ls = nullptr;
				if (rt->fa->rs == rt)rt->fa->rs = nullptr;
				_erase_maintain(rt->fa);
				delete rt;
				--siz;
				return 1;
			}
		}
		else if (Cmp()(rt->key, key)) return _erase(rt->rs, key);
		else return _erase(rt->ls, key);
	}
	void _erase_maintain(_node* rt) {
		while (!rt->is_end) {
			_update(rt);
			_node* temp = rt->fa;
			if (abs(rt->balance) > 1)_maintain(rt);
			rt = temp;
		}
	}
	void _swap(_node* ptr1, _node* ptr2) {
		_node* fa1 = ptr1->fa, * ls1 = ptr1->ls, * rs1 = ptr1->rs;
		_node* fa2 = ptr2->fa, * ls2 = ptr2->ls, * rs2 = ptr2->rs;
		if (fa1->ls == ptr1)fa1->ls = ptr2;
		if (fa1->rs == ptr1)fa1->rs = ptr2;
		if (fa2->ls == ptr2)fa2->ls = ptr1;
		if (fa2->rs == ptr2)fa2->rs = ptr1;
		if (ls1 != nullptr)ls1->fa = ptr2;
		if (rs1 != nullptr)rs1->fa = ptr2;
		if (ls2 != nullptr)ls2->fa = ptr1;
		if (rs2 != nullptr)rs2->fa = ptr1;
		std::swap(ptr1->fa, ptr2->fa);
		std::swap(ptr1->ls, ptr2->ls);
		std::swap(ptr1->rs, ptr2->rs);
	}
	void _maintain(_node* rt) {
		if (rt->balance > 0) {
			if (rt->ls->balance >= 0)_Rrotate(rt);
			else _LRrotate(rt);
		}
		else {
			if (rt->rs->balance <= 0)_Lrotate(rt);
			else _RLrotate(rt);
		}
	}
	void _Rrotate(_node* rt) {
		_node* nrt = rt->ls;
		rt->ls = nrt->rs;
		if (rt->ls != nullptr)rt->ls->fa = rt;
		nrt->rs = rt;
		nrt->fa = rt->fa;
		if (rt->fa->ls == rt)rt->fa->ls = nrt;
		if (rt->fa->rs == rt)rt->fa->rs = nrt;
		rt->fa = nrt;
		_update(rt);
		_update(nrt);
	}
	void _Lrotate(_node* rt) {
		_node* nrt = rt->rs;
		rt->rs = nrt->ls;
		if (rt->rs != nullptr)rt->rs->fa = rt;
		nrt->ls = rt;
		nrt->fa = rt->fa;
		if (rt->fa->ls == rt)rt->fa->ls = nrt;
		if (rt->fa->rs == rt)rt->fa->rs = nrt;
		rt->fa = nrt;
		_update(rt);
		_update(nrt);
	}
	void _RLrotate(_node* rt) {
		_Rrotate(rt->rs);
		_Lrotate(rt);
	}
	void _LRrotate(_node* rt) {
		_Lrotate(rt->ls);
		_Rrotate(rt);
	}
};

template<typename KT, typename VT, typename Cmp = std::less<KT>>
class map {
private:
	struct _node {
		int balance = 0;
		size_t h = 1;
		bool is_end;
		_node* ls = nullptr, * rs = nullptr, * fa = nullptr;
		pair<const KT, VT> key_val;
		const KT& key = key_val.first;
		VT& val = key_val.second;
		_node(const KT& key, const VT& val) :key_val(key, val), is_end(false) {}
		_node() :is_end(true), key_val(KT(), VT()) {}
		~_node() {
			if (ls != nullptr)delete ls;
			if (ls != rs && rs != nullptr)delete rs;
		}
	};
	_node* pre_root;
	size_t siz = 0;
public:
	class iterator :public _tree_iterator_base<_node*> {
	private:
		using _base = _tree_iterator_base<_node*>;
	public:
		using _base::ptr;
		iterator(_node* ptr) :_base(ptr) {}
		iterator& operator++() {
			_base::operator++();
			return *this;
		}
		iterator& operator--() {
			_base::operator--();
			return *this;
		}
		pair<const KT, VT>& operator*() {
			if (ptr->is_end)throw std::out_of_range("");
			return ptr->key_val;
		}
	};
	map() :pre_root(new _node()) {}
	~map() { delete pre_root; }
	pair<iterator, bool> insert(const KT& key, const VT& val) {
		if (pre_root->ls == nullptr) {
			pre_root->ls = pre_root->rs = new _node(key, val);
			pre_root->ls->fa = pre_root;
			++siz;
			return pair<iterator, bool>(iterator(pre_root->ls), true);
		}
		return _insert(pre_root->ls, pre_root, key, val);
	}
	size_t erase(const KT& key) {
		if (pre_root->ls == nullptr)return 0;
		return _erase(pre_root->ls, key);
	}
	iterator begin() { return iterator(_min(pre_root)); }
	iterator end() { return iterator(pre_root); }
	size_t size()const { return siz; }
	VT& operator[](const KT& key) {
		iterator it = find(key);
		if (it == end()) {
			return (*insert(key, VT()).first).second;
		}
		else return (*it).second;
	}
	iterator lower_bound(const KT& key) {
		_node* ptr = pre_root->ls;
		if (ptr == nullptr)return iterator(pre_root);
		while (1) {
			if (ptr->key == key)return iterator(ptr);
			else if (Cmp()(ptr->key, key)) {
				if (ptr->rs == nullptr)return ++iterator(ptr);
				else ptr = ptr->rs;
			}
			else {
				if (ptr->ls == nullptr)return iterator(ptr);
				else ptr = ptr->ls;
			}
		}
	}
	iterator upper_bound(const KT& key) {
		_node* ptr = pre_root->ls;
		if (ptr == nullptr)return iterator(pre_root);
		while (1) {
			if (ptr->key == key)return ++iterator(ptr);
			else if (Cmp()(ptr->key, key)) {
				if (ptr->rs == nullptr)return ++iterator(ptr);
				else ptr = ptr->rs;
			}
			else {
				if (ptr->ls == nullptr)return iterator(ptr);
				else ptr = ptr->ls;
			}
		}
	}
	iterator find(const KT& key) {
		iterator ret = lower_bound(key);
		if (ret == this->end() || (*ret).first != key)return this->end();
		else return ret;
	}
	void clear() {
		if (pre_root->ls != nullptr)delete pre_root->ls;
		pre_root->ls = pre_root->rs = nullptr;
		siz = 0;
	}
private:
	void _update(_node* rt) {
		size_t lh = _geth(rt->ls), rh = _geth(rt->rs);
		rt->h = std::max(lh, rh) + 1;
		rt->balance = lh - rh;
	}
	size_t _geth(_node* rt) {
		return rt == nullptr ? 0 : rt->h;
	}
	_node* _min(_node* rt) {
		while (rt->ls != nullptr)rt = rt->ls;
		return rt;
	}
	_node* _max(_node* rt) {
		while (rt->rs != nullptr)rt = rt->rs;
		return rt;
	}
	pair<iterator, bool> _insert(_node*& rt, _node* fn, const KT& key, const VT& val) {
		if (rt == nullptr) {
			rt = new _node(key, val);
			++siz;
			rt->fa = fn;
			auto temp = rt;
			_insert_maintain(rt);
			return pair<iterator, bool>(iterator(temp), true);
		}
		if (rt->key == key) {
			return pair<iterator, bool>(iterator(rt), false);
		}
		else if (Cmp()(rt->key, key))return _insert(rt->rs, rt, key, val);
		else return _insert(rt->ls, rt, key, val);
	}
	void _insert_maintain(_node* rt) {
		while (abs(rt->balance) < 2) {
			rt = rt->fa;
			if (rt->is_end)return;
			_update(rt);
		}
		_maintain(rt);
	}
	size_t _erase(_node* rt, const KT& key) {
		if (rt == nullptr)return 0;
		if (rt->key == key) {
			if (rt->ls != nullptr) {
				_node* ptr = _max(rt->ls);
				_swap(rt, ptr);
				return _erase(rt, key);
			}
			else if (rt->rs != nullptr) {
				_node* ptr = _min(rt->rs);
				_swap(rt, ptr);
				return _erase(rt, key);
			}
			else {
				if (rt->fa->ls == rt)rt->fa->ls = nullptr;
				if (rt->fa->rs == rt)rt->fa->rs = nullptr;
				_erase_maintain(rt->fa);
				delete rt;
				--siz;
				return 1;
			}
		}
		else if (Cmp()(rt->key, key)) return _erase(rt->rs, key);
		else return _erase(rt->ls, key);
	}
	void _erase_maintain(_node* rt) {
		while (!rt->is_end) {
			_update(rt);
			_node* temp = rt->fa;
			if (abs(rt->balance) > 1)_maintain(rt);
			rt = temp;
		}
	}
	void _swap(_node* ptr1, _node* ptr2) {
		_node* fa1 = ptr1->fa, * ls1 = ptr1->ls, * rs1 = ptr1->rs;
		_node* fa2 = ptr2->fa, * ls2 = ptr2->ls, * rs2 = ptr2->rs;
		if (fa1->ls == ptr1)fa1->ls = ptr2;
		if (fa1->rs == ptr1)fa1->rs = ptr2;
		if (fa2->ls == ptr2)fa2->ls = ptr1;
		if (fa2->rs == ptr2)fa2->rs = ptr1;
		if (ls1 != nullptr)ls1->fa = ptr2;
		if (rs1 != nullptr)rs1->fa = ptr2;
		if (ls2 != nullptr)ls2->fa = ptr1;
		if (rs2 != nullptr)rs2->fa = ptr1;
		std::swap(ptr1->fa, ptr2->fa);
		std::swap(ptr1->ls, ptr2->ls);
		std::swap(ptr1->rs, ptr2->rs);
	}
	void _maintain(_node* rt) {
		if (rt->balance > 0) {
			if (rt->ls->balance >= 0)_Rrotate(rt);
			else _LRrotate(rt);
		}
		else {
			if (rt->rs->balance <= 0)_Lrotate(rt);
			else _RLrotate(rt);
		}
	}
	void _Rrotate(_node* rt) {
		_node* nrt = rt->ls;
		rt->ls = nrt->rs;
		if (rt->ls != nullptr)rt->ls->fa = rt;
		nrt->rs = rt;
		nrt->fa = rt->fa;
		if (rt->fa->ls == rt)rt->fa->ls = nrt;
		if (rt->fa->rs == rt)rt->fa->rs = nrt;
		rt->fa = nrt;
		_update(rt);
		_update(nrt);
	}
	void _Lrotate(_node* rt) {
		_node* nrt = rt->rs;
		rt->rs = nrt->ls;
		if (rt->rs != nullptr)rt->rs->fa = rt;
		nrt->ls = rt;
		nrt->fa = rt->fa;
		if (rt->fa->ls == rt)rt->fa->ls = nrt;
		if (rt->fa->rs == rt)rt->fa->rs = nrt;
		rt->fa = nrt;
		_update(rt);
		_update(nrt);
	}
	void _RLrotate(_node* rt) {
		_Rrotate(rt->rs);
		_Lrotate(rt);
	}
	void _LRrotate(_node* rt) {
		_Lrotate(rt->ls);
		_Rrotate(rt);
	}
};

STLIMI_END