#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<poll.h>
#include<sys/socket.h>
#include"message.h"
#include"logging.h"
#include<sys/wait.h> 
#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)
typedef struct bidder{
    char* bpath;
    int n_of_arg;
    char** args;
} bidder;
typedef struct pollfd Pollfd;
int min(int d[],int size){
    int i;
    int min=d[0];
    for(i=1;i<size;i++){
        if(d[i]>d[i-1]){
            min=d[i];
        }
    }
    return min;
}


int main(){
    int starting_bid;
    int minimum_increment;
    int number_of_bidders;
    int i,j;
    int active;
    int** bidFds;
    pid_t* pids;
    Pollfd* pfds;
    // Starting bid,min increment and number of bidders are taken. 
    // Path, number of arguments and the arguments are taken below.
    scanf("%d %d %d",&starting_bid,&minimum_increment,&number_of_bidders);
    bidder* bidders=(bidder*)malloc(number_of_bidders*sizeof(bidder));
    for(i=0;i<number_of_bidders;i++){
        bidders[i].bpath = (char*)(malloc(sizeof(char)*200));
        scanf("%s",bidders[i].bpath);
        scanf("%d",&bidders[i].n_of_arg);
        //bidders[i].args = calloc((bidders[i].n_of_arg+2),sizeof(char*));
        bidders[i].args = (char**)malloc((bidders[i].n_of_arg+2)*sizeof(char*));
        //bidders[i].args[0]=(char*)malloc(200);
        bidders[i].args[0]=bidders[i].bpath;
        for(j=0;j<bidders[i].n_of_arg;j++){
            bidders[i].args[j+1]=(char*)malloc(200);
            scanf("%s",bidders[i].args[j+1]);
        }   
        //bidders[i].args[j+1]=(char*)malloc(200);
        bidders[i].args[j+1]=NULL;
       

       
    }
    //File descriptors for each bidder is allocated
    bidFds=malloc(sizeof(int*)*number_of_bidders);
    pids=malloc(sizeof(pid_t)*number_of_bidders);
    pfds=calloc(number_of_bidders,sizeof(Pollfd));
    //pfds=malloc(number_of_bidders*sizeof(Pollfd));
    for(i=0;i<number_of_bidders;i++){
        bidFds[i]=calloc(2,sizeof(int));
    }
    int temp;
    //Pipe for each bidder
    for(i=0;i<number_of_bidders;i++){
        PIPE(bidFds[i]);
        temp=bidFds[i][0];
        pfds[i] = (Pollfd) {temp,POLLIN,0};
        if((pids[i]=fork())!=0){ //parent
            close(bidFds[i][1]);
        }
        else{  //child
            close(bidFds[i][0]);
            dup2(bidFds[i][1],0);
            dup2(bidFds[i][1],1);
            close(bidFds[i][1]);
            execvp(bidders[i].bpath,bidders[i].args);
            
        }
    }
    active=number_of_bidders;
    int* delays = calloc(number_of_bidders,sizeof(int));
    int* status = calloc(number_of_bidders,sizeof(int));
    int current_bid=0;
    int winner_id;
    while(active){
        poll(pfds,number_of_bidders,min(delays,number_of_bidders));
        for(i=0;i<number_of_bidders;i++){
            if(pfds[i].revents && POLLIN){
                cm bmessage;
                if(read(bidFds[i][0],&bmessage,sizeof(cm))){
                    if(bmessage.message_id==CLIENT_CONNECT){
                        delays[i]=bmessage.params.delay;
                        ii val={bmessage.message_id,pids[i],bmessage.params};
                        print_input(&val,i);

                        sm response;
                        response.message_id=SERVER_CONNECTION_ESTABLISHED;
                        response.params.start_info.client_id=i;
                        response.params.start_info.current_bid=current_bid;
                        response.params.start_info.minimum_increment=minimum_increment;
                        response.params.start_info.starting_bid=starting_bid;
                        write(bidFds[i][0],&response,sizeof(sm));
                        oi data={response.message_id,pids[i],response.params};
                        print_output(&data,i);
                    }

                    else if(bmessage.message_id==CLIENT_BID){
                        ii val2 = {bmessage.message_id,pids[i],bmessage.params};
                        print_input(&val2,i);
                        sm response2;
                        response2.message_id=SERVER_BID_RESULT;
                        oi data2;
                        if(bmessage.params.bid<starting_bid){
                            response2.params.result_info.result=BID_LOWER_THAN_STARTING_BID;
                            response2.params.result_info.current_bid=current_bid;
                            write(bidFds[i][0],&response2,sizeof(sm));
                            data2.type=response2.message_id;data2.pid=pids[i];data2.info=response2.params;
                            print_output(&data2,i);
                        }

                        else if(bmessage.params.bid<current_bid){
                            response2.params.result_info.result=BID_LOWER_THAN_CURRENT;
                            response2.params.result_info.current_bid=current_bid;
                            write(bidFds[i][0],&response2,sizeof(sm));
                            data2.type=response2.message_id;data2.pid=pids[i];data2.info=response2.params;
                            print_output(&data2,i);
                        }

                        else if(bmessage.params.bid<(current_bid+minimum_increment)){
                            response2.params.result_info.result=BID_INCREMENT_LOWER_THAN_MINIMUM;
                            response2.params.result_info.current_bid=current_bid;
                            write(bidFds[i][0],&response2,sizeof(sm));
                            data2.type=response2.message_id;data2.pid=pids[i];data2.info=response2.params;
                            print_output(&data2,i);
                        }

                        else{
                            response2.params.result_info.result=BID_ACCEPTED;
                            current_bid=bmessage.params.bid;
                            winner_id=i;
                            response2.params.result_info.current_bid=current_bid;
                            write(bidFds[i][0],&response2,sizeof(sm));
                            data2.type=response2.message_id;data2.pid=pids[i];data2.info=response2.params;
                            print_output(&data2,i);
                        }
                    }

                    else{
                        active--;
                        pfds[i].fd=-1;
                        status[i]=bmessage.params.status;
                        ii val3={bmessage.message_id,pids[i],bmessage.params};
                        print_input(&val3,i);

                    }
                }

            }
        }
    }
    sm lresponse;
    lresponse.message_id=SERVER_AUCTION_FINISHED;
    lresponse.params.winner_info.winner_id=winner_id;
    lresponse.params.winner_info.winning_bid=current_bid;
    print_server_finished(winner_id,current_bid);
    oi fdata;
    for(i=0;i<number_of_bidders;i++){
        write(bidFds[i][0],&lresponse,sizeof(sm));
        fdata.type=SERVER_AUCTION_FINISHED;
        fdata.pid=pids[i];
        fdata.info.winner_info.winner_id=winner_id;
        fdata.info.winner_info.winning_bid=current_bid;
        print_output(&fdata,i);
    }
    int stat;
    for(i=0;i<number_of_bidders;i++){
        close(bidFds[i][0]);
        close(bidFds[i][1]);
        wait(&stat);
        print_client_finished(i,status[i],(WEXITSTATUS(stat)==status[i]));
        
    }
    
    for(i=0;i<number_of_bidders;i++){
       
        for(j=1;j<(bidders[i].n_of_arg+1);j++){
            free(bidders[i].args[j]);
        }
        free(bidders[i].bpath);
        free(bidders[i].args);
        free(bidFds[i]);
        
    }
    free(pfds);
    free(bidFds);
    free(bidders);
    free(pids);
    free(delays);
    free(status);
    return 0;
    
}