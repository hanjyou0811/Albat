#!/bin/sh
#  .
#  ├─ run_test.sh  ←　これ
#  ├─ makefile
#  ├─ bin/albat
#  └─ sample/*.al

if [ ! -f bin/albat ]; then
    echo "albat が存在しません。make を実行します..."
    make || { echo "make に失敗しました"; exit 1; }
fi

SAMPLE_DIR="sample"
ALBAT="bin/albat"

for alfile in "$SAMPLE_DIR"/*.al; do
    if [ ! -e "$alfile" ]; then
        echo "sample ディレクトリに .al ファイルがありません。"
        exit 0
    fi

    base=$(basename "$alfile" .al)
    cppfile="$SAMPLE_DIR/$base.cpp"

    echo "==> $alfile ..."
    "$ALBAT" < "$alfile" > "$cppfile"

    if [ $? -ne 0 ]; then
        echo "albat の実行中にエラーが発生しました: $alfile"
        exit 1
    fi

    echo "       : $cppfile"
done
echo "1.OK (al -> cpp)"

for cppfile in "$SAMPLE_DIR"/*.cpp; do
    if [ ! -e "$cppfile" ]; then
        echo "sampleディレクトリに cppファイルがありません。"
        exit 0
    fi

    base=$(basename "$cppfile" .cpp)
    cppfile="$SAMPLE_DIR/$base.cpp"
    execfile="$base"

    echo "g++ $cppfile"
    echo "$SAMPLE_DIR/$base.out"
    g++ -o "$SAMPLE_DIR/$base" "$cppfile"

    if [ $? -ne 0 ]; then
        echo "コンパイル中にエラーが発生しました: $cppfile"
        exit 1
    fi

    if [ "$base" = "io" ]; then
        echo " input : '3 4 5'"
        expect="12 60"
        result=$(echo "3 4 5" | "$SAMPLE_DIR/$base" | tr '\n' ' ')
        result=$(echo "$result" | sed 's/ $//') 

        echo " output: $result"
        echo " expected: $expect"

        if [ "$result" = "$expect" ]; then
            echo "==> OK---"
        else
            echo "==> NG---"
            exit 1
        fi
    fi
    if [ "$base" = "op" ]; then
        echo " input : '5 3'"
        expect="125 2 3"
        result=$(echo "5 3" | "$SAMPLE_DIR/$base" | awk '{print $NF}' | tr '\n' ' ')
        result=$(echo "$result" | sed 's/ $//') 

        echo " output: $result"
        echo " expected: $expect"

        if [ "$result" = "$expect" ]; then
            echo "==> OK---"
        else
            echo "==> NG---"
            exit 1
        fi
    fi

    if [ "$base" = "rep" ]; then
        echo " input : '10'"
        expect="1 3 6 10 15 21 28 36 45 55"
        result=$(echo "10" | "$SAMPLE_DIR/$base" | tr '\n' ' ')
        result=$(echo "$result" | sed 's/ $//') 

        echo " output: $result"
        echo " expected: $expect"

        if [ "$result" = "$expect" ]; then
            echo "==> OK---"
        else
            echo "==> NG---"
            exit 1
        fi
    fi

    if [ "$base" = "sample1" ]; then
        echo " input : '3 5 1 2 3 1 2 4'"
        expect="Yes"
        result=$(echo "3 5 1 2 3 1 2 4" | "$SAMPLE_DIR/$base" | tr '\n' ' ')
        result=$(echo "$result" | sed 's/ $//') 

        echo " output: $result"
        echo " expected: $expect"

        if [ "$result" = "$expect" ]; then
            echo "==> OK---"
        else
            echo "==> NG---"
            exit 1
        fi
    fi

    if [ "$base" = "sample2" ]; then
        echo " input : '3 3 11 1 3 9 1 2 4 2 4 0 1 2 2 1 3 1 2'"
        expect="Yes No Yes"
        result=$(echo "3 3 11 1 3 9 1 2 4 2 4 0 1 2 2 1 3 1 2" | "$SAMPLE_DIR/$base" | tr '\n' ' ')
        result=$(echo "$result" | sed 's/ $//') 

        echo " output: $result"
        echo " expected: $expect"

        if [ "$result" = "$expect" ]; then
            echo "==> OK---"
        else
            echo "==> NG---"
            exit 1
        fi
    fi

    if [ "$base" = "tuple" ]; then
        echo " input : ''"
        expect="a : 1, b : 2, c : 3 a : 2, b : 3, c : 1"
        result=$(echo "" | "$SAMPLE_DIR/$base" | tr '\n' ' ')
        result=$(echo "$result" | sed 's/ $//') 

        echo " output: $result"
        echo " expected: $expect"

        if [ "$result" = "$expect" ]; then
            echo "==> OK---"
        else
            echo "==> NG---"
            exit 1
        fi
    fi

    echo " done : $cppfile"
done
echo "2.OK (cpp)"