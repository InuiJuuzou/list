#include <iostream>

template<typename T>
class list
{
private:
	struct Node
	{
		T date;

		Node* node_next = nullptr;
		Node* node_prev = nullptr;
		
		Node() = default;

		Node(T value, Node* next, Node* prev)
		{
			date = value;
			node_next = next;
			node_prev = prev;
		}

	};
	//двунаправленные итераторы (bidirectional iterators)
	template<typename Iter>
	struct IteratorList
	{
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef Iter value_type;
		typedef T& reference;
		typedef Iter* pointer;
		typedef ptrdiff_t difference_type;//разница между указателями

		Iter* value;

		IteratorList(const IteratorList& it):value(it.value){}//copy

		bool operator==(const IteratorList& rhs)
		{
			return value == rhs.value;
		}
		bool operator!=(const IteratorList& rhs)
		{
			return value != rhs.value;
		}
		IteratorList& operator++()
		{
			value = value->node_next;
			return *this;
		}
		IteratorList operator++(int)
		{
			Iter* temp = value;
			operator++();
			return temp;

		}
		IteratorList& operator--()
		{
			value=value->node_prev;
			return *this;
		}
		IteratorList operator--(int)
		{
			Iter* temp = value;
			operator--();
			return temp;
		}
		//??
		/* https://ru.stackoverflow.com/questions/239051/%D0%97%D0%B0%D1%87%D0%B5%D0%BC-%D0%BD%D0%B0%D0%B4%D0%BE-%D0%BF%D0%B8%D1%81%D0%B0%D1%82%D1%8C-typename-%D0%B4%D0%BB%D1%8F-%D1%83%D1%82%D0%BE%D1%87%D0%BD%D0%B5%D0%BD%D0%B8%D1%8F-%D1%82%D0%B8%D0%BF%D0%B0 */
		const typename IteratorList::reference operator*() const
		{
			return value->date;
		}
		typename IteratorList::reference operator*()
		{
			return value->date;
		}
		typename IteratorList::pointer operator->()
		{
			return value;
		}
		const typename IteratorList::pointer operator->() const
		{
			return value;
		}

		IteratorList& operator=(IteratorList rhs)
		{
			Iter tmp(rhs);
			std::swap(value, tmp->value);
			return *this;
		}

		friend class list;
		
	private:
		IteratorList(Iter* p):value(p){}
	};

	Node* head_=nullptr;
	Node* tell_=nullptr;
	size_t size_ = 0;
public:
	//??
	typedef IteratorList<Node> iterator;
	typedef IteratorList<const Node> const_iterator;

	iterator begin()
	{
		return iterator(head_);
	}
	iterator end()
	{
		return nullptr;
	}
	const_iterator cbegin()const
	{
		return const_iterator(head_);
	}
	const_iterator cend()const
	{
		return nullptr;
	}

	iterator rbegin()
	{
		return iterator(tell_);
	}
	iterator rend()
	{
		return nullptr;
	}

	void swap(list rhl)
	{
		std::swap(head_, rhl.head_);
		std::swap(tell_, rhl.tell_);
		std::swap(size_, rhl.size_);
	}


	list() = default;

	list(std::initializer_list<T> values)
	{
		for (auto i = values.begin(); i != values.end(); ++i)
		{
			push_back(*i);
		}
	}
	~list()
	{
		while (!empty())
		{
			pop_back();
		}
		head_ = nullptr;
	}
	//copy
	//list(const list& copy_list)
	list(list& copy_list)
	{
		if (copy_list == *this)
			return;
		for (auto i = copy_list.begin(); i != copy_list.end(); ++i)
		{
			push_back(*i);
		}
	}
	list& operator=(const list& rhs)
	{
		if (rhs == *this)
			return *this;
		list tmp(rhs);
		swap(tmp);
		return *this;
	}

	list(list&& movy_list)
	{

	}
	list& operator=(const list&& rhs)
	{

	}
	
	/*list()
	{
		tell_ = new Node(? , nullptr, nullptr);
	}*/

	

	bool empty() const
	{
		return size_==0;
	}
	size_t size() const
	{
		return size_;
	}

	void push_front(T value)
	{
		//может выбросить исключение
		Node* newNode = new Node(value, head_,nullptr);//создали новую ноду указывающую на следующую, предыдущего нет
		if (head_ == nullptr)
		{
			head_ = newNode;
			tell_ = newNode;
			++size_;
			return;
		}
		head_->node_prev = newNode;
		head_ = newNode;//переместили головной указатель на новую ноду
		++size_;
	}

	void pop_front()
	{
		//создание временного узла чтоб не потерять его
		Node* temp = head_->node_next;
		delete head_;
		head_ = temp;
		--size_;
	}

	void push_back(T value)
	{
		Node* newNode = new Node(value, nullptr, tell_);
		if (tell_ == nullptr)
		{
			head_ = newNode;
			tell_ = newNode;
			++size_;
			return;
		}

		tell_->node_next = newNode;
		tell_ = newNode;
		++size_;
	}

	void pop_back()
	{
		if (tell_ != nullptr) {
			Node* temp = tell_->node_prev;
			delete tell_;
			tell_ = temp;
			--size_;
		}
	}
	//вставка до позиции
	void insert(iterator pos, T value)
	{
		if (pos == begin())
		{
			push_front(value);
			//return iterator(head_);
		}

		for (auto i = begin(); i != end(); ++i)
		{
			if (i == pos)
			{
				Node* newNode = new Node(value, nullptr, nullptr);
				newNode->node_next = i;
				newNode->node_prev = i->node_prev;
				//return iterator(newNode);
				return;
			}
		}

		push_back(value);
		//return iterator(tell_);
		
	}

	T& front() const
	{
		return head_->date;
	}
	T& back() const
	{
		return tell_->date;
	}

	bool operator==(const list& rhs) const
	{
		if (head_ == rhs.head_ && tell_ == rhs.tell_)
			return true;
		return false;
	}

	bool operator!=(const list& rhs) const
	{
		return !(*this == rhs);
	}
	//friend std::ostream& operator<<(std::ostream& os, const iterator& obj);
};

//std::ostream& operator<<(std::ostream& os, const iterator& obj)
//{
//	os << obj->date << " ";
//	return os;
//}

int main()
{
	list<int> lt = { 1,2,3,5,7 };

	std::cout << lt.empty() << " " << lt.size() << std::endl;
	/*lt.push_front(1);
	lt.push_front(2);
	lt.push_front(3);*/
	/*lt.push_back(1);
	lt.push_back(2);
	lt.push_back(3);
	std::cout << lt.empty() << " " << lt.size() << std::endl;

	lt.front() = 7;

	std::cout << lt.front();*/
	//lt.insert(lt.begin(), 99);
	std::cout << lt.empty() << " " << lt.size() << std::endl;
	for (auto i = lt.begin(); i != lt.end(); ++i)
	{
		std::cout << *i;
	}
	std::cout << "\n";
	list<int> tt(lt);

	for (auto i = tt.begin(); i != tt.end(); ++i)
	{
		std::cout << *i;
	}
	

	std::cout << "H";
}