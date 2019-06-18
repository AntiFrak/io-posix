compile :
	rm -rf fryzjer
	mkdir fryzjer
	gcc main.c -o fryzjer/program -pthread

ctm :
	make compile
	clear
	make tm

tm :
	fryzjer/program 5
	
tmd :
	fryzjer/program 5 -debug
	
tz :
	fryzjer/program 5 -zmienne

tzd :
	fryzjer/program 5 -debug -zmienne
