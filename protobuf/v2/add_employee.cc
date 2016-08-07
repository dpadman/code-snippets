#include <iostream>
#include <fstream>
#include <errno.h>
#include <build/employeedb.pb.h>
using namespace std;

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

    cout << "Enter the age (or leave blank to finish): ";
    int age = 0;
    cin >> age;
    cin.ignore(256, '\n');
    if (age) {
        emp->set_age(age);
    }

    cout << "Enter the data of birth (mm/dd/yyyy): ";
    string dob;
    getline(cin, dob);
    if (!dob.empty()) {
        emp->set_dob(dob);
    }
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

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
