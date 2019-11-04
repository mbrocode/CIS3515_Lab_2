/*main{
    //create new clients
}

void* clientFunc(void* arges){
    int sd = 0;
    struct sockaddr_in server_addr;
    if((sd = socket(AF_INT, SOCK_STREAM, 0)) < 0)
    {
        printf("\nSocket creation error \n");
        return NULL;
    }

    memset(&server_adddr, '0', sizeof(server_addr))
}

server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);

if(inet_pton(AF_INT, localhost, &server_addr.sin_addr) <=0)
{
    printf("invalid address, thread is terminating");
    return NULL;
}

if (connect(sd, (struct sockaddr *)&server_addr, sizeof(server)adr) )



sendWords*/