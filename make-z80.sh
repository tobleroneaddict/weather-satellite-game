sdcc -mz80 --code-loc 0x0000 --data-loc 0x8000 --no-std-crt0 -o sat-code/cmdhandler.ihx sat-code/cmdhandler.c
makebin sat-code/cmdhandler.ihx sat-code/cmdhandler.bin


rm -rf sat-code/*.ihx
rm -rf sat-code/*.lk
rm -rf sat-code/*.lst
rm -rf sat-code/*.map
rm -rf sat-code/*.rel
rm -rf sat-code/*.noi
rm -rf sat-code/*.sym

