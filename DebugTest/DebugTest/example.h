#pragma once

#include <memory>
#include <vector>



class Obj;

class SubObj
{
public:
	SubObj(Obj& parent)
		:m_parent(parent)
	{

	};
	char* someData = "hello";
	void DoSomething()
	{
		printf("someData");
	};

	Obj& m_parent;
};

class Obj
{
public:
	Obj()
		:m_subobj(*this)
	{

	};
	char* data = "test";

	SubObj m_subobj;
};


struct Container
{
	std::vector<Obj> m_objs;
};

typedef std::unique_ptr<Container> ContainerPtr;