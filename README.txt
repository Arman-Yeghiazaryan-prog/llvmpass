/home/arman/llvm/llvm-project/build/bin/clang -Xclang -disable-O0-optnone -emit-llvm -S -o formpreopt.ll clangtest.c
cd llvmpass/
rm -fr build/
LLVM_INSTALL_DIR=/home/arman/llvm/llvm-project/build/
LLVM_OPT=/home/arman/llvm/llvm-project/build/bin/opt
cmake -DLLVM_INSTALL_DIR=$LLVM_INSTALL_DIR -G "Unix Makefiles" -B build/ .
cd build/
cmake --build .
cd ../lib/
gcc -shared -fPIC -o libmy_c_code.so my_c_code.c
$LLVM_OPT -load-pass-plugin ../build/lib/libAddConst.so -passes="add-const" -S ~/llvm/formpreopt.ll -o ~/llvm/afteroptform.ll
/home/arman/llvm/llvm-project/build/bin/clang ~/llvm/afteroptform.ll -L . libmy_c_code.so -o ~/llvm/finalform
cd ~/llvm/
./finalform
