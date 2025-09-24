echo -n "Before: "
echo Hello | tee hello.txt
./jello
echo -n "After: "
cat hello.txt
