# Google protobuf example code-snippets

Building and running the app
----------------------------

pip install protobuf==3.0.0
Download the https://github.com/google/protobuf.git, compile and install executable
git clone https://github.com/dpadman/code-snippets.git
sudo apt-get install -y cmake
mkdir -p protobuf/build/
cd protobuf/build/
cmake .. && make
./add_employee employeedb.dat
python list_employee.pyc employeedb.dat
