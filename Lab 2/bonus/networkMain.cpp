#include <iostream>

#include "message.h"
#include "host.h"
#include "router.h"

using namespace std;


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void print_table (Router *x)
{

  std::cout << "Table for router: " << x->get_address() << std::endl;
  
  for(std::pair<int, Hop> xx : x->route_table)
    {    
      std::cout << xx.first << " " << xx.second.next_hop << ' ' << xx.second.hops_away << "\n";
    }

  std::cout << std::endl <<std::endl;

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


int main()
{

  	Host h1(1);
	Router r1(1);

	Host h2(2);
	Router r2(2);

	Host h3(3);
	Router r3(3);

	Host h4(4);
	Router r4(4);

	Host h5(5);
	Router r5(5);

	Host h6(6);
	Router r6(6);

	Host h7(7);
	Router r7(7);

	Host h8(8);
	Router r8(8);

	Host h9(9);
	Router r9(9);

	Host h10(10);
	Router r10(10);

	Host h11(11);
	Router r11(11);

	Host h12(12);
	Router r12(12);

	r1.connectTo(h1);
	r2.connectTo(h2);
	r3.connectTo(h3);
	r4.connectTo(h4);
	r5.connectTo(h5);
	r6.connectTo(h6);
	r7.connectTo(h7);
	r8.connectTo(h8);
	r9.connectTo(h9);
	r9.connectTo(h10);
	r9.connectTo(h11);
	r10.connectTo(h12);

        r1.connectTo(r2);
        r2.connectTo(r3);
        r3.connectTo(r4);
        r4.connectTo(r1);
	r2.disconnectFrom(r1);
	r2.connectTo(r1);

	r4.connectTo(r5);
	r6.connectTo(r5);
	r6.connectTo(r3);
	r6.connectTo(r7);
	r8.connectTo(r7);
	r5.connectTo(r8);
	r9.connectTo(r7);
	r10.connectTo(r6);
	r11.connectTo(r10);

	r10.disconnect(r6);
	r6.disconnectFrom(r3);
	r5.disconnect(r8);

	r9.connectTo(r10);

	r11.connectTo(r2);
	r3.disconnectFrom(r2);
	h12.disconnect(r10);

	h1.send(12, "hi");
	h12.send(4, "hi");
	h6.send(9, "hi");
	h9.disconnect(r9);
	r9.disconnect(h10);
	r9.disconnect(h11);
	r12.connectTo(r11);
	h12.connectTo(r12);

       	print_table(&r1); print_table(&r2);
	print_table(&r3); print_table(&r4);
	print_table(&r5); print_table(&r6);
	print_table(&r7); print_table(&r8);
	print_table(&r9); print_table(&r10);
	print_table(&r11); print_table(&r12);































  /*
	Host h1(1);
	Router r1(1);
    
	r1.connectTo(h1);
    
	Host h2(2);
	Router r2(2);
    
	r1.connectTo(r2);
    
	r2.connectTo(h2);
    
	h1.send(2, "This is a test");
   
	Router r3(3);
	Host h3(3);
	r3.connectTo(h3);
    
	Router r4(4);
	Host h4(4);
	r4.connectTo(h4);
    
	Router r5(5);
    
	r3.connectTo(r4);
	r3.connectTo(r5);
    
	Host h5(5);

	r5.connectTo(h5);
    
	h4.send(5, "This is another test");
    
 	r5.connectTo(r4);
    
 	h4.send(5, "Hello world");
    
 	r2.connectTo(r3);
    
 	h1.send(3, "What a wonderful world");
    
 	r4.disconnect(h4);
  
 	h2.send(4, "Is anybody out there?");
    
 	r3.disconnectFrom(r5);
    
 	h3.send(5, "How many links must a packet walk down?");

	return 0;
  
	*/
}
