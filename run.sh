if [ -f out ]; then
    rm out
fi

echo "compiling..."
g++ -g -Wall -O2 -std=c++17 -o out $1

if [ -f out ]; then
    echo "successfully compiled."
    echo "running..."
    time ./out
else
    echo "compilation failed."
fi
