import employeedb_pb2
import sys

def ListEmployee(emp_db):
    for emp in emp_db.employee:
        print "Employee ID:", emp.id
        print "  Name:", emp.name
        if emp.email != "":
            print "  E-mail address:", emp.email
       
        for phone_number in emp.phones:
            if phone_number.type == employeedb_pb2.Employee.MOBILE:
                print "  Mobile phone #:",
            elif phone_number.type == employeedb_pb2.Employee.HOME:
                print "  Home phone #:",
            elif phone_number.type == employeedb_pb2.Employee.WORK:
                print "  Work phone #:",
            print phone_number.number

        print "  Age:", emp.age
        print "  DOB:", emp.dob

if len(sys.argv) != 2:
    print "Usage:", sys.argv[0], "ADDRESS_BOOK_FILE"
    sys.exit(-1)

emp_db = employeedb_pb2.EmployeeDb()

# Read the existing address book.
with open(sys.argv[1], "rb") as f:
    emp_db.ParseFromString(f.read())

ListEmployee(emp_db)
