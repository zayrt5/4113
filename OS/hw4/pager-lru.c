/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) { 
    
    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */
    int proctmp;
    int pagetmp;
    int proc;
    int page;
    int pc;
    int lrutmp;
    int lru;

    /* initialize static vars on first run */
    if(!initialized){
	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
		timestamps[proctmp][pagetmp] = 0; 
	    }
	}
	initialized = 1;
    }
    
    for(proc=0; proc<MAXPROCESSES; proc++) {
        // Determine if the process is active
        if(q[proc].active) {

            pc = q[proc].pc;                
            page = pc/PAGESIZE;             

            timestamps[proc][page] = tick;
            //if the current processes current page is not allocated
            if(!q[proc].pages[page]) {
                //printf("Paging in active process %i \n", proc);
                // try to swap in, do the work if it fails
                if(!pagein(proc,page)) {
                    //printf("Paging in active process %i \n", proc);

                    lrutmp = -1; //set so any tick time will be greater
                    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
                        // if current page is lru and is currently swapped in
                        if((tick - timestamps[proc][pagetmp]) > lrutmp && q[proc].pages[pagetmp]) {

                            lru = pagetmp;
                            lrutmp = (tick - timestamps[proc][pagetmp]);
                        }
                    }
                    //printf("Evicting page %i from process %i \n ", lru, proc);
                    pageout(proc,lru);
                }
            }
        }
        //inactive processes and their pages are evicted
        
        else{
            //printf("Evicting inactive process %i \n", proc);
            for(pagetmp=0; pagetmp<MAXPROCPAGES; pagetmp++){
                    //if(q[proc].pages[pagetmp]){
                        pageout(proc,pagetmp);
                    //}
                }
        }
        
    }
    tick++;
} 
