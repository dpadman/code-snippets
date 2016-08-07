#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <build/employeedb.pb.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
using namespace google::protobuf::io;

typedef struct tdata {
    int sock;
    int ret;
} tdata_t;

static const int PORT = 9909;

void error(const char *msg) {
    perror(msg);
    exit(errno);
}

void *DataHandler(void *arg) {
    example::EmployeeDb emp_db;
    tdata_t *tdata = (tdata_t*)arg;
    int sock = tdata->sock;

    char buf[4];
    if (recv(sock, &buf, sizeof(buf), MSG_PEEK) != sizeof(buf)) {
        error("recv 1 failed!");
    }

    google::protobuf::uint32 size;
    google::protobuf::io::ArrayInputStream ais(buf, 4);
    CodedInputStream coded_input(&ais);
    coded_input.ReadVarint32(&size);

    cout << "Received data size: " << size << endl;

    char data[size + 4];
    // Read the entire buffer including the hdr
    if(recv(sock, (void *)data, 4 + size, MSG_WAITALL) != (size + 4)){
        error("recv 2 error");
    }

    new (&ais) ArrayInputStream(data, size + 4);

    new (&coded_input) CodedInputStream(&ais);
    coded_input.ReadVarint32(&size);
    CodedInputStream::Limit msgLimit = coded_input.PushLimit(size);
    emp_db.ParseFromCodedStream(&coded_input);
    coded_input.PopLimit(msgLimit);

    cout << "Employee Database\n====================\n" 
         << emp_db.DebugString() << endl;

    tdata->ret = 0;
    return &tdata->ret;
}

void listen_for_clients(void) {

    socklen_t len;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        error("socket failed!");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET ;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY ;

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
        error("bind failed!");
    }

    if (listen(sock, 10) < 0) {
        error("listen failed!");
    }

    while (true) {
        pthread_t tid = 0;
        struct sockaddr_in sender;
        int rsock = accept(sock, (struct sockaddr*)&sender, &len);
        if (rsock < 0) {
            error("accept failed!");
        }
        tdata_t *data = new tdata_t;
        data->sock = rsock;
        pthread_create(&tid, 0, &DataHandler, (void*)data);
        pthread_detach(tid);
    }
}

int main(int argc, char **argv) {
    listen_for_clients();
    return 0;
}
