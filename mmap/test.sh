echo -n "Before: "
echo Hello | tee hello.txt
if ./jello; then
  echo -n "After: "
  cat hello.txt
else
  echo "FAIL: jello"
fi
echo Hello > hello.txt
