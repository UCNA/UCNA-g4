echo "starting thread 1"
./bin/Linux-g++/UCNb 100 0 0 0 a > output/100_5_1.run &
sleep 2
echo "starting thread 2"
./bin/Linux-g++/UCNb 100 0 0 0 b > output/100_5_2.run &
sleep 2
echo "starting thread 3"
./bin/Linux-g++/UCNb 100 0 0 0 c > output/100_5_3.run &
sleep 2
echo "starting thread 4"
./bin/Linux-g++/UCNb 100 0 0 0 d > output/100_5_4.run &
echo "waiting for threads to finish"
# watch "ps -ef | grep [L]inux-g++/UCNb"
wait
echo "processing results"
cat output/100_5_1.run output/100_5_2.run output/100_5_3.run output/100_5_4.run >> output/100_5.run
grep UCNb_event output/100_5.run | gawk '{print $2,$3,$4,$5,$6,$7,$8,$9}' >> output/100_5.out
echo "done"

