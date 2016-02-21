// DebugTest.cpp : Defines the entry point for the console application.
//

#include "example.h"


int main()
{
	ContainerPtr p1(std::make_unique<Container>());
	Obj a;

	p1->m_objs.push_back(a);
	printf("BEFORE: %s\n", p1->m_objs[0].data);
	ContainerPtr p2 = std::move(p1);
	Obj b = p2->m_objs[0];
	b.m_subobj.DoSomething();
	//printf("AFTER: %s\n", );
	
	char c;
	scanf(&c);
    return 0;
}

