#include <iostream>

#include "CImg.h"
#include "../transformaciones.h"

#include "item1.h"
#include "item2.h"
#include "item3.h"
#include "item4.h"

using namespace std;

int main(int argc, char** argv) {
	// Item 1
	item1(argc, argv);
	
	// Item 2
	item2(argc, argv);
	
	// Item 3
	item3(argc, argv);
	
	// Item 4
	item4(argc, argv);
	
	cout<<"*********************************************************"<<endl<<endl;
	
	return 0;
}
