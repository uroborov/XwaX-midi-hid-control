/***** config.h *****/

/*
   Configure your midi note assignments here.

example :

up = 34; will assign note 34 to the up cursor key

*/

#ifndef _CONFIG_H_
#define _CONFIG_H_


        /*cursors*/
	int up = 65;    // seek up
        int down = 63;  // seek down
	int page_up = 75; //page up
	int page_down = 73; //page down
        int left = 85;  // folder up
        int right = 83; // folder down
        int rescan = 30; // trigger rescan

	/* deck 1 */
        int f1 = 22;   // load to deck1
        int f2 = 36;   // set time 
        int f3 = 29;   // toggle vinyl control 
        /* additional controls */
        //int control = 38;
        //int toggle = 69;
        //int tab = 20;
    
#endif
