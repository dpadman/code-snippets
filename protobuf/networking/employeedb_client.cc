#include <iostream>
#include <fstream>
#include <errno.h>
#include <unistd.h>
#include <build/employeedb.pb.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
using namespace google::protobuf::io;

static const int PORT = 9909;
static const char *IP = "127.0.0.1";

static char outbuf[1024];

void error(const char *msg) {
    perror(msg);
    exit(errno);
}

void usage(char **argv) {
    errno = EINVAL;
    sprintf(outbuf, "Usage: %s EMPLOYEEDB_FILE\n", argv[0]);
    error(outbuf);
}

void get_data_from_user(example::Employee *emp) {
    cout << "Enter employee ID number: ";
    int id;
    cin >> id;
    emp->set_id(id);
    cin.ignore(256, '\n');

    cout << "Enter name: ";
    getline(cin, *emp->mutable_name());

    cout << "Enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty()) {
	emp->set_email(email);
    }

    while (true) {
	cout << "Enter a phone number (or leave blank to finish): ";
	string number;
	getline(cin, number);
	if (number.empty()) {
	    break;
	}

	example::Employee::PhoneNumber* phone_number = emp->add_phones();
	phone_number->set_number(number);

	cout << "Is this a mobile, home, or work phone? ";
	string type;
	getline(cin, type);
	if (type == "mobile") {
	    phone_number->set_type(example::Employee::MOBILE);
	} else if (type == "home") {
	    phone_number->set_type(example::Employee::HOME);
	} else if (type == "work") {
	    phone_number->set_type(example::Employee::WORK);
	} else {
	    cout << "Unknown phone type.  Using default." << endl;
	}
    }
}

void send_data_to_server(example::EmployeeDb& emp_db) {

    int usrdata_sz = emp_db.ByteSize();
    int pktdata_sz = usrdata_sz + 4;
    char *pkt = new char[pktdata_sz];

    ArrayOutputStream aos(pkt, pktdata_sz);
    CodedOutputStream *coded_output = new CodedOutputStream(&aos);
    coded_output->WriteVarint32(emp_db.ByteSize());
    emp_db.SerializeToCodedStream(coded_output);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        error("socket failed!");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        error("connect failed!");
    }

    cout << "Sending data size to server: " << emp_db.ByteSize() << endl;

    int ret = send(sock, (void *) pkt, pktdata_sz, 0);
    if (ret != pktdata_sz) {
        error("send failed!");
    }

    close (sock);
    delete pkt;
}

int main(int argc, char **argv) {

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc != 2)
        usage(argv);

    example::EmployeeDb emp_db;
    fstream inp(argv[1], ios::in | ios::binary);
    if (!inp) {
	cout << argv[1] << " creating new file, file not found" << endl;
    } else if (!emp_db.ParseFromIstream(&inp)) {
	sprintf(outbuf, "Failed to parse employee database.\n");
	error(outbuf);
    }

    get_data_from_user(emp_db.add_employee());

    fstream out(argv[1], ios::out | ios::trunc | ios::binary);
    if (!emp_db.SerializeToOstream(&out)) {
	sprintf(outbuf, "Failed to write address book.\n");
    }

    send_data_to_server(emp_db);

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
