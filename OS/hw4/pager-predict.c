/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#include "simulator.h"



struct page_info{
    int page;
    int *timestamp;
};

typedef struct page_info pageInfo;


int getlru(Pentry q[MAXPROCESSES], int timestamps[MAXPROCESSES][MAXPROCPAGES], int proc, int *lrutmp){
    //local
    int page;
    int pagetmp;
    int lrutime= INT_MAX;

    for(page=0; page<MAXPROCPAGES; page++){
        //could do subtraction, but comparing a larger number is quicker
        if(q[proc].pages[page] && timestamps[proc][page] < lrutime){

            *lrutmp = page;
            lrutime = timestamps[proc][page];
            pagetmp = 0;

        }
    }

    return pagetmp;
}


  
void init_info(pageInfo pageChain[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES]){
    int i, j, k;
    for(i=0; i<MAXPROCESSES; i++){
        for(j=0; j<MAXPROCPAGES; j++){
            for(k=0; k<MAXPROCPAGES; k++){

                pageChain[i][j][k].page = -1;
                pageChain[i][j][k].timestamp = NULL;

            }
        }
    }
}

//insert page number, and time into struct
void insert_info(int cur_page, int proc, int prevpage,
        pageInfo pageChain[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES],
        int timestamps[MAXPROCESSES][MAXPROCPAGES]){


    int pagetmp;
    pageInfo *newPageInfo;
    newPageInfo = pageChain[proc][prevpage];

    //tracks processes, previous pages -> current pages
    for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++){

        if(newPageInfo[pagetmp].page == cur_page){

            break;

        }

        if(newPageInfo[pagetmp].page == -1){

            newPageInfo[pagetmp].timestamp = &(timestamps[proc][pagetmp]);
            newPageInfo[pagetmp].page = cur_page;
            break;

        }
    }
}


//counting non empty pages
int nPage(pageInfo *pages){
    int n=0;
    while(pages[n].page != -1 && n < MAXPROCPAGES)
        n++;
    return n;
}

void swap( pageInfo* x, pageInfo* y){
    pageInfo tmp=*y;
    *y=*x;
    *x=tmp;
}

//puts most recently used first
void sortlru(pageInfo *pages){
    int len = nPage(pages);
    int i;
    int swapped = 0;
    while(swapped){
        swapped=0;
        for(i = 1;i < len;i++){
            assert(pages[i-1].timestamp != NULL &&
                   pages[i].timestamp != NULL);
            if(*(pages[i-1].timestamp) < *(pages[i].timestamp)){
                swap(pages+(i-1), pages+i);
                swapped = 1;
            }
        }
    }
}


void pageit(Pentry q[MAXPROCESSES]) { 

    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];
    static int activeProcs[MAXPROCESSES];
    static int curpc[MAXPROCESSES];

    //[proc#][prevpage#][seqpage#]
    static pageInfo simInfo[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES];

    /* Local vars */
    int proctmp;
    int pagetmp;
    int lrutmp;
    int prevpage;
    int cur_page;

    /* initialize static vars on first run */
    if(!initialized){
        init_info(simInfo);
        for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++){
            for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++){
                timestamps[proctmp][pagetmp] = 0;
            }
            activeProcs[proctmp] = 0;
        }
        initialized = 1;
    }

    for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++){
        //inactive or unset processes are skipped
        if(!q[proctmp].active || prevpage == -1)
            continue;
        //otherwise current and previous pages are retrieved
        //prev is swapped out  if different than current
        //page info is put into the page info struct
        prevpage = (curpc[proctmp])/PAGESIZE;
        curpc[proctmp] = q[proctmp].pc;
        cur_page = (q[proctmp].pc)/PAGESIZE;
        if(cur_page == prevpage)
            continue;

        pageout(proctmp, prevpage);
        insert_info(cur_page, proctmp, prevpage, simInfo, timestamps);
    }

    //time update
    for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++){
        if(!q[proctmp].active)
            continue;
        pagetmp = (q[proctmp].pc-1)/PAGESIZE;
        timestamps[proctmp][pagetmp] = tick;
    }

    for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++){
        //get non-active process pages out
        if (!q[proctmp].active){
            for(pagetmp=0; pagetmp<MAXPROCPAGES; pagetmp++){
                pageout(proctmp,pagetmp);
            }
            continue;
        }
        pagetmp = (q[proctmp].pc)/PAGESIZE;

        if(q[proctmp].pages[pagetmp] == 1)
            continue;

        if(pagein(proctmp,pagetmp)){
            //deactivate process, reset loop
            activeProcs[proctmp]=0;
            continue;
        }
        if(activeProcs[proctmp])
            continue;

        //all frames taken, get lru and swap it out
        if(getlru( q, timestamps, proctmp, &lrutmp)){
            continue;
        }

        //process becomes active
        activeProcs[proctmp]=1;
    }

    for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++){
        pageInfo *chain;
        int i;
        if(!q[proctmp].active){
            continue;
        }
        // get optimal lru pages
        //get current page
        chain = simInfo[proctmp][(q[proctmp].pc+101)/PAGESIZE];
        sortlru(chain);
        //print_guess(chain);
        //page in the most recently used
        for(i = 0; i < nPage(chain); i++){
            pagein(proctmp, chain[i].page);
        }
    }

    

    tick++;
}
