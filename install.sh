rm -rf build && mkdir build && cd build && cmake ..
make || exit 1
make install
cd ../bindings/python-rj/ && rm -rf build
python3 setup.py install || exit 1
#cd ../../tests/Python_and_core && python3 test.py
