compile :
	rm -rf fryzjer
	mkdir fryzjer
	gcc main.c -o fryzjer/program -pthread

ctm :
	make compile
	clear
	make tm

tm :
	compiled/fryzjer 5
	
tmd :
	compiled/fryzjer 5 -debug
	
tz :
	compiled/fryzjer 5 -zmienne

tzd :
	compiled/fryzjer 5 -debug -zmienne