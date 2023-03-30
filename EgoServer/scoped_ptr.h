#pragma once
template<typename T>
class scoped_ptr
{
public:
	scoped_ptr(T* ptr);
	scoped_ptr<T>& operator=(T* lhs);
	T* operator->();
	bool operator==(T* lhs);
	~scoped_ptr();
	T* operator *();
private:
	T* p;

};
template<typename T>
scoped_ptr<T>::scoped_ptr(T* ptr) :p(ptr)
{

}
template<typename T>
scoped_ptr<T>& scoped_ptr<T>::operator=(T* lhs)
{
	if (p != nullptr)
	{
		delete p;
		p = nullptr;
	}
	p = lhs;
	return *this;
}
template<typename T>
T* scoped_ptr<T>::operator->()
{
	return p;
}
template<typename T>
scoped_ptr<T>::~scoped_ptr()
{
	if (p != nullptr)
	{
		delete p;
		p = nullptr;
	}
}
template<typename T>
bool scoped_ptr<T>::operator==(T* lhs)
{
	return p == lhs;
}