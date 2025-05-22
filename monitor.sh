while [ 1 ]
do
    last=$(awk '{ print length }' out.txt | tail -n 1)
    sleep 5
    cur=$(awk '{ print length }' out.txt | tail -n 1)
    dif=$(expr $(($cur-$last)))

    clear
    echo "${cur} | ${dif}"
    echo "40403592"
done
