#pragma execution_character_set("utf-8")
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

		//??
		//~Node()
		//{
		//	delete node_next;
		//	delete node_prev;
		//}

		void swap(Node& rhs) noexcept
		{
			std::swap(date, rhs.date);
			std::swap(node_next, rhs.node_next);
			std::swap(node_prev, rhs.node_prev);
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

		bool operator==(const IteratorList& rhs) const
		{
			return value == rhs.value;
		}
		bool operator!=(const IteratorList& rhs) const
		{
			return !(*this == rhs);
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
	const_iterator cbegin() const
	{
		return const_iterator(head_);
	}
	const_iterator cend() const
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
	const_iterator rbegin() const
	{
		return const_iterator(tell_);
	}
	const_iterator rend() const
	{
		return nullptr;
	}

	//неправильно
	//не бросает исключения т.к. указатели и базовые типы не бросают исключения
	void swap(list rhl) noexcept 
	{
		std::swap(head_, rhl.head_);
		std::swap(tell_, rhl.tell_);
		std::swap(size_, rhl.size_);
	}

	list() = default;

	list(std::initializer_list<T> values)
	{
		try {
			for (auto i = values.begin(); i != values.end(); ++i)
			{
				push_back(*i);
			}
		}
		catch (...)
		{
			//??
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
		//проверяем что не копируем сами себя
		if (&copy_list == this)
			return;
		//у нас еще нет текущей 
		//должен ли он выбрасывать исключения?

		for (auto i = copy_list.cbegin(); i != copy_list.cend(); ++i)
		{
			push_back(*i);
		}

		//list<T> temp;// чтоб не портить наш объект создаем копию в конце копия с данными которые были у нас самоуничтожится
		////вставляем элементы в копию
		//for (auto i = copy_list.begin(); i != copy_list.end(); ++i)
		//{
		//	temp.push_back(*i);
		//}
		//
		////цикл чтоб поменять каждый узел местами
		//for (auto i = temp.begin(); i != temp.end(); ++i)
		//{
		//}
		//неправильно
		//swap(temp);
	}
	//безопасен
	list& operator=(list& rhs)
	{
		if (&rhs == this)
			return *this;
		list tmp(rhs);//может бросить исключение но не меняет состояние класса. В конце метода самоуничтожется с помощью диструктора
		
		//неправильно?
		swap(tmp);
		return *this;
	}

	list(list&& movy_list)
	{
		//??
	}
	list& operator=(const list&& rhs)
	{
		//??
	}
	
	bool empty() const
	{
		return size_==0;
	}
	size_t size() const
	{
		return size_;
	}

	void push_front(T& value)
	{
		Node* newNode = new Node(value, head_, nullptr);
		try
		{
			//может выбросить исключение
			//newNode = new Node(value, head_, nullptr);//создали новую ноду указывающую на следующую, предыдущего нет
			//--------------------------------------------------
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
		catch (...)
		{
			//освободить выделенную память
			delete newNode;
			throw;
		}
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
		try
		{
			//Node* newNode = new Node(value, nullptr, tell_);
			//----------------------------
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
		catch (...)
		{
			delete newNode;
			throw;
		}
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
	//void insert(iterator& pos, T& value)
	//{
	//	if (pos == begin())
	//	{
	//		push_front(value);
	//		//return iterator(head_);
	//	}
	//	for (auto i = begin(); i != end(); ++i)
	//	{
	//		if (i == pos)
	//		{
	//			Node* newNode = new Node(value, nullptr, nullptr);
	//			newNode->node_next = i;
	//			newNode->node_prev = i->node_prev;
	//			//return iterator(newNode);
	//			return;
	//		}
	//	}
	//	push_back(value);
	//	//return iterator(tell_);
	//	
	//}

	iterator insert(iterator& pos, T& value)
	{
		try {//т.к. может прилететь исключение из методов вставки
			if (pos == begin())
			{
				push_front(value);
				return begin();
			}
		}
		catch (...)
		{
			pop_front();//удаляем вставленный элемент чтобы вернуть все к тому что было
			throw;
		}
		try{
			if (pos == rbegin())
			{
				push_back(value);
				return rbegin();
			}
		} 
		catch (...)
		{
			pop_back();
			throw;
		}
		try {
			//проходимся по всему созданному листу и ищем позицию для вставки
			auto i = begin();
			for (; i != end(); ++i)
			{
				if (i == pos)
				{
					Node* newNode = new Node(value, nullptr, nullptr);

					newNode->node_prev = i->node_prev;
					newNode->node_next = i;
					//поменять указатель на предыдущий у i
					//поменять указатель на следующий у --i
					newNode->node_prev->node_next = newNode;
					i->node_prev = newNode;
					break;
				}
			}
		}
		catch (...)
		{

		}

		return pos;
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

	for (auto i = lt.begin(); i != lt.end(); ++i)
	{
		std::cout << *i;
	}
	std::cout << "\n";

	//list<int> tt(lt);

	//for (auto i = tt.begin(); i != tt.end(); ++i)
	//{
	//	std::cout << *i;
	//}
	
	list<int> ut = { 7,6,5,4,3,2,1 };
	std::cout << ut.empty() << " " << ut.size() << std::endl;

	for (auto i = ut.begin(); i != ut.end(); ++i)
	{
		std::cout << *i;
	}

	lt = ut;

	std::cout << "lt\n";
	std::cout << lt.empty() << " " << lt.size() << std::endl;

	for (auto i = lt.begin(); i != lt.end(); ++i)
	{
		std::cout << *i;
	}

	std::cout << "H";
}